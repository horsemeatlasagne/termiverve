#include <cmath>
#include <string>
#include <raylib.h>
using namespace std;
const int screenWidth = 1600, screenHeight = 1200; // Screen resolution
float dis(float x, float y)						   // Distance
{
	x = x - screenWidth / 2, y = y - screenHeight / 2;
	float len = sqrt(x * x + y * y);
	return len;
}
int main()
{
	InitWindow(screenWidth, screenHeight, "Mlocc.io Games");			// Initialize window
	SetTargetFPS(60);													// Set frame rate
	float PlayerRadius = 5.0f;											// Player radius
	float MapRadius = 1000.0f;											// Map radius
	Vector2 playerPosition = {screenWidth / 2.0f, screenHeight / 2.0f}; // Player coordinates
	Camera2D camera = {0};												// Camera
	camera.target = playerPosition;										// Camera follows player
	camera.offset = {screenWidth / 2.0f, screenHeight / 2.0f};			// Fix the player as the camera center
	camera.rotation = 0.0f;												// Initial rotation angle is 0
	camera.zoom = 10.0f;												// Initial zoom ratio
	float Speed = 5.0f;													// Player movement speed
	// Main game loop
	while (!WindowShouldClose())
	{
		Vector2 mousePosition = GetMousePosition(); // Mouse coordinates
		// Absolute mouse coordinates
		Vector2 transformedMousePosition =
			{
				(mousePosition.x - camera.offset.x) / camera.zoom + camera.target.x,
				(mousePosition.y - camera.offset.y) / camera.zoom + camera.target.y};
		// Absolute mouse direction
		Vector2 direction = {transformedMousePosition.x - playerPosition.x, transformedMousePosition.y - playerPosition.y};
		// Absolute mouse distance
		float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
		// Player movement
		if (length > 0)
		{
			float len = dis(playerPosition.x, playerPosition.y);
			if (len > MapRadius - 10)
			{
				float ppo = (MapRadius - 10) / len;
				playerPosition.x = playerPosition.x * ppo, playerPosition.y = playerPosition.y * ppo;
			}
			else
			{
				direction.x /= length;
				direction.y /= length;
				playerPosition.x += direction.x * Speed;
				playerPosition.y += direction.y * Speed;
			}
		}
		// Zoom (the viewing distance will be changed according to the tentacle petals later)
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			float wheelMove = GetMouseWheelMove();
			if (wheelMove != 0)
			{
				camera.zoom += wheelMove * 0.01f;
				if (camera.zoom < 0.1f)
					camera.zoom = 0.1f;
				if (camera.zoom > 2.0f)
					camera.zoom = 2.0f;
			}
		}
		camera.target = playerPosition;
		BeginDrawing();
		// —————————————————————————————— Start drawing ——————————————————————————————
		ClearBackground(BLACK);																  // Background
		BeginMode2D(camera);																  // Open camera
		DrawCircle(screenWidth / 2, screenHeight / 2, MapRadius, ColorAlpha(DARKGRAY, 0.7f)); // Map
		Color pointerColor = GRAY;															  // Pointer color
		float lineThickness = PlayerRadius * 0.75f;											  // Pointer thickness
		DrawLineEx(playerPosition, transformedMousePosition, lineThickness, pointerColor);	  // Draw pointer
		DrawCircleV(playerPosition, PlayerRadius, YELLOW);									  // Player drawing
		EndMode2D();																		  // End camera
		DrawCircle(screenWidth - 200, 200, 160, ColorAlpha(BLACK, 0.75f));					  // Map border
		DrawCircle(screenWidth - 200, 200, 150, ColorAlpha(GRAY, 0.75f));					  // Map
		DrawText("Maps", screenWidth - 240, 365, 30, WHITE);								  // Map text
		string Position_View = to_string((int)playerPosition.x / 10) + "," + to_string((int)playerPosition.y / 10);
		int Position_View_Len = Position_View.size();
		DrawText(Position_View.c_str(), screenWidth - 235 - (Position_View_Len - 5) * 5, 400, 25, BLUE); // Map text
		Vector2 miniPlayerPosition = {(playerPosition.x - screenWidth / 2) / MapRadius * 150, (playerPosition.y - screenHeight / 2) / MapRadius * 150};
		DrawCircle(screenWidth - 200 + miniPlayerPosition.x, 200 + miniPlayerPosition.y, 10, ColorAlpha(YELLOW, 0.75f)); // Player mini-map drawing
		// Current frame rate
		int fps = GetFPS();
		DrawText(TextFormat("FPS: %d", fps), 10, 10, 20, WHITE);
		// Current view
		DrawText(TextFormat("Zoom: %.2f", camera.zoom), 10, 40, 20, WHITE);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
