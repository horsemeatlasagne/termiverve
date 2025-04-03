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

// Game object types
enum GameObject
{
    GROUND,
    OBSTACLE,
    TREE,
    WORKBENCH,
    PLAYER
};

// Item types
enum GameDrops
{
    EMPTY,
    WOOD,
    STONE,
    LEAF,
    WORK,
    STICK
};

// Game entity structure
struct GameEntity;

struct mobs;

// Global variables for player state
extern float playerX, playerY;
extern float playerVelocityX, playerVelocityY;

// Additional player-related globals
extern GameDrops& SelectedDrop;
extern bool HaveSelected;
extern int lastAttackTime;

// Global variables for game state
extern bool isPaused;
extern POINT lastMousePos;
extern int lastMouseGridX;
extern int lastMouseGridY;
extern DWORD hoverStartTime;
extern bool showHoverInfo;

// Backpack and inventory related globals
extern HWND backpackWindow;
extern bool isBackpackOpen;
extern DWORD lastBackpackToggleTime;


// Global game map and mob management
extern std::vector<std::vector<GameEntity>> gameMap;
extern std::vector<mobs> allmobs;

// Global string arrays
extern std::string DropsName[];
extern std::string ModelName[];

// Global map for object type names
extern std::map<GameObject, std::string> mp;

// Function declarations
void generateRandomMap();
void drawGame(HDC hdc);
void handleInput();
void updateMouseHover();
void handleMouseClick();
void SummonMobs();
void ToggleBackpackWindow(HWND parentHwnd, HINSTANCE hInstance);
bool isPathClear(float startX, float startY, float endX, float endY);
void attackTarget(int targetX, int targetY);
void attackMobs(float targetX, float targetY);
bool CheckPos(float newX, float newY);
void MobDeath();
bool isDestroyed(float x, float y);
void Drop(float x, float y);
