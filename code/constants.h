#pragma once
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1 : 0) // For keyboard detection

// Game constants
const int GRID_SIZE = 32;           // Size of each grid cell (32 pixels)
const int MAP_WIDTH = 70;           // Map width (in grid cells)
const int MAP_HEIGHT = 70;          // Map height (in grid cells)
const int WINDOW_WIDTH = 800;       // Window width
const int WINDOW_HEIGHT = 800;      // Window height
const float PLAYER_SPEED = 0.2f;    // Player movement speed
const float ATTACK_INTERVAL = 0.2f; // Attack interval 0.2 seconds
const int MAX_ATTACK_DISTANCE = 3;  // Maximum attack distance is 3 grid cells
const int TREE_HEALTH = 10;
const int WORKBENCH_HEALTH = 7;
const int ATTACK_DAMAGE = 2, attackPower = 1, stickAttackPower = 5;
const int CARD_WORD_Y1 = WINDOW_HEIGHT - 80;
const int CARD_WORD_Y2 = WINDOW_HEIGHT - 40;
const int CARD_WORD_X1 = 0;
const int CARD_WORD_X2 = WINDOW_WIDTH - 10;
const int OBSTACLE_HEALTH = 7;
