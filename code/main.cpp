#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "constants.h"
#include "main.h"
#include "inputhandling.h"
#include "inventory.h"
#include "gamelogic.h"



std::string DropsName[] = { "Empty", "Wood", "Stone", "Leaf", "Bench", "Stick" };
std::string ModelName[] = { "L", "R", "F1", "F2", "F3", "1", "2", "3", "4", "5" };


std::vector<mobs> allmobs;
// Map state
std::vector<std::vector<GameEntity>> gameMap(MAP_HEIGHT, std::vector<GameEntity>(MAP_WIDTH, { GROUND, 0 }));

float playerX = MAP_WIDTH / 2.0f;  // Player X position (in grid cells)
float playerY = MAP_HEIGHT / 2.0f; // Player Y position (in grid cells)
float playerVelocityX = 0.0f;      // Player horizontal velocity
float playerVelocityY = 0.0f;      // Player vertical velocity
GameDrops *SelectedDrop;
bool HaveSelected = false;
int lastAttackTime;

// Name mapping
std::map<GameObject, std::string> mp;

// Add global pause state variable
bool isPaused = false;

// Add global variables for mouse hover functionality
POINT lastMousePos = { -1, -1 }; // Last mouse position
int lastMouseGridX = -1;       // Last grid X coordinate under mouse
int lastMouseGridY = -1;       // Last grid Y coordinate under mouse
DWORD hoverStartTime = 0;      // Time when mouse started hovering
bool showHoverInfo = false;    // Whether to show hover information

// Add global variables for backpack window
HWND backpackWindow = NULL;
bool isBackpackOpen = false;

// Add global variable to track last backpack toggle time
DWORD lastBackpackToggleTime = 0;

// Forward declaration of backpack window procedure
// LRESULT CALLBACK BackpackWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Backpack window procedure
LRESULT CALLBACK BackpackWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Set up colors and fonts
        SetBkColor(hdc, RGB(50, 50, 50));      // Dark background
        SetTextColor(hdc, RGB(255, 255, 255)); // White text

        HFONT hFont = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        SelectObject(hdc, hFont);

        // Draw background
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        HBRUSH bgBrush = CreateSolidBrush(RGB(50, 50, 50));
        FillRect(hdc, &clientRect, bgBrush);
        DeleteObject(bgBrush);

        // Draw title
        const char *title = "BACKPACK";
        TextOutA(hdc, 110, 10, title, strlen(title));

        // Draw separator line
        HPEN linePen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
        SelectObject(hdc, linePen);
        MoveToEx(hdc, 10, 35, NULL);
        LineTo(hdc, 290, 35);
        DeleteObject(linePen);

        // Get item counts
        std::map<std::string, int> itemCounts = CountBagItems();

        // Draw table headers
        TextOutA(hdc, 30, 45, "ITEM", 4);
        TextOutA(hdc, 200, 45, "COUNT", 5);

        // Draw another separator
        linePen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
        SelectObject(hdc, linePen);
        MoveToEx(hdc, 10, 65, NULL);
        LineTo(hdc, 290, 65);
        DeleteObject(linePen);

        // Draw items and counts
        int y = 80;
        for (const auto &item : itemCounts)
        {
            if (item.first != "Empty") // Don't show empty items
            {
                TextOutA(hdc, 30, y, item.first.c_str(), item.first.length());

                // Convert count to string
                char countStr[10];
                sprintf(countStr, "%d", item.second);
                TextOutA(hdc, 200, y, countStr, strlen(countStr));

                y += 25; // Move down for next item
            }
        }

        // Draw close instruction
        const char *closeText = "Press 'B' to close";
        TextOutA(hdc, 90, 350, closeText, strlen(closeText));

        DeleteObject(hFont);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_KEYDOWN:
        if (wParam == 'B')
        {
            // Don't close immediately on B press - let the main loop handle it
            // This prevents the window from closing and immediately reopening
            return 0;
        }
        return 0;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        backpackWindow = NULL;
        isBackpackOpen = false;
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Function to create and show backpack window
void ToggleBackpackWindow(HWND parentHwnd, HINSTANCE hInstance)
{
    // Add a time-based cooldown to prevent rapid toggling
    DWORD currentTime = GetTickCount();
    if (currentTime - lastBackpackToggleTime < 300) // 300ms cooldown
    {
        return; // Ignore toggle requests that come too quickly
    }

    lastBackpackToggleTime = currentTime;

    if (isBackpackOpen && backpackWindow != NULL)
    {
        // Close the window if it's already open
        DestroyWindow(backpackWindow);
        backpackWindow = NULL;
        isBackpackOpen = false;
        return;
    }

    // Create backpack window if it doesn't exist
    if (backpackWindow == NULL)
    {
        // Register window class for backpack
        WNDCLASSA backpackClass = {};
        backpackClass.lpfnWndProc = BackpackWindowProc;
        backpackClass.hInstance = hInstance;
        backpackClass.lpszClassName = "BackpackWindow";
        RegisterClassA(&backpackClass);

        // Get parent window position
        RECT parentRect;
        GetWindowRect(parentHwnd, &parentRect);

        // Create the backpack window
        backpackWindow = CreateWindowExA(
            WS_EX_TOPMOST | WS_EX_TOOLWINDOW, // Make it stay on top without taskbar icon
            "BackpackWindow",
            "Backpack",
            WS_POPUP | WS_VISIBLE, // Popup style without borders
            parentRect.left + 50,  // Position it near the parent window
            parentRect.top + 50,
            300,        // Width
            400,        // Height
            parentHwnd, // Parent window
            NULL,
            hInstance,
            NULL);

        if (backpackWindow != NULL)
        {
            ShowWindow(backpackWindow, SW_SHOW);
            isBackpackOpen = true;
        }
    }
}





// Check if path between start and target is clear of obstacles
bool isPathClear(float startX, float startY, float endX, float endY)
{
    int steps = std::max(std::abs(endX - startX), std::abs(endY - startY));
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
        gameMap[y][x] = { OBSTACLE, OBSTACLE_HEALTH };
    }
    // Generate random trees
    for (int i = 0; i < 100; ++i)
    {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        gameMap[y][x] = { TREE, TREE_HEALTH };
    }
    // Generate random workbenches
    for (int i = 0; i < 50; ++i)
    {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        gameMap[y][x] = { WORKBENCH, WORKBENCH_HEALTH };
    }
    gameMap[playerY][playerX] = { GROUND, 0 };
}

// Draw the game
void drawGame(HDC hdc)
{
    // Draw the map
    for (int y = 0; y < MAP_HEIGHT; ++y)
    { //-10~9
        for (int x = 0; x < MAP_WIDTH; ++x)
        { //-7~8
            if (gameMap[y][x].health <= 0 && gameMap[y][x].type != GROUND)
                gameMap[y][x].type = GROUND;
            GameEntity entity = gameMap[y][x];
            HBRUSH brush = NULL;
            if (entity.type == GROUND)
            {
                brush = CreateSolidBrush(RGB(200, 200, 200)); // Gray ground
            }
            else if (entity.type == OBSTACLE)
            {
                brush = CreateSolidBrush(RGB(100, 100, 100)); // Dark gray obstacles
            }
            else if (entity.type == TREE)
            {
                brush = CreateSolidBrush(RGB(0, 255, 0)); // Green trees
            }
            else if (entity.type == WORKBENCH)
            {
                brush = CreateSolidBrush(RGB(139, 69, 19)); // Brown workbenches
            }
            int PaintX1 = x * GRID_SIZE - playerX * GRID_SIZE + 400,
                PrintY1 = y * GRID_SIZE - playerY * GRID_SIZE + 400,
                PrintX2 = (x + 1) * GRID_SIZE - playerX * GRID_SIZE + 400,
                PrintY2 = (y + 1) * GRID_SIZE - playerY * GRID_SIZE + 400;
            SelectObject(hdc, brush);
            Rectangle(hdc, PaintX1, PrintY1, PrintX2, PrintY2);
            DeleteObject(brush);
        }
    }

    // Draw the player
    // int playerPosX = static_cast<int>(round(playerX * GRID_SIZE));  // Player screen X position
    // int playerPosY = static_cast<int>(round(playerY * GRID_SIZE));  // Player screen Y position
    int playerPosX = 400, playerPosY = 400;
    int playerSize = GRID_SIZE;                            // Player size is one grid cell
    HBRUSH playerBrush = CreateSolidBrush(RGB(255, 0, 0)); // Red player
    SelectObject(hdc, playerBrush);
    Rectangle(hdc, playerPosX, playerPosY, playerPosX + playerSize, playerPosY + playerSize); // Draw as square
    DeleteObject(playerBrush);

    // Draw mobs
    for (size_t i = 0; i < allmobs.size(); i++)
    {
        HBRUSH brush;
        brush = CreateSolidBrush(RGB(100, 100, 255)); // Mobs are blue
        int PaintX1 = allmobs[i].x * GRID_SIZE - playerX * GRID_SIZE + 400,
            PrintY1 = allmobs[i].y * GRID_SIZE - playerY * GRID_SIZE + 400,
            PrintX2 = allmobs[i].x * GRID_SIZE + GRID_SIZE - playerX * GRID_SIZE + 400,
            PrintY2 = allmobs[i].y * GRID_SIZE + GRID_SIZE - playerY * GRID_SIZE + 400;
        SelectObject(hdc, brush);
        Rectangle(hdc, PaintX1, PrintY1, PrintX2, PrintY2);
        DeleteObject(brush);
    }

    // Display card
    SetTextColor(hdc, RGB(0, 0, 0)); // Black text
    SetBkMode(hdc, TRANSPARENT);     // Transparent background

    // Construct item names string
    std::vector<std::string> itemNames = {
        DropsName[LeftHand], DropsName[RightHand],
        DropsName[onPlayer[0]], DropsName[onPlayer[1]], DropsName[onPlayer[2]],
        DropsName[Bar[0]], DropsName[Bar[1]], DropsName[Bar[2]], DropsName[Bar[3]], DropsName[Bar[4]] };
    // Prepare strings with consistent 4-space separation
    std::string itemNamesStr, modelStr;
    for (size_t i = 0; i < itemNames.size(); ++i)
    {
        for (size_t j = 1; j <= itemNames[i].size() / 2; ++j)
        {
            modelStr += " ";
        }
        modelStr += ModelName[i];

        itemNamesStr += itemNames[i];
        if (i < itemNames.size() - 1)
        {
            itemNamesStr += "    "; // 4 spaces between items
            for (size_t j = 1; j <= (itemNames[i].size() + itemNames[i + 1].size()) / 2 + 4; ++j)
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
    SIZE textSize, modelTextSize;
    GetTextExtentPoint32A(hdc, text, strlen(text), &textSize);
    GetTextExtentPoint32A(hdc, model, strlen(model), &modelTextSize);

    // Position text at bottom center
    int centerX = WINDOW_WIDTH / 2;
    TextOutA(hdc, centerX - textSize.cx / 2, WINDOW_HEIGHT - 80, text, strlen(text));
    TextOutA(hdc, centerX - modelTextSize.cx / 2, WINDOW_HEIGHT - 60, model, strlen(model));

    // Display pause text if game is paused
    if (isPaused)
    {
        SetTextColor(hdc, RGB(255, 0, 0)); // Red text for pause
        const char *pauseText = "GAME PAUSED";
        TextOutA(hdc, WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2, pauseText, strlen(pauseText));
    }

    // Display hover information if active
    if (showHoverInfo)
    {
        // Create tooltip-style bubble
        char hoverText[50];
        int isGround = (gameMap[lastMouseGridY][lastMouseGridX].type == GROUND) ? 1 : 0;
        sprintf(hoverText, "X:%d, Y:%d, Ground:%d", lastMouseGridX, lastMouseGridY, isGround);

        // Calculate text dimensions
        SIZE textSize;
        GetTextExtentPoint32A(hdc, hoverText, strlen(hoverText), &textSize);

        // Calculate tooltip position (offset from cursor)
        int tooltipX = lastMousePos.x + 15;
        int tooltipY = lastMousePos.y - textSize.cy - 10;

        // Ensure tooltip stays within window bounds
        if (tooltipX + textSize.cx + 10 > WINDOW_WIDTH)
            tooltipX = WINDOW_WIDTH - textSize.cx - 10;
        if (tooltipY < 5)
            tooltipY = lastMousePos.y + 15;

        // Draw tooltip background
        HBRUSH tooltipBrush = CreateSolidBrush(RGB(255, 255, 225));   // Light yellow background
        HPEN tooltipPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128)); // Gray border
        SelectObject(hdc, tooltipBrush);
        SelectObject(hdc, tooltipPen);

        // Draw rounded rectangle for tooltip
        RoundRect(hdc, tooltipX, tooltipY,
            tooltipX + textSize.cx + 10, tooltipY + textSize.cy + 6,
            5, 5);

        // Draw text
        SetTextColor(hdc, RGB(0, 0, 0)); // Black text
        SetBkMode(hdc, TRANSPARENT);     // Transparent background
        TextOutA(hdc, tooltipX + 5, tooltipY + 3, hoverText, strlen(hoverText));

        // Clean up
        DeleteObject(tooltipBrush);
        DeleteObject(tooltipPen);
    }
}

// Window procedure function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // the following code may generate issues: Game window not showing
    // for (float i = 0.0; i <= MAP_HEIGHT; i += 1.00 / 32.00)
    // {
    //     for (float j = 0.0; j <= MAP_WIDTH; j += 1.00 / 32.00)
    //     {
    //         if (isDestroyed(j, i))
    //         {
    //             Drop(j, i);
    //         }
    //     }
    // }
    switch (uMsg)
    {

    case WM_MOUSEMOVE:
        // Track mouse movement for hover functionality
        lastMousePos.x = GET_X_LPARAM(lParam);
        lastMousePos.y = GET_Y_LPARAM(lParam);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        drawGame(hdc); // Draw the game
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Handle mouse click for attacks
void handleMouseClick()
{
    int currentTime = clock();
    if (currentTime - lastAttackTime >= 200)
    { // Attack interval is 0.2 seconds
        // std::cout<<1;
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        ScreenToClient(GetForegroundWindow(), &cursorPos);

        float targetX = (cursorPos.x - 400 + playerX * GRID_SIZE) / GRID_SIZE;
        float targetY = (cursorPos.y - 400 + playerY * GRID_SIZE) / GRID_SIZE;
        // std::cout<< targetX << ' ' << targetY << ' ' << playerX << ' ' << playerY <<"\n";

        if (targetX >= 0 && targetX < MAP_WIDTH && targetY >= 0 && targetY < MAP_HEIGHT &&
            isPathClear(playerX, playerY, (int)targetX, (int)targetY) && KEY_DOWN(MOUSE_MOVED))
        {
            attackTarget((int)targetX, (int)targetY); // std::cout<<"A\n";
            attackMobs(targetX, targetY);
            lastAttackTime = currentTime; // Update last attack time
        }
    }
}

void SummonMobs()
{
    int SummonRand = rand() % 100;
    if (SummonRand == 0)
    {
        mobs newMob;
        newMob.x = rand() % MAP_WIDTH;
        newMob.y = rand() % MAP_HEIGHT;
        newMob.speed = 0.1;
        newMob.damage = 1;
        newMob.blood = 10;
        while (gameMap[(int)newMob.y][(int)newMob.x].type != GROUND)
        {
            newMob.x = rand() % MAP_WIDTH;
            newMob.y = rand() % MAP_HEIGHT;
        }
        allmobs.push_back(newMob);
    }
    for (size_t i = 0; i < allmobs.size(); i++)
    {
        bool moved = false;
        for (int attempt = 0; attempt < 3; attempt++)
        {
            float dx = (rand() % 3 - 1) * allmobs[i].speed;
            float dy = (rand() % 3 - 1) * allmobs[i].speed;
            float newX = allmobs[i].x + dx;
            float newY = allmobs[i].y + dy;
            if (CheckPos(newX, newY))
            {
                allmobs[i].x = newX;
                allmobs[i].y = newY;
                moved = true;
                break;
            }
        }
        if (!moved)
        {
            // If after multiple attempts the mob can't move, stay in place
        }
    }
}

// Main entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    mp[OBSTACLE] = "Stone";
    mp[TREE] = "Wood";
    mp[WORKBENCH] = "Bench";

    // Register window class
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SmoothMovementGame";
    RegisterClassA(&wc);

    // Create window
    HWND hwnd = CreateWindowExA(
        0,
        "SmoothMovementGame",
        "Termiverve v3",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Get console window handle
    // HWND ConsoleHwnd = GetConsoleWindow();

    // Hide console window
    // ShowWindow(ConsoleHwnd, SW_HIDE);

    // Generate random map
    generateRandomMap();

    // Message loop
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        // Process messages
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Check for pause key
            if (GetAsyncKeyState('P') & 0x8000)
            {
                isPaused = !isPaused;
                Sleep(200); // Prevent rapid toggling
            }

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
            }

            // Always redraw the main window to prevent visual glitches
            InvalidateRect(hwnd, NULL, TRUE);

            // Also redraw backpack window if it's open
            if (isBackpackOpen && backpackWindow != NULL)
            {
                InvalidateRect(backpackWindow, NULL, TRUE);
            }

            // Sleep to control frame rate
            Sleep(20);
        }
    }

    // Clean up backpack window if it's still open
    if (backpackWindow != NULL)
    {
        DestroyWindow(backpackWindow);
    }

    return 0;
}
