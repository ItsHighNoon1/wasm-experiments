#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdio.h>

extern void dom_set_innertext(const char* target, const char* inner_text);

bool on_click(int event_type, const EmscriptenMouseEvent* mouse_event, void* user_data) {
    emscripten_console_log("C on_click()");
    dom_set_innertext("#button", "Changed...");
    return false;
}

int main() {
    printf("C main()\n");
    emscripten_set_click_callback("#button", NULL, true, on_click);
}