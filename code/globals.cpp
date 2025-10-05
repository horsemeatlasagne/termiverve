#include "globals.h"
#include "constants.h"
#include <array>
#include <map>
#include <string>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
// Globals

std::string DropsName[] = {"Empty", "Wood", "Stone", "Leaf", "Bench", "Stick"};
std::string ModelName[] = {"L", "R", "F1", "F2", "F3", "1", "2", "3", "4", "5"};

std::vector<mobs> allmobs;
// Map state
std::vector<std::vector<GameEntity>> gameMap(MAP_HEIGHT,
                                             std::vector<GameEntity>(MAP_WIDTH, {GROUND, 0}));
std::array<std::array<Rectangle, MAP_WIDTH>, MAP_HEIGHT> gameMapWorldPos;
std::array<std::array<Color, MAP_WIDTH>, MAP_HEIGHT> gameMapColor;

Vector2 playerPos = {MAP_WIDTH / 2.0f, MAP_HEIGHT / 2.0f};
float playerVelocityX = 0.0f; // Player horizontal velocity
float playerVelocityY = 0.0f; // Player vertical velocity
GameDrops *SelectedDrop;
bool HaveSelected = false;
int lastAttackTime;

// Name mapping
std::map<GameObject, std::string> mp;

// Add global pause state variable
bool isPaused = false;

// Add global variables for mouse hover functionality
Vector2 lastMousePos = {-1, -1}; // Last mouse position
int lastMouseGridX = -1;         // Last grid X coordinate under mouse
int lastMouseGridY = -1;         // Last grid Y coordinate under mouse
long hoverStartTime = 0;         // Time when mouse started hovering
bool showHoverInfo = false;      // Whether to show hover information

// Add global variables for backpack window
bool isBackpackOpen = false;

// Add global variable to track last backpack toggle time
long lastBackpackToggleTime = 0;

// Forward declaration of backpack window procedure
// LRESULT CALLBACK BackpackWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// Initialize game items
GameDrops LeftHand = EMPTY;
GameDrops RightHand = EMPTY;
GameDrops Bar[12] = {EMPTY};
GameDrops onPlayer[5] = {EMPTY};
std::vector<std::string> bag;
