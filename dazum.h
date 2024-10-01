#ifndef DAZUM_H
# define DAZUM_H

/* @------------------------------------------------------------------------@ */
/* |                            Define Section                              | */
/* @------------------------------------------------------------------------@ */

# define CAMERA_ZOOM_DELTA 0.1f
# define FLASHLIGHT_ZOOM 100.0f
# define FLASHLIGHT_ZOOM_DELTA 10.0f
# define FLASHLIGHT_SHADER_CODE "#version 330 core\n"\
								"uniform sampler2D noiseTexture;\n"\
								"uniform vec2 screenSize;\n"\
								"uniform vec2 flashlightCenter;\n"\
								"uniform float flashlightRadius;\n"\
								"out vec4 fragColor;\n"\
								"void main()\n"\
								"{\n"\
								"	vec2 uv = gl_FragCoord.xy / screenSize;\n"\
								"	float distance = length(gl_FragCoord.xy - flashlightCenter);\n"\
								"	if (distance < flashlightRadius) fragColor = texture(noiseTexture, uv);\n"\
								"	else fragColor = vec4(0.0, 0.0, 0.0, 0.8);\n"\
								"}"

/* @------------------------------------------------------------------------@ */
/* |                            Include Section                             | */
/* @------------------------------------------------------------------------@ */

# include <stdio.h>
# include <stdbool.h>

# define STD_IMAGE_WRITE_IMPLEMENTATION
# include "stb_image_write.h"

# if defined(_WIN32)
# include <stdlib.h>
// Redefining conflicting windows function names with raylib
#  define Rectangle _WIN32_Rectangle
#  define CloseWindow _WIN32_CloseWindow
#  define ShowCursor _WIN32_ShowCursor
#  define DrawText _WIN32_DrawText
#  define DrawTextEx _WIN32_DrawTextEx
#  define LoadImage _WIN32_LoadImage
#  define PlaySound _WIN32_PlaySound
#  include "windows.h"
// Undefining previous defines so raylib is include safely
#  undef Rectangle
#  undef CloseWindow
#  undef ShowCursor
#  undef DrawText
#  undef DrawTextEx
#  undef LoadImage
#  undef PlaySound
# endif // _WIN32

# ifdef __linux__
#  include "X11/Xlib.h"
# endif // __linux__

# include "raylib.h"

/* @------------------------------------------------------------------------@ */
/* |                           Function Section                             | */
/* @------------------------------------------------------------------------@ */

int take_screenshot(const char *filename, const int width, const int height);
int get_screen_width(void);
int get_screen_height(void);
void get_mouse_position(int *x, int *y);
Vector2 custom_get_mouse_position(void);
Camera2D init_camera(void);
void reset_camera(Camera2D *camera);
void drag_position(Camera2D *camera);
void handle_zoom(Camera2D *camera, float *flashlightRadius, bool isFlashlightMode);

#endif // DAZUM_H
