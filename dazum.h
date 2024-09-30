#ifndef DAZUM_H
#define DAZUM_H

#define CAMERA_ZOOM_DELTA 0.1f
#define FLASHLIGHT_ZOOM 100.0f
#define FLASHLIGHT_ZOOM_DELTA 10.0f

#include <stdio.h>

void convert_to_rgba(unsigned char *data, const int width, const int height, int stride);
int take_screenshot(const char *filename, const int width, const int height);
int get_screen_width(void);
int get_screen_height(void);
void get_mouse_position(int *x, int *y);

#endif // DAZUM_H
