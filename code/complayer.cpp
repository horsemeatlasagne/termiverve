// Compatibility layer-- w32-2-raylib.

#ifndef _termiverve_win32_api
#define _termiverve_win32_api // Win32 DEFINED HERE
#endif

#ifdef _termiverve_win32_api
#include <windows.h>
#else
#include <raylib.h>
#endif

#include <cmath>
#include "constants.h"
#include "globals.h"

namespace termiVerve::api
{
    namespace Input
    {
#ifdef _termiverve_win32_api
        bool isKeyPressed(int key)
        {
            return (GetAsyncKeyState(key) & 0x8000);
        }

        void getMousePosition(LONG &x, LONG &y);
        {
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            ScreenToClient(GetForegroundWindow(), &cursorPos);
            x = cursorPos.x;
            y = cursorPos.y;
        }
#else
        bool isKeyPressed(int key)
        {
            // Convert Windows key codes to Raylib key codes if needed
            int raylibKey = -1;
            switch (key)
            {
            case 0x41: // 'A'
                raylibKey = KEY_A;
                break;
            case 0x44: // 'D'
                raylibKey = KEY_D;
                break;
            case 0x57: // 'W'
                raylibKey = KEY_W;
                break;
            case 0x53: // 'S'
                raylibKey = KEY_S;
                break;
            case VK_LEFT:
                raylibKey = KEY_LEFT;
                break;
            case VK_RIGHT:
                raylibKey = KEY_RIGHT;
                break;
            case VK_UP:
                raylibKey = KEY_UP;
                break;
            case VK_DOWN:
                raylibKey = KEY_DOWN;
                break;
            case 0x4c: //'L'
                raylibKey = KEY_L;
                break;
            case 0x52: //'R'
                raylibKey = KEY_R;
                break;
            case 0x31: // '1'
                raylibKey = KEY_ONE;
                break;
            case 0x32: // '2'
                raylibKey = KEY_TWO;
                break;
            case 0x33: // '3'
                raylibKey = KEY_THREE;
                break;
            case 0x34: // '4'
                raylibKey = KEY_FOUR;
                break;
            case 0x35: // '5'
                raylibKey = KEY_FIVE;
                break;
            case 0x70: // F1
                raylibKey = KEY_F1;
                break;
            case 0x71: // F2
                raylibKey = KEY_F2;
                break;
            case 0x72: // F3
                raylibKey = KEY_F3;
                break;
            case 0x42: // 'B'
                raylibKey = KEY_B;
                break;
            default:
                break;
            }
            if (raylibKey != -1)
            {
                return IsKeyPressed(raylibKey);
            }
            return false;
        }
        void getMousePosition(int &x, int &y)
        {
            Vector2 mousePos = GetMousePosition();
            x = static_cast<int>(mousePos.x);
            y = static_cast<int>(mousePos.y);
        }
#endif
    }
}