#include <cmath>
#include <raylib.h>
#include "constants.h"
#include "inputhandling.h"
#include "gamelogic.h"
#include "inventory.h"
#include "main.h"
#include "globals.h"
// Handle input
void handleInput()
{
    if (IsKeyDown(KEY_LEFT) || IsKeyDown('A'))
    {
        if (CheckPos(playerX - PLAYER_SPEED, playerY, true) > 0)
            playerX -= PLAYER_SPEED;
        else if (CheckPos(playerX - PLAYER_SPEED, playerY, true) == 0) // excl. OOB
            playerX = floor(playerX);
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown('D'))
    {
        if (CheckPos(playerX + PLAYER_SPEED, playerY, true) > 0)
            playerX += PLAYER_SPEED;
        else if (CheckPos(playerX + PLAYER_SPEED, playerY, true) == 0) // excl. OOB
            playerX = floor(playerX);
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown('W'))
    {
        if (CheckPos(playerX, playerY - PLAYER_SPEED, true) > 0)
            playerY -= PLAYER_SPEED;
        else if (CheckPos(playerX, playerY - PLAYER_SPEED, true) == 0) // excl. OOB
            playerY = floor(playerY);
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown('S'))
    {
        if (CheckPos(playerX, playerY + PLAYER_SPEED, true) > 0)
            playerY += PLAYER_SPEED;
        else if (CheckPos(playerX, playerY + PLAYER_SPEED, true) == 0) // excl. OOB
            playerY = floor(playerY);
    }
    // L=0x4c,R=0x52,1~5=0x31~0x35,F1~F3=0x70~0x72
    if (IsKeyDown(KEY_L))
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
    if (IsKeyDown(KEY_R))
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
    if (IsKeyDown(KEY_ONE))
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
    if (IsKeyDown(KEY_TWO))
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
    if (IsKeyDown(KEY_THREE))
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
    if (IsKeyDown(KEY_FOUR))
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
    if (IsKeyDown(KEY_FIVE))
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
    if (IsKeyDown(KEY_F1))
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
    if (IsKeyDown(KEY_F2))
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
    if (IsKeyDown(KEY_F3))
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
    bool bKeyIsPressed = (IsKeyDown(KEY_B)) != 0;
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