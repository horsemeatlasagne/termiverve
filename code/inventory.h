#pragma once
#include "main.h"

extern std::vector<std::string> bag;
extern GameDrops LeftHand;
extern GameDrops RightHand;
extern GameDrops onPlayer[3];
extern GameDrops Bar[5];
extern std::string DropsName[];
extern std::string ModelName[];
std::map<std::string, int> CountBagItems();