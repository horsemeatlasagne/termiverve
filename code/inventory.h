#pragma once
#include "main.h"
#include "constants.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

std::map<std::string, int> CountBagItems();
void Drop(float x, float y);
void MobDeath();
