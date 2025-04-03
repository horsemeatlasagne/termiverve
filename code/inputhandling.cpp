#include "inputhandling.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
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
            std::swap(*SelectedDrop, Bar[0]);
        }
        else
        {
            HaveSelected = true;
            SelectedDrop = &Bar[0];
        }
    }
    if (GetAsyncKeyState(0x32) & 0x8000)
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
    if (GetAsyncKeyState(0x33) & 0x8000)
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
    if (GetAsyncKeyState(0x34) & 0x8000)
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
    if (GetAsyncKeyState(0x35) & 0x8000)
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
    if (GetAsyncKeyState(0x70) & 0x8000)
    {
        if (HaveSelected)
        {
            HaveSelected = false;
            std::swap(*SelectedDrop, onPlayer[0]);
        }
        else
        {
            HaveSelected = true;
            SelectedDrop = &onPlayer[0];
        }
    }
    if (GetAsyncKeyState(0x71) & 0x8000)
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
    if (GetAsyncKeyState(0x72) & 0x8000)
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

    // Improved backpack toggle with B key
    bool bKeyIsPressed = (GetAsyncKeyState('B') & 0x8000) != 0;
    if (bKeyIsPressed)
    {
        ToggleBackpackWindow(GetForegroundWindow(), GetModuleHandle(NULL));
        // Sleep(200);
    }
}