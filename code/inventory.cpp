#include <algorithm>
#include "constants.h"
#include "main.h"
#include "globals.h"



// Count items in the inventory. Inventory=bag+hands+slots
void CountInventoryItems(std::map<std::string, int> &itemCounts)
{

    // Count items from the bag vector. Bag=backpack
    for (const auto &item : bag)
    {
        ++itemCounts[item];
    }

    // Count items from hands and slots. Hands=left+right, slots=onPlayer+Bar
    if (LeftHand != EMPTY)
        ++itemCounts[DropsName[LeftHand]];
    if (RightHand != EMPTY)
        ++itemCounts[DropsName[RightHand]];

    for (int i = 1; i <= 3; i++)
    {
        if (onPlayer[i] != EMPTY)
            ++itemCounts[DropsName[onPlayer[i]]];
    }

    for (int i = 1; i <= 5; i++)
    {
        if (Bar[i] != EMPTY)
            ++itemCounts[DropsName[Bar[i]]];
    }
    return;
}

bool SlotsAreFull()
{
    int cnt = 0;
    if (LeftHand != EMPTY)
        ++cnt;
    if (RightHand != EMPTY)
        ++cnt;
    for (int i = 1; i <= 3; i++)
    {
        if (onPlayer[i] != EMPTY)
            ++cnt;
    }
    for (int i = 1; i <= 5; i++)
    {
        if (Bar[i] != EMPTY)
            ++cnt;
    }
    return cnt == 10; //10 slots
}

void Drop(float x, float y)
{
    if (!SlotsAreFull()) //check if hands and slots are full
        bag.push_back(mp[gameMap[(int)y][(int)x].type]); // Missing drop function
    std::sort(bag.begin(), bag.end());
}

void MobDeath()
{
    for (size_t i = 0; i < allmobs.size(); i++)
    {
        while (i < allmobs.size() && allmobs[i].blood <= 0)
        {
            allmobs.erase(allmobs.begin() + i); //logic needs optimisation
            if (LeftHand == EMPTY)
                LeftHand = STICK;
            else if (RightHand == EMPTY)
                RightHand = STICK;
            else
            {
                for (int i = 1; i <= 5; i++)
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