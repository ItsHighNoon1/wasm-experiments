#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdio.h>

extern void dom_set_innertext(const char* target, const char* inner_text);

bool on_click(int event_type, const EmscriptenMouseEvent* mouse_event, void* user_data) {
    /* ### RANT: ###
    Ok, so despite the fact that I have set noExitRuntime: true
    over in prejs.js, the runtime is actually still doing some
    cleanup behind the scenes. One of these things is standard out,
    and I was bashing my head for like 30 minutes trying to figure
    out why noExitRuntime: true wasn't working (which it isn't)
    until I eventually said "oh let's try console.log instead"
    which worked. So now with a working event handler I was like
    ok what happens if I set noExitRuntime: false? And the answer
    is exactly the same literally NOTHING changed but whatever
    just something to keep in mind I guess...
    */
    emscripten_console_log("C on_click()");
    dom_set_innertext("#button", "Changed...");
    return false;
}

int main() {
    printf("C main()\n");
    emscripten_set_click_callback("#button", NULL, true, on_click);
}