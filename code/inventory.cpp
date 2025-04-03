#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
// #include <chrono>
#include <windowsx.h>
#include <string>
#include <algorithm>
#include <map>
#include <cstdio>
#include "constants.h"
#include "main.h"
#include "inventory.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
// Initialize game items
GameDrops LeftHand = EMPTY;
GameDrops RightHand = EMPTY;
GameDrops Bar[12] = { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY };
GameDrops onPlayer[5] = { EMPTY, EMPTY, EMPTY };
std::vector<std::string> bag;


// Count items in the bag
std::map<std::string, int> CountBagItems()
{
    std::map<std::string, int> itemCounts;

    // Count items from the bag vector
    for (const auto& item : bag)
    {
        itemCounts[item]++;
    }

    // Count items from hands and slots
    if (LeftHand != EMPTY)
        itemCounts[DropsName[LeftHand]]++;
    if (RightHand != EMPTY)
        itemCounts[DropsName[RightHand]]++;

    for (int i = 0; i < 3; i++)
    {
        if (onPlayer[i] != EMPTY)
            itemCounts[DropsName[onPlayer[i]]]++;
    }

    for (int i = 0; i < 5; i++)
    {
        if (Bar[i] != EMPTY)
            itemCounts[DropsName[Bar[i]]]++;
    }

    return itemCounts;
}

void Drop(float x, float y)
{
    bag.push_back(mp[gameMap[(int)y][(int)x].type]); // Missing drop function
    std::sort(bag.begin(), bag.end());
}

void MobDeath()
{
    for (int i = 0; i < allmobs.size(); i++)
    {
        while (i < allmobs.size() && allmobs[i].blood <= 0)
        {
            allmobs.erase(allmobs.begin() + i);
            if (LeftHand == EMPTY)
                LeftHand = STICK;
            else if (RightHand == EMPTY)
                RightHand = STICK;
            else
            {
                for (int i = 0; i < 5; i++)
                {
                    if (Bar[i] == EMPTY)
                    {
                        Bar[i] = STICK;
                        break;
                    }
                }
            }
        }
    }
}