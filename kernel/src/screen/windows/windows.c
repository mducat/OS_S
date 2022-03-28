#include <frameBuffer.h>
#include <windows.h>
#include <malloc.h>
#include <screen.h>

#include <mouse.h>
#include <string.h>

void WindowManager_focus_window(WindowManager_t *winManager, Window_t *win);
void WindowManager_update_screen(WindowManager_t *winManager);


WindowManager_t *g_WindowManager = 0;

bool is_in_rect(int rect[4], int p[2]) {
    if (rect[0] < p[0] && p[0] < rect[0]+rect[2] &&
        rect[1] < p[1] && p[1] < rect[1]+rect[3])
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

void Window_free(Window_t *win) {
    framebuffer_destroy(win->buffer);
    free(win);
}

void Window_update_from_buffer(Window_t *win, uint32_t *pixels, int size[2]) {
    if (win->size[0] == size[0] && win->size[1] == size[1]) {

    } else {
        framebuffer_destroy(win->buffer);
        win->buffer = framebuffer_create(size[0], size[1]);
        win->size[0] = size[0];
        win->size[1] = size[1];
    }
    framebuffer_t *src = framebuffer_create_from_uint32_array(size[0], size[1], pixels);
    my_draw_buffer_on_buffer(win->buffer, src, 0, 0);
    free(src);
}


static Window_t *g_Window_try_Move_dest;
static int g_Window_try_Move_pix_offset[2] = {0, 0};
static void (*mouse_click_call_back_old)(int x, int y, int32_t type) = 0;

void Window_try_Move_callBack(int x, int y, int32_t type) {
    static framebuffer_t *buf = 0;
    static bool isPrev = false;
    static int prevPos[2] = {0, 0};
    if (!buf) {
        buf = framebuffer_create_from_uint32_array(disp->screen->x_len, disp->screen->y_len, (uint32_t *)disp->screen->p_loc);
    }
    if (type & 1) {
        if (isPrev) {
            Vector2f_t cursorLines[] = {
                {prevPos[0]+g_Window_try_Move_pix_offset[0]+g_Window_try_Move_dest->size[0],    prevPos[1]+g_Window_try_Move_pix_offset[1]},
                {prevPos[0]+g_Window_try_Move_pix_offset[0]+g_Window_try_Move_dest->size[0],    prevPos[1]+g_Window_try_Move_pix_offset[1]-20},
                {prevPos[0]+g_Window_try_Move_pix_offset[0],                                    prevPos[1]+g_Window_try_Move_pix_offset[1]-20},
                {prevPos[0]+g_Window_try_Move_pix_offset[0],                                    prevPos[1]+g_Window_try_Move_pix_offset[1]},
                {prevPos[0]+g_Window_try_Move_pix_offset[0]+g_Window_try_Move_dest->size[0],    prevPos[1]+g_Window_try_Move_pix_offset[1]},
                {prevPos[0]+g_Window_try_Move_pix_offset[0]+g_Window_try_Move_dest->size[0],    prevPos[1]+g_Window_try_Move_pix_offset[1]+g_Window_try_Move_dest->size[1]},
                {prevPos[0]+g_Window_try_Move_pix_offset[0],                                    prevPos[1]+g_Window_try_Move_pix_offset[1]+g_Window_try_Move_dest->size[1]},
                {prevPos[0]+g_Window_try_Move_pix_offset[0],                                    prevPos[1]+g_Window_try_Move_pix_offset[1]},
            };
            my_xor_lines(buf, cursorLines, 8);
        }
        isPrev = true;
        prevPos[0] = x;
        prevPos[1] = y;
        Vector2f_t cursorLines[] = {
            {x+g_Window_try_Move_pix_offset[0]+g_Window_try_Move_dest->size[0],    y+g_Window_try_Move_pix_offset[1]},
            {x+g_Window_try_Move_pix_offset[0]+g_Window_try_Move_dest->size[0],    y+g_Window_try_Move_pix_offset[1]-20},
            {x+g_Window_try_Move_pix_offset[0],                                    y+g_Window_try_Move_pix_offset[1]-20},
            {x+g_Window_try_Move_pix_offset[0],                                    y+g_Window_try_Move_pix_offset[1]},
            {x+g_Window_try_Move_pix_offset[0]+g_Window_try_Move_dest->size[0],    y+g_Window_try_Move_pix_offset[1]},
            {x+g_Window_try_Move_pix_offset[0]+g_Window_try_Move_dest->size[0],    y+g_Window_try_Move_pix_offset[1]+g_Window_try_Move_dest->size[1]},
            {x+g_Window_try_Move_pix_offset[0],                                    y+g_Window_try_Move_pix_offset[1]+g_Window_try_Move_dest->size[1]},
            {x+g_Window_try_Move_pix_offset[0],                                    y+g_Window_try_Move_pix_offset[1]},
        };
        my_xor_lines(buf, cursorLines, 8);
    } else {
        isPrev = false;
        mouse_click_call_back = mouse_click_call_back_old;
        g_Window_try_Move_dest->pos[0] = x+g_Window_try_Move_pix_offset[0];
        g_Window_try_Move_dest->pos[1] = y+g_Window_try_Move_pix_offset[1];
        WindowManager_update_screen(g_WindowManager);
    }
}

bool Window_handle_event(Window_t *win, Event_t *event) {
    if (event->type == EVENT_MOUSE) {
        EventMouse_t *eventMouse = (EventMouse_t *)event;
        int pos[2] = {eventMouse->x, eventMouse->y};

        
        { // is inside window
            int rect[4] = {win->pos[0], win->pos[1]-20, win->size[0], win->size[1]+20};
            if (!is_in_rect(rect, pos)) {
                return 0;
            }
        }

        // click
        if (eventMouse->data & 3) {
            WindowManager_focus_window(g_WindowManager, win);   
        }
        // left click
        if (eventMouse->data & 1) {
            { // delete buton
                int rect[4] = {win->pos[0]+win->size[0]-18, win->pos[1]-18, 16, 16};
                if (is_in_rect(rect, pos)) {
                    win->toDelete = true;
                    return true;
                } 
            }
            { // drag & drop
                int rect[4] = {win->pos[0], win->pos[1]-20, win->size[0], 20};
                if (is_in_rect(rect, pos)) {
                    g_Window_try_Move_dest = win;
                    mouse_click_call_back_old = mouse_click_call_back;
                    mouse_click_call_back = &Window_try_Move_callBack;
                    g_Window_try_Move_pix_offset[0] = win->pos[0]-pos[0];
                    g_Window_try_Move_pix_offset[1] = win->pos[1]-pos[1];
                    return true;
                }
            }
        }
        
    }
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

WindowManager_t *g_event_window_manager;

void WindowManager_handle_event(WindowManager_t *, Event_t *);

void mouseClickCallBack_Window(int x, int y, int32_t type) {
    EventMouse_t event;
    event.x = x;
    event.y = y;
    event.data = type;
    event.type = EVENT_MOUSE;
    WindowManager_handle_event(g_event_window_manager, (Event_t*)&event);
}

WindowManager_t *WindowManager_init() {
    WindowManager_t *winManager = (WindowManager_t *)malloc(sizeof(WindowManager_t));
    winManager->windows = lld_init();
    g_event_window_manager = winManager;
    mouse_click_call_back = &mouseClickCallBack_Window;
    return winManager;
}   

Window_t *WindowManager_create_Window(WindowManager_t *winManager, int sizex, int sizey) {
    Window_t *win = Window_init(sizex, sizey);
    lld_insert(winManager->windows, lld_len(winManager->windows), win);
    return win;
}

void WindowManager_focus_window(WindowManager_t *winManager, Window_t *win) {
    lld_t *wins = winManager->windows->next;
    int i = 0;
    while (wins) {
        Window_t *data = wins->data;
        if (data == win) {
            void *node = lld_pop_node(winManager->windows, i);
            lld_insert_node(winManager->windows, lld_len(winManager->windows), node);
            return;
        }
        wins = wins->next;
        i++;
    }
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
    if (event)
        while (wins) {
            Window_t *win = wins->data;
            wins = wins->next;
            if (Window_handle_event(win, event))
                return;
        }
    // clear windows
    wins = winManager->windows->next;
    int i = 0;
    bool needRefresh = false;
    while (wins) {
        Window_t *win = wins->data;
        if (win->toDelete) {
            needRefresh = true;
            lld_t *next = wins->next;
            Window_free(lld_pop(wins, i));
            wins = next;
        } else {
            wins = wins->next;
        }
        i++;
    }
    if (needRefresh) {
        WindowManager_update_screen(winManager);
    }
}


void WindowManager_update_screen(WindowManager_t *winManager) {
    framebuffer_t *buf = 0;
    if (!buf)
        buf = framebuffer_create_from_uint32_array(disp->screen->x_len, disp->screen->y_len, (uint32_t *)disp->back);
    
    Color_t color;
    memset(&color, 0, sizeof(color));
    my_clear_buffer(buf, color);
    WindowManager_draw_all(winManager);
    refresh();
}

void draw_fb_win(int id, void *pixels, rect_t *rect) {
    Window_t *win = WindowManager_get_Window(g_WindowManager, id);
    int size[2] = {rect->dx, rect->dy};
    if (!win) {
        win = WindowManager_create_Window(g_WindowManager, size[0], size[1]);
    }
    Window_update_from_buffer(win, pixels, size);
    WindowManager_update_screen(g_WindowManager);
}