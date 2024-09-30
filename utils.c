#include "dazum.h"

Camera2D init_camera(void)
{
	Camera2D camera;

	camera.offset = (Vector2){0, 0};
	camera.target = (Vector2){0, 0};
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	return (camera);
}

Vector2 custom_get_mouse_position(void)
{
	int originalMousePositionX;
	int originalMousePositionY;
	get_mouse_position(&originalMousePositionX, &originalMousePositionY);
	return ((Vector2){
		(float)originalMousePositionX,
		(float)originalMousePositionY,
	});
}

void reset_camera(Camera2D *camera)
{
	camera->target = (Vector2){0, 0};
	camera->offset = (Vector2){0, 0};
	camera->zoom = 1.0f;
}

void drag_position(Camera2D *camera)
{
	Vector2 delta = GetMouseDelta();
	delta.x *= -1.0f / camera->zoom;
	delta.y *= -1.0f / camera->zoom;
	camera->target.x += delta.x;
	camera->target.y += delta.y;
}

void handle_zoom(Camera2D *camera, float *flashlightRadius, bool isFlashlightMode)
{
	float mouseMovement = GetMouseWheelMove();
	if (IsKeyDown(KEY_LEFT_CONTROL) && isFlashlightMode)
	{
		if (mouseMovement > 0 && *flashlightRadius > FLASHLIGHT_ZOOM_DELTA)
			*flashlightRadius -= FLASHLIGHT_ZOOM_DELTA;
		else if (mouseMovement < 0)
			*flashlightRadius += FLASHLIGHT_ZOOM_DELTA;
	}
	else if (mouseMovement != 0)
	{
		camera->target = GetScreenToWorld2D(custom_get_mouse_position(), *camera);
		camera->offset = custom_get_mouse_position();
		if (mouseMovement > 0)
			camera->zoom += CAMERA_ZOOM_DELTA;
		else if (mouseMovement < 0 && camera->zoom > CAMERA_ZOOM_DELTA)
			camera->zoom -= CAMERA_ZOOM_DELTA;
	}
}

int get_screen_width(void)
{
#ifdef __linux__
	Display *display = XOpenDisplay(NULL);
	if (!display)
		return (0);
	int width = XDisplayWidth(display, 0);
	return (width);
#elif _WIN32
	return (GetSystemMetrics(SM_CXSCREEN));
#endif
}

int get_screen_height(void)
{
#ifdef __linux__
	Display *display = XOpenDisplay(NULL);
	if (!display)
		return (0);
	int height = XDisplayHeight(display, 0);
	XCloseDisplay(display);
	return (height);
#elif _WIN32
	return (GetSystemMetrics(SM_CYSCREEN));
#endif
}

void get_mouse_position(int *x, int *y)
{
#ifdef __linux
	Display *display = XOpenDisplay(NULL);
	if (!display)
		return;
	Window root = RootWindow(display, 0);
	unsigned int mask;
	XQueryPointer(display, root, &root, &root, x, y, x, y, &mask);
	XCloseDisplay(display);
#elif _WIN32
	POINT point;
	GetCursorPos(&point);
	*x = point.x;
	*y = point.y;
#endif
}

void convert_to_rgba(unsigned char *data, const int width, const int height, const int stride)
{
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int offset = y * stride + x * 4;
			unsigned char r = data[offset + 2];
			unsigned char g = data[offset + 1];
			unsigned char b = data[offset + 0];
			unsigned char a = 255;
			data[offset + 0] = r;
			data[offset + 1] = g;
			data[offset + 2] = b;
			data[offset + 3] = a;
		}
	}
}

int take_screenshot(const char *filename, const int width, const int height)
{
	const int comps = 4;
	const int stride = width * comps;
#ifdef __linux__
	Display *display = XOpenDisplay(NULL);
	if (!display)
		return (1);
	Window root = RootWindow(display, 0);
	XImage *image = XGetImage(display, root, 0, 0, width, height, AllPlanes, ZPixmap);
	if (!image)
	{
		XFree(image->data);
		XFree(image);
		XCloseDisplay(display);
		return (1);
	}
	unsigned char *data = (unsigned char *)image->data;
	convert_to_rgba(data, width, height, stride);
	stbi_write_png(filename, width, height, comps, data, stride);
	XFree(image->data);
	XFree(image);
	XCloseDisplay(display);
#elif _WIN32
	unsigned char *data = (unsigned char *)malloc(stride * height);
	HDC hdc = GetDC(NULL);
	HDC memdc = CreateCompatibleDC(hdc);
	HBITMAP bitmap = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(memdc, bitmap);
	BitBlt(memdc, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
	BITMAPINFOHEADER bi = {0};
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	GetDIBits(memdc, bitmap, 0, height, data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);
	convert_to_rgba(data, width, height, stride);
	stbi_write_png(filename, width, height, comps, data, stride);
	free(data);
	DeleteObject(bitmap);
	DeleteDC(memdc);
	ReleaseDC(NULL, hdc);
#endif
	return (0);
}
