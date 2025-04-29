#pragma once
#include <windows.h>
#include <windowsx.h>

// Function declarations
void generateRandomMap();
void drawGame(HDC hdc);
void handleInput();
void updateMouseHover();
void handleMouseClick();
void SummonMobs();
void ToggleBackpackWindow(HWND parentHwnd, HINSTANCE hInstance);
bool isPathClear(float startX, float startY, float endX, float endY);
