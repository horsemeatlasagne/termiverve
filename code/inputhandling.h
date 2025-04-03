#pragma once
#include "main.h"
#include <windows.h>
#include <windowsx.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "constants.h"

void handleInput();
void updateMouseHover();