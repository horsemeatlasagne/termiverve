#pragma once
#include <raylib.h>
// Function declarations
void generateRandomMap();
void drawGame();
void handleInput();
void updateMouseHover();
void SummonMobs();
// void ToggleBackpackWindow(HWND parentHwnd, HINSTANCE hInstance);
bool isPathClear(float startX, float startY, float endX, float endY);