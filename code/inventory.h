#pragma once
#include "main.h"
#include "constants.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
extern std::vector<std::string> bag;
extern GameDrops LeftHand;
extern GameDrops RightHand;
extern GameDrops onPlayer[3];
extern GameDrops Bar[5];
extern std::string DropsName[];
extern std::string ModelName[];
std::map<std::string, int> CountBagItems();
void Drop(float x, float y);
void MobDeath();
