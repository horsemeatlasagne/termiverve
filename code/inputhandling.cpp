#include <cmath>
#include "constants.h"
#include "inputhandling.h"
#include "gamelogic.h"
#include "inventory.h"
#include "main.h"
#include "globals.h"
// Handle input
void handleInput()
{
    if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000))
    {
        if (CheckPos(playerX - PLAYER_SPEED, playerY, true) > 0)
            playerX -= PLAYER_SPEED;
        else if (CheckPos(playerX - PLAYER_SPEED, playerY, true) == 0) // excl. OOB
            playerX = floor(playerX);
    }
    if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000))
    {
        if (CheckPos(playerX + PLAYER_SPEED, playerY, true) > 0)
            playerX += PLAYER_SPEED;
        else if (CheckPos(playerX + PLAYER_SPEED, playerY, true) == 0) // excl. OOB
            playerX = floor(playerX);
    }
    if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000))
    {
        if (CheckPos(playerX, playerY - PLAYER_SPEED, true) > 0)
            playerY -= PLAYER_SPEED;
        else if (CheckPos(playerX, playerY - PLAYER_SPEED, true) == 0) // excl. OOB
            playerY = floor(playerY);
    }
    if ((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('S') & 0x8000))
    {
        if (CheckPos(playerX, playerY + PLAYER_SPEED, true) > 0)
            playerY += PLAYER_SPEED;
        else if (CheckPos(playerX, playerY + PLAYER_SPEED, true) == 0) // excl. OOB
            playerY = floor(playerY);
    }
    // L=0x4c,R=0x52,1~5=0x31~0x35,F1~F3=0x70~0x72
    if (GetAsyncKeyState(0x4c) & 0x8000)
    {
        if (HaveSelected)
        {
            HaveSelected = false;
            std::swap(*SelectedDrop, LeftHand);
        }
        else
        {
            HaveSelected = true;
            SelectedDrop = &LeftHand;
        }
    }
    if (GetAsyncKeyState(0x52) & 0x8000)
    {
        if (HaveSelected)
        {
            HaveSelected = false;
            std::swap(*SelectedDrop, RightHand);
        }
        else
        {
            HaveSelected = true;
            SelectedDrop = &RightHand;
        }
    }
    if (GetAsyncKeyState(0x31) & 0x8000)
    {
        if (HaveSelected)
        {
            HaveSelected = false;
            std::swap(*SelectedDrop, Bar[1]);
        }
        else
        {
            HaveSelected = true;
            SelectedDrop = &Bar[1];
        }
    }
    if (GetAsyncKeyState(0x32) & 0x8000)
    {
        if (HaveSelected)
        {
            HaveSelected = false;
            std::swap(*SelectedDrop, Bar[2]);
        }
        else
        {
            HaveSelected = true;
            SelectedDrop = &Bar[2];
        }
    }
    if (GetAsyncKeyState(0x33) & 0x8000)
    {
        if (HaveSelected)
        {
            HaveSelected = false;
            std::swap(*SelectedDrop, Bar[3]);
        }
        else
        {
            HaveSelected = true;
            SelectedDrop = &Bar[3];
        }
    }
    if (GetAsyncKeyState(0x34) & 0x8000)
    {
        if (HaveSelected)
        {
            HaveSelected = false;
            std::swap(*SelectedDrop, Bar[4]);
        }
        else
        {
            HaveSelected = true;
            SelectedDrop = &Bar[4];
        }
    }
    if (GetAsyncKeyState(0x35) & 0x8000)
    {
        if (HaveSelected)
        {
            HaveSelected = false;
            std::swap(*SelectedDrop, Bar[5]);
        }
        else
        {
            HaveSelected = true;
            SelectedDrop = &Bar[5];
        }
    }
    if (GetAsyncKeyState(0x70) & 0x8000)
    {
        if (HaveSelected)
        {
            HaveSelected = false;
            std::swap(*SelectedDrop, onPlayer[1]);
        }
        else
        {
            HaveSelected = true;
            SelectedDrop = &onPlayer[1];
        }
    }
    if (GetAsyncKeyState(0x71) & 0x8000)
    {
        if (HaveSelected)
        {
            HaveSelected = false;
            std::swap(*SelectedDrop, onPlayer[2]);
        }
        else
        {
            HaveSelected = true;
            SelectedDrop = &onPlayer[2];
        }
    }
    if (GetAsyncKeyState(0x72) & 0x8000)
    {
        if (HaveSelected)
        {
            HaveSelected = false;
            std::swap(*SelectedDrop, onPlayer[3]);
        }
        else
        {
            HaveSelected = true;
            SelectedDrop = &onPlayer[3];
        }
    }

    // Improved backpack toggle with B key
    bool bKeyIsPressed = (GetAsyncKeyState('B') & 0x8000) != 0;
    if (bKeyIsPressed)
    {
        ToggleBackpackWindow(GetForegroundWindow(), GetModuleHandle(NULL));
        // Sleep(200);
    }
}
// Update mouse hover information
void updateMouseHover()
{
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(GetForegroundWindow(), &cursorPos);

    // Convert screen coordinates to grid coordinates
    int gridX = (cursorPos.x - 400 + playerX * GRID_SIZE) / GRID_SIZE;
    int gridY = (cursorPos.y - 400 + playerY * GRID_SIZE) / GRID_SIZE;

    // Check if mouse is within map bounds
    if (gridX >= 0 && gridX < MAP_WIDTH && gridY >= 0 && gridY < MAP_HEIGHT)
    {
        // Check if mouse has moved to a new grid cell
        if (gridX != lastMouseGridX || gridY != lastMouseGridY)
        {
            lastMouseGridX = gridX;
            lastMouseGridY = gridY;
            hoverStartTime = GetTickCount(); // Reset hover timer
            showHoverInfo = false;
        }
        else if (!showHoverInfo && GetTickCount() - hoverStartTime > 500) // 0.5 seconds
        {
            // Mouse has been hovering over the same cell for 0.5 seconds
            showHoverInfo = true;
        }
    }
    else
    {
        // Mouse is outside map bounds
        showHoverInfo = false;
        lastMouseGridX = -1;
        lastMouseGridY = -1;
    }

    lastMousePos = cursorPos;
}