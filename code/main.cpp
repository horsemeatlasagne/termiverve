#include "constants.h"
#include "globals.h"
#include "inputhandling.h"
#include <cstdio>
#include <ctime>
#include <map>

// #include "inventory.h"
#include "gamelogic.h"
#include "inputhandling.h"
#include "inventory.h"
#include "main.h"
#include <raylib.h>

// ============================================================
// BACKPACK UI - DEFERRED TO FUTURE MILESTONE
// Original Win32 implementation preserved for reference below
// Keybind: 'B' (currently does nothing)
// TODO: Rewrite using Raylib drawing primitives when needed
// ============================================================

// TODO: Rewrite Backpack Window Procedure
// Backpack window procedure
// LRESULT CALLBACK BackpackWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// {
//     // TODO: Rewrite logic
//     switch (uMsg)
//     {
//     case WM_PAINT:
//     {
//         PAINTSTRUCT ps;
//         HDC hdc = BeginPaint(hwnd, &ps);

//         // Set up colors and fonts
//         SetBkColor(hdc, RGB(50, 50, 50));      // Dark background
//         SetTextColor(hdc, RGB(255, 255, 255)); // White text

//         HFONT hFont = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
//                                   OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
//                                   DEFAULT_PITCH | FF_DONTCARE, L"Arial");
//         SelectObject(hdc, hFont);

//         // Draw background
//         RECT clientRect;
//         GetClientRect(hwnd, &clientRect);
//         HBRUSH bgBrush = CreateSolidBrush(RGB(50, 50, 50));
//         FillRect(hdc, &clientRect, bgBrush);
//         DeleteObject(bgBrush);

//         // Draw title
//         const char *title = "BACKPACK";
//         TextOutA(hdc, 80, 10, title, strlen(title));

//         // Draw separator line
//         HPEN linePen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
//         SelectObject(hdc, linePen);
//         MoveToEx(hdc, 10, 35, NULL);
//         LineTo(hdc, 290, 35);
//         DeleteObject(linePen);

//         // Get item counts (only from bag/backpack)
//         std::map<std::string, int> itemCounts;
//         CountBagItems(itemCounts);

//         // Draw table headers
//         TextOutA(hdc, 30, 45, "ITEM", 4);
//         TextOutA(hdc, 200, 45, "COUNT", 5);

//         // Draw another separator
//         linePen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
//         SelectObject(hdc, linePen);
//         MoveToEx(hdc, 10, 65, NULL);
//         LineTo(hdc, 290, 65);
//         DeleteObject(linePen);

//         // Draw items and counts
//         int y = 80;
//         for (const auto &item : itemCounts)
//         {
//             if (item.first != "Empty") // Don't show empty items
//             {
//                 TextOutA(hdc, 30, y, item.first.c_str(), item.first.length());

//                 // Convert count to string
//                 char countStr[10];
//                 sprintf(countStr, "%d", item.second);
//                 TextOutA(hdc, 200, y, countStr, strlen(countStr));

//                 y += 25; // Move down for next item
//             }
//         }

//         // Draw close instruction
//         const char *closeText = "Press 'B' to close";
//         TextOutA(hdc, 90, 350, closeText, strlen(closeText));

//         DeleteObject(hFont);
//         EndPaint(hwnd, &ps);
//         return 0;
//     }

//     case WM_KEYDOWN:
//         if (wParam == 'B')
//         {
//             // Don't close immediately on B press - let the main loop handle it
//             // This prevents the window from closing and immediately reopening
//             return 0;
//         }
//         return 0;

//     case WM_CLOSE:
//         DestroyWindow(hwnd);
//         backpackWindow = NULL;
//         isBackpackOpen = false;
//         return 0;
//     }

//     return DefWindowProc(hwnd, uMsg, wParam, lParam);
// }
// // * END todo

// // Function to create and show backpack window
// void ToggleBackpackWindow(HWND parentHwnd, HINSTANCE hInstance)
// {
//     // Add a time-based cooldown to prevent rapid toggling
//     DWORD currentTime = GetTickCount();
//     if (currentTime - lastBackpackToggleTime < 300) // 300ms cooldown
//     {
//         return; // Ignore toggle requests that come too quickly
//     }

//     lastBackpackToggleTime = currentTime;

//     if (isBackpackOpen && backpackWindow != NULL)
//     {
//         // Close the window if it's already open
//         DestroyWindow(backpackWindow);
//         backpackWindow = NULL;
//         isBackpackOpen = false;
//         return;
//     }

//     // Create backpack window if it doesn't exist
//     if (backpackWindow == NULL)
//     {
//         // Register window class for backpack
//         WNDCLASSA backpackClass = {};
//         backpackClass.lpfnWndProc = BackpackWindowProc;
//         backpackClass.hInstance = hInstance;
//         backpackClass.lpszClassName = "BackpackWindow";
//         RegisterClassA(&backpackClass);

//         // Get parent window position
//         RECT parentRect;
//         GetWindowRect(parentHwnd, &parentRect);

//         // Create the backpack window
//         backpackWindow = CreateWindowExA(
//             WS_EX_TOPMOST | WS_EX_TOOLWINDOW, // Make it stay on top without taskbar icon
//             "BackpackWindow", "Backpack",
//             WS_POPUP | WS_VISIBLE, // Popup style without borders
//             parentRect.left + 50,  // Position it near the parent window
//             parentRect.top + 50,
//             300,        // Width
//             400,        // Height
//             parentHwnd, // Parent window
//             NULL, hInstance, NULL);

//         if (backpackWindow != NULL)
//         {
//             ShowWindow(backpackWindow, SW_SHOW);
//             isBackpackOpen = true;
//         }
//     }
// }

inline Vector2 calculateWorldPos(Vector2 gridPos)
{
    return {gridPos.x * GRID_SIZE, gridPos.y * GRID_SIZE};
}
inline float calculateWorldPos(float gridPos)
{
    return gridPos * GRID_SIZE;
}

// Check if path between start and target is clear of obstacles
bool isPathClear(float startX, float startY, float endX, float endY)
{
    int steps = std::max(abs(endX - startX), abs(endY - startY));
    for (int i = 0; i < steps; ++i)
    {
        float x = startX + (endX - startX) * (i / (float)steps);
        float y = startY + (endY - startY) * (i / (float)steps);
        if (gameMap[(int)y][(int)x].type == OBSTACLE)
        {
            return false; // If path contains obstacle, return false
        }
    }
    return true;
}

// Generate random obstacles, trees and workbenches
void generateRandomMap()
{
    srand(static_cast<unsigned>(time(0)));
    // Generate random obstacles
    for (int i = 0; i < 300; ++i)
    {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        gameMap[y][x] = {OBSTACLE, OBSTACLE_HEALTH};
    }
    // Generate random trees
    for (int i = 0; i < 100; ++i)
    {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        gameMap[y][x] = {TREE, TREE_HEALTH};
    }
    // Generate random workbenches
    for (int i = 0; i < 50; ++i)
    {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        gameMap[y][x] = {WORKBENCH, WORKBENCH_HEALTH};
    }
    gameMap[playerPos.y][playerPos.x] = {GROUND, 0};
}

// Draw the game
void drawGame()
{
    // Draw the map
    for (int y = 0; y < MAP_HEIGHT; ++y)
    { //-10~9
        for (int x = 0; x < MAP_WIDTH; ++x)
        { //-7~8
            if (gameMap[y][x].health <= 0 && gameMap[y][x].type != GROUND)
                gameMap[y][x].type = GROUND;
            GameEntity CurrentSelectedBlock = gameMap[y][x];
            Color clr;
            if (CurrentSelectedBlock.type == GROUND)
            {
                // printf("Game map: x:%d y:%d type:Ground\n", x, y);
                clr = GRAY; // Gray ground
            }
            else if (CurrentSelectedBlock.type == OBSTACLE)
            {
                // printf("Game map: x:%d y:%d type:Obstacle\n", x, y);
                clr = DARKGRAY; // Dark gray obstacles
            }
            else if (CurrentSelectedBlock.type == TREE)
            {
                // printf("Game map: x:%d y:%d type:Tree\n", x, y);
                clr = GREEN; // Green trees
            }
            else if (CurrentSelectedBlock.type == WORKBENCH)
            {
                // printf("Game map: x:%d y:%d type:Workbench\n", x, y);
                clr = BROWN; // Brown workbenches
            }
            Rectangle CurrentBlock = {calculateWorldPos(x), calculateWorldPos(y), GRID_SIZE,
                                      GRID_SIZE};
            DrawRectangleRec(CurrentBlock, clr); // Draw the map entity
            DrawRectangleLinesEx(CurrentBlock, 1, BLACK);
        }
    }

    // Draw the player
    Rectangle playerRect = {calculateWorldPos(playerPos.x), calculateWorldPos(playerPos.y),
                            GRID_SIZE, GRID_SIZE};
    DrawRectangleRec(playerRect, RED); // Draw as square
    // printf("Player X: %f, Player Y: %f\n", calculateWorldPos(playerPos).x,
    //        calculateWorldPos(playerPos).y);
    // Draw mobs
    for (int i = 0; i < allmobs.size(); i++)
    {
        Color clr = BLUE; // Mobs are blue
        Rectangle mob = {calculateWorldPos(allmobs[i].x), calculateWorldPos(allmobs[i].y),
                         GRID_SIZE, GRID_SIZE};
        DrawRectangleRec(mob, clr);
    }

    // // Display card
    // // ClearBackground({0, 0, 0});

    // Construct item names string
    std::vector<std::string> itemNames = {DropsName[LeftHand],    DropsName[RightHand],
                                          DropsName[onPlayer[1]], DropsName[onPlayer[2]],
                                          DropsName[onPlayer[3]], DropsName[Bar[1]],
                                          DropsName[Bar[2]],      DropsName[Bar[3]],
                                          DropsName[Bar[4]],      DropsName[Bar[5]]};
    // Prepare strings with consistent 4-space separation
    std::string itemNamesStr, modelStr;
    for (size_t i = 0; i < itemNames.size(); ++i)
    {
        size_t centerOffset = itemNames[i].size() / 2;
        for (size_t j = 1; j <= centerOffset; ++j)
        {
            modelStr += " ";
        }
        modelStr += ModelName[i];

        itemNamesStr += itemNames[i];
        if (i < itemNames.size() - 1)
        {
            itemNamesStr += "    "; // 4 spaces between items
            size_t currentHalf = itemNames[i].size() / 2, nextHalf = itemNames[i + 1].size() / 2,
                   totalSpaces = currentHalf + nextHalf + 4;
            for (size_t j = 1; j <= totalSpaces; ++j)
            {
                modelStr += " ";
            }
        }
    }
    // Construct model labels to match item names
    // std::string modelStr = "L    R    F1   F2   F3   1    2    3    4    5";
    // printf("%d %d\n", itemNamesStr.size(), modelStr.size());
    const char *text = itemNamesStr.c_str();
    const char *model = modelStr.c_str();

    // Calculate text widths to center both
    int textSizeWidth, modelTextSizeWidth; // Replaced GDI SIZE with int for raylib MeasureText
    textSizeWidth = MeasureText(text, 20);
    modelTextSizeWidth = MeasureText(model, 20);

    // Position text at bottom center
    int centerX = WINDOW_WIDTH / 2;
    // Convert screen coordinates to world coordinates
    Vector2 screenPosText = {(float)(centerX - textSizeWidth / 2), (float)(WINDOW_HEIGHT - 80)};
    Vector2 screenPosModel = {(float)(centerX - modelTextSizeWidth / 2),
                              (float)(WINDOW_HEIGHT - 60)};

    // Convert to world coordinates
    Vector2 worldPosText = GetScreenToWorld2D(screenPosText, camera);
    Vector2 worldPosModel = GetScreenToWorld2D(screenPosModel, camera);

    // Draw at world coordinates (but will appear at fixed screen positions)
    DrawText(text, worldPosText.x, worldPosText.y, 20, BLACK);
    DrawText(model, worldPosModel.x, worldPosModel.y, 20, BLACK);

    // For pause text as well
    if (isPaused)
    {
        Vector2 screenPosPause = {(float)(centerX - 50), (float)(WINDOW_HEIGHT / 2)};
        Vector2 worldPosPause = GetScreenToWorld2D(screenPosPause, camera);
        DrawText("GAME PAUSED", worldPosPause.x, worldPosPause.y, 20, RED);
    }
}

// // Window procedure function
// LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// {

//     switch (uMsg)
//     {
//         // for (int i = 0; i <= MAP_HEIGHT; i += 1.0 / 32.0)
//         // {
//         //     for (int j = 0; j <= MAP_WIDTH; j += 1.0 / 32.0)
//         //     {
//         //         printf("i: %d j: %d\n", i, j);
//         //         if (isDestroyed(j, i))
//         //         {
//         //             Drop(j, i);
//         //             gameMap[i][j].type = GROUND;
//         //         }
//         //     }
//         // }
//     case WM_MOUSEMOVE:
//         // Track mouse movement for hover functionality
//         lastMousePos.x = GET_X_LPARAM(lParam);
//         lastMousePos.y = GET_Y_LPARAM(lParam);
//         return 0;

//     case WM_PAINT:
//     {
//         PAINTSTRUCT ps;
//         HDC hdc = BeginPaint(hwnd, &ps);
//         drawGame(hdc); // Draw the game
//         EndPaint(hwnd, &ps);
//         return 0;
//     }
//     case WM_CLOSE:
//         PostQuitMessage(0);
//         return 0;
//     }
//     return DefWindowProc(hwnd, uMsg, wParam, lParam);
// }

// Main entry point
int main()
{
    // freopen("tmvlog.txt", "w", stdout);

    // DEBUG: Log initial positions
    printf("DEBUG INIT: Initial playerX=%.2f, playerY=%.2f\n", playerPos.x, playerPos.y);

    camera.offset = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    mp[OBSTACLE] = "Stone";
    mp[TREE] = "Wood";
    mp[WORKBENCH] = "Bench";
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Termiverve v3");
    SetTargetFPS(60);
    generateRandomMap();
    SetExitKey(KEY_ESCAPE);
    while (!WindowShouldClose())
    {
        // Update player world position every frame
        camera.target = calculateWorldPos(playerPos);
        // Drawing logic begins here
        BeginDrawing();
        ClearBackground(GRAY);
        BeginMode2D(camera);
        // Only run game logic if not paused
        if (!isPaused)
        {
            // Update mouse hover information
            updateMouseHover();

            // Summon mobs
            SummonMobs();
            // Handle input and game logic
            handleInput();
            // Handle mouse
            handleMouseClick();
            // Handle mob deaths
            MobDeath();
            drawGame();
            // handle zoom
            handleZoom();
        }
        EndMode2D();
        // may add redraw logic
        // Sleep to control frame rate
        // Sleep(20);
        EndDrawing();
        // Drawing logic ends here
    }
    CloseWindow(); // Close window and unload OpenGL context
    return 0;
}
