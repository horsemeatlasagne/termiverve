#pragma once
#include <string>
#include <vector>
#include <map>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <windowsx.h>

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
struct GameEntity
{
    GameObject type;
    int health;
};
struct mobs
{
    double x, y;
    double speed;
    int damage, blood;
    std::string name;
};

// Global variables for player state
extern float playerX, playerY;
extern float playerVelocityX, playerVelocityY;

// Additional player-related globals
extern GameDrops *SelectedDrop;
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

extern GameDrops LeftHand;
extern GameDrops RightHand;
extern GameDrops Bar[12];
extern GameDrops onPlayer[5];
extern std::vector<std::string> bag;