#pragma once
#include <string>
#include <map>


void CountInventoryItems(std::map<std::string, int> &itemCounts);
bool SlotsAreFull(); //check if hands and slots are full
void Drop(float x, float y);
void MobDeath();
