#pragma once
#include <vector>
#include <string>
#include <map>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
// #include <chrono>
#include <windowsx.h>
#include <algorithm>
#include <cstdio>

// Function declarations
void generateRandomMap();
void drawGame(HDC hdc);
void handleInput();
void updateMouseHover();
void handleMouseClick();
void SummonMobs();
void ToggleBackpackWindow(HWND parentHwnd, HINSTANCE hInstance);
bool isPathClear(float startX, float startY, float endX, float endY);
