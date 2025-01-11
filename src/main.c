#include "os.h"
#include "render.h"

int main() {
    os_createWindow();
    render_init();

    while (!os_windowCloseRequested()) {
        os_poll();
    }

    render_destroy();
    os_destroyWindow();

    return 0;
}