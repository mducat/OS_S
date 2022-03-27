#include <frameBuffer.h>
#include <windows.h>
#include <malloc.h>
#include <screen.h>

WindowManager_t *g_WindowManager = 0;

bool is_in_rect(int rect[4], int p[2]) {
    int d[2] = {
        rect[0]+rect[2]/2-p[0],
        rect[1]+rect[3]/2-p[1],
    };
    int size[2] = {
        rect[2]/2,
        rect[3]/2,
    };
    if (d[0]*d[0] + d[1]*d[1] < size[0]*size[1])
        return true;
    return false;
}

Window_t *Window_init(int sizex, int sizey) {
    Window_t *window = (Window_t *)malloc(sizeof(Window_t));
    window->pos[0] = 0;
    window->pos[1] = 0;
    window->size[0] = sizex;
    window->size[1] = sizey;
    window->buffer = framebuffer_create(sizex, sizey);
    static int idRoll = 0;
    *(int *)&window->id = idRoll++;
    window->eventHandler = 0;
    window->toDelete = 0;
    return window;
}

bool Window_handle_event(Window_t *win, Event_t *event) {
    int pos_size[4] = {win->pos[0]+win->size[0]-18, win->pos[1]-18, 16, 16};

    return win->eventHandler ? win->eventHandler(win, event) : 0;
}

void Window_draw_header(Window_t *win) {
    static framebuffer_t *bufBack = 0;
    if (!bufBack)
        bufBack = framebuffer_create_from_uint32_array(disp->screen->x_len, disp->screen->y_len, (uint32_t *)disp->back);

    Color_t headerColor;
    headerColor.r = 50;
    headerColor.g = 50;
    headerColor.b = 50;
    headerColor.a = 255;
    {
        int pos_size[4] = {win->pos[0], win->pos[1]-20, win->size[0], 20};
        my_draw_rect(bufBack, pos_size, headerColor);
    }

    // draw close button
    headerColor.r = 25;
    headerColor.g = 25;
    headerColor.b = 25;
    headerColor.a = 255;
    int pos_size[4] = {win->pos[0]+win->size[0]-18, win->pos[1]-18, 16, 16};
    {
        my_draw_rect(bufBack, pos_size, headerColor);
    }
    headerColor.r = 255;
    headerColor.g = 255; 
    headerColor.b = 255;
    headerColor.a = 255;
    my_draw_line(bufBack, 
        (Vector2f_t){pos_size[0], pos_size[1]}, 
        (Vector2f_t){pos_size[0]+pos_size[2]-1, pos_size[1]+pos_size[3]-1},
        headerColor
    );
    my_draw_line(bufBack, 
        (Vector2f_t){pos_size[0], pos_size[1]+pos_size[3]-1}, 
        (Vector2f_t){pos_size[0]+pos_size[2]-1, pos_size[1]}, 
        headerColor
    );
}

void Window_draw(Window_t *win) {
    static framebuffer_t *bufBack = 0;
    if (!bufBack)
        bufBack = framebuffer_create_from_uint32_array(disp->screen->x_len, disp->screen->y_len, (uint32_t *)disp->back);
    
    Window_draw_header(win);

    my_draw_buffer_on_buffer(bufBack, win->buffer, win->pos[0], win->pos[1]);
}

WindowManager_t *WindowManager_init() {
    WindowManager_t *winManager = (WindowManager_t *)malloc(sizeof(WindowManager_t));
    winManager->windows = lld_init();
    return winManager;
}   

Window_t *WindowManager_create_Window(WindowManager_t *winManager, int sizex, int sizey) {
    Window_t *win = Window_init(sizex, sizey);
    lld_insert(winManager->windows, lld_len(winManager->windows), win);
    return win;
}

void WindowManager_draw_all(WindowManager_t *winManager) {
    lld_t *wins = winManager->windows->next;
    while (wins) {
        Window_t *win = wins->data;
        // draw window;
        Window_draw(win);
        wins = wins->next;
    }
}

Window_t *WindowManager_get_Window(WindowManager_t *winManager, int id) {
    lld_t *wins = winManager->windows->next;
    while (wins) {
        Window_t *win = wins->data;
        if (win->id == id)
            return win;
        wins = wins->next;
    }
    return 0;
}

void WindowManager_handle_event(WindowManager_t *winManager, Event_t *event) {
    lld_t *wins = winManager->windows->next;
    while (wins) {
        Window_t *win = wins->data;
        wins = wins->next;
    }
}