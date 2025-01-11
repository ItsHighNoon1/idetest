#pragma once

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 500

int os_createWindow();
int os_windowCloseRequested();
void os_poll();
void os_destroyWindow();