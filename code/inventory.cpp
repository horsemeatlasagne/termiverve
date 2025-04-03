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

// Initialize game items
GameDrops LeftHand = EMPTY;
GameDrops RightHand = EMPTY;
GameDrops Bar[12] = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
GameDrops onPlayer[5] = {EMPTY, EMPTY, EMPTY};

std::vector<std::string> bag;

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