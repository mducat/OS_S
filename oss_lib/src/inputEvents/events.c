#include <my.h>

typedef struct event_manager {
} event_manager_t;


enum event_type {
    KEYBOARD_INPUT,
    MOUSE_CLICK,
    EXIT,
};


typedef struct event {
    enum event_type type;
    int val;
} event_t;


// init event_manager_t
void init_event_loader(event_manager_t *event_geter){
    int rs = read(); // blocking if 
}

// return true if event_t has been set²
bool get_event(event_t *event) {
    event->val = read();
    return true;
}
