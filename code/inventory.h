#pragma once
#include <string>
#include <map>


void CountInventoryItems(std::map<std::string, int> &itemCounts);
bool SlotsAreFull(); //check if hands and slots are full
void Drop(const int x, const int y); //drop to slots and bag
void MobDeath();
