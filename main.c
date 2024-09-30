#include "dazum.h"
#include "raylib.h"

Camera2D
init_camera(void)
{
	Camera2D camera;

	camera.offset = (Vector2){0, 0};
	camera.target = (Vector2){0, 0};
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	return (camera);
}

Vector2 custom_get_mouse_position()
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
	camera->zoom = 1.0f;
}

void handle_zoom(Camera2D *camera, float *flashlightRadius, bool isFlashlight)
{
	float mouseMovement = GetMouseWheelMove();
	if (IsKeyDown(KEY_LEFT_CONTROL) && isFlashlight)
	{
		if (mouseMovement > 0 && *flashlightRadius > FLASHLIGHT_ZOOM_DELTA)
			*flashlightRadius -= FLASHLIGHT_ZOOM_DELTA;
		else if (mouseMovement < 0)
			*flashlightRadius += FLASHLIGHT_ZOOM_DELTA;
	}
	else
	{
		if (mouseMovement > 0)
			camera->zoom += CAMERA_ZOOM_DELTA;
		else if (mouseMovement < 0 && camera->zoom > CAMERA_ZOOM_DELTA)
			camera->zoom -= CAMERA_ZOOM_DELTA;
	}
}
#ifdef _WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main(void)
#endif
{
	const int width = get_screen_width();
	const int height = get_screen_height();
	const char *filename = TextFormat("%stmp.png", GetApplicationDirectory());
	if (width == 0 || height == 0)
	{
		fprintf(stderr, "An error ocurred getting the width and height\n");
		return (1);
	}
	if (take_screenshot(filename, width, height) != 0)
	{
		fprintf(stderr, "An error ocurred while taking the screenshot\n");
		return (1);
	}
	Color backgroundColor = GetColor(0x101010FF);
	Vector2 originalMousePosition = custom_get_mouse_position();
	SetConfigFlags(FLAG_FULLSCREEN_MODE);
	InitWindow(width, height, "dazum");
	SetMousePosition(originalMousePosition.x, originalMousePosition.y);
	SetTargetFPS(60);
	SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
	Camera2D camera = init_camera();
	Texture texture = LoadTexture(filename);
	Vector2 pin = camera.target;
	bool isDragging = false;
	bool isFlashlight = false;
	bool showCursor = true;
	RenderTexture target = LoadRenderTexture(width, height);
	const char *fsCode = "#version 330 core\n"
						 "uniform sampler2D noiseTexture;\n"
						 "uniform vec2 screenSize;\n"
						 "uniform vec2 flashlightCenter;\n"
						 "uniform float flashlightRadius;\n"
						 "out vec4 fragColor;\n"
						 "void main() {\n"
						 "    vec2 uv = gl_FragCoord.xy / screenSize;\n"
						 "    float distance = length(gl_FragCoord.xy - flashlightCenter);\n"
						 "    if (distance < flashlightRadius) fragColor = texture(noiseTexture, uv);\n"
						 "    else fragColor = vec4(0.0, 0.0, 0.0, 0.8);\n"
						 "}";
	Shader flashlightShader = LoadShaderFromMemory(NULL, fsCode);
	Vector2 flashlightCenter = (Vector2){originalMousePosition.x, (float)height - originalMousePosition.y};
	Vector2 flashlightRadius = (Vector2){FLASHLIGHT_ZOOM, FLASHLIGHT_ZOOM};
	int circleCenterLoc = GetShaderLocation(flashlightShader, "flashlightCenter");
	int circleRadiusLoc = GetShaderLocation(flashlightShader, "flashlightRadius");
	int screenSizeLoc = GetShaderLocation(flashlightShader, "screenSize");
	while (!WindowShouldClose() && !IsKeyPressed(KEY_Q))
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_Z))
		{
			pin = GetScreenToWorld2D(custom_get_mouse_position(), camera);
			isDragging = true;
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			isDragging = false;
		if (isDragging)
		{
			Vector2 currentMousePosition = custom_get_mouse_position();
			camera.target.x = pin.x - (currentMousePosition.x) / camera.zoom;
			camera.target.y = pin.y - (currentMousePosition.y) / camera.zoom;
		}
		if (IsKeyPressed(KEY_ZERO))
			reset_camera(&camera);
		else if (IsKeyPressed(KEY_F))
			isFlashlight = !isFlashlight;
		else if (IsKeyPressed(KEY_C))
			showCursor = !showCursor;
		else
			handle_zoom(&camera, &flashlightRadius.x, isFlashlight);
		if (showCursor)
			ShowCursor();
		else
			HideCursor();
		BeginDrawing();
		BeginMode2D(camera);
		ClearBackground(backgroundColor);
		DrawTexture(texture, 0, 0, WHITE);
		if (isFlashlight)
		{
			SetShaderValue(flashlightShader, circleCenterLoc, &flashlightCenter, SHADER_UNIFORM_VEC2);
			SetShaderValue(flashlightShader, circleRadiusLoc, &flashlightRadius, SHADER_UNIFORM_FLOAT);
			SetShaderValue(flashlightShader, screenSizeLoc, &(Vector2){width, height}, SHADER_UNIFORM_VEC2);
			BeginShaderMode(flashlightShader);
			DrawTexture(target.texture, 0, 0, WHITE);
			EndShaderMode();
			Vector2 currentMousePosition = custom_get_mouse_position();
			flashlightCenter = (Vector2){currentMousePosition.x, (float)height - currentMousePosition.y};
			flashlightRadius = (Vector2){flashlightRadius.x, flashlightRadius.y};
		}
		EndMode2D();
		EndDrawing();
	}
	UnloadRenderTexture(target);
	UnloadShader(flashlightShader);
	UnloadTexture(texture);
	remove(filename);
	CloseWindow();
	return (0);
}
