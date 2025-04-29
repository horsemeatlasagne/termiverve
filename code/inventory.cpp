#include "constants.h"
#include "main.h"
#include "globals.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif


// Count items in the bag
std::map<std::string, int> CountBagItems()
{
    std::map<std::string, int> itemCounts;

    // Count items from the bag vector
    for (const auto &item : bag)
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
    for (size_t i = 0; i < allmobs.size(); i++)
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