#include <frameBuffer.h>
#include <lld.h>

typedef enum {
    EVENT_MOUSE = 0,
    EVENT_KEYBOARD = 1,
} EventType;

typedef struct Event
{
    EventType type;
} Event_t;

typedef struct EventMouse
{
    EventType type;
    int x;
    int y;
    uint32_t data;
} EventMouse_t;

typedef struct EventKeyboard
{
    EventType type;
} EventKeyboard_t;

typedef struct Window
{
    int pos[2];
    int size[2];
    int const id;
    framebuffer_t *buffer;
    bool (*eventHandler)(struct Window *win, Event_t *event);
    bool toDelete;
} Window_t;

typedef struct WindowManager
{
    lld_t *windows;
} WindowManager_t;

Window_t *Window_init(int sizex, int sizey);
void Window_draw(Window_t *win);

WindowManager_t *WindowManager_init();
Window_t *WindowManager_create_Window(WindowManager_t *winManager, int sizex, int sizey);
void WindowManager_draw_all(WindowManager_t *winManager);
Window_t *WindowManager_get_Window(WindowManager_t *winManager, int id);

void draw_fb_win(int id, void *pixels, rect_t *rect);


extern WindowManager_t *g_WindowManager;
