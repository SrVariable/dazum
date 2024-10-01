#include "dazum.h"

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main(void)
#endif // _WIN32
{
	const int width = get_screen_width();
	const int height = get_screen_height();
	if (width == 0 || height == 0) return (1);
	const char *filename = TextFormat("%stmp.png", GetApplicationDirectory());
	if (take_screenshot(filename, width, height) != 0) return (1);
	Color backgroundColor = GetColor(0x101010FF);
	Vector2 originalMousePosition = custom_get_mouse_position();
	SetConfigFlags(FLAG_FULLSCREEN_MODE);
	InitWindow(width, height, "dazum");
	SetMousePosition(originalMousePosition.x, originalMousePosition.y);
	SetTargetFPS(60);
	SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
	Camera2D camera = init_camera();
	Texture texture = LoadTexture(filename);
	bool isFlashlightMode = false;
	bool showCursor = true;
	RenderTexture target = LoadRenderTexture(width, height);
	Shader flashlightShader = LoadShaderFromMemory(NULL, FLASHLIGHT_SHADER_CODE);
	Vector2 flashlightCenter = (Vector2){originalMousePosition.x, (float)height - originalMousePosition.y};
	Vector2 flashlightRadius = (Vector2){FLASHLIGHT_ZOOM, FLASHLIGHT_ZOOM};
	int circleCenterLoc = GetShaderLocation(flashlightShader, "flashlightCenter");
	int circleRadiusLoc = GetShaderLocation(flashlightShader, "flashlightRadius");
	int screenSizeLoc = GetShaderLocation(flashlightShader, "screenSize");
	while (!WindowShouldClose() && !IsKeyPressed(KEY_Q))
	{
		if (showCursor) ShowCursor();
		else HideCursor();
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) drag_position(&camera);
		if (IsKeyPressed(KEY_ZERO)) reset_camera(&camera);
		else if (IsKeyPressed(KEY_F)) isFlashlightMode = !isFlashlightMode;
		else if (IsKeyPressed(KEY_C)) showCursor = !showCursor;
		else handle_zoom(&camera, &flashlightRadius.x, isFlashlightMode);
		BeginDrawing();
		BeginMode2D(camera);
		ClearBackground(backgroundColor);
		DrawTexture(texture, 0, 0, WHITE);
		if (isFlashlightMode)
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
