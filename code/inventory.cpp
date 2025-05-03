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

void DropToSlots(const int x, const int y)
{
    // Convert GameObject to GameDrops
    GameDrops itemType;
    GameObject objType = gameMap[y][x].type;

    // Map GameObject to GameDrops
    if (objType == OBSTACLE)
    {
        itemType = STONE;
    }
    else if (objType == TREE)
    {
        itemType = WOOD;
    }
    else if (objType == WORKBENCH)
    {
        itemType = WORK;
    }
    else
    {
        // Default case or unknown type
        itemType = EMPTY;
        printf("Drop failed.\n");
        return; // Can't pick up GROUND or PLAYER
    }
    if (LeftHand == EMPTY) //fill hands first
    {
        LeftHand = itemType;
        return;
    }
    if (RightHand == EMPTY)
    {
        RightHand = itemType;
        return;
    }
    for (int i = 1; i <= 3; i++) //fill slots on player
    {
        if (onPlayer[i] == EMPTY)
        {
            onPlayer[i] = itemType;
            return;
        }
    }
    for (int i = 1; i <= 5; i++) //fill bars
    {
        if (Bar[i] == EMPTY)
        {
            Bar[i] = itemType;
            return;
        }
    }
}

void Drop(const int x, const int y)
{
    if (SlotsAreFull())
    { //check if hands and slots are full
        printf("slots are full, dropping to bag.\n"); //debugging helper
        bag.push_back(mp[gameMap[(int)y][(int)x].type]);
        std::sort(bag.begin(), bag.end());
    }
    else //implement drop to hands and slots
    {
        printf("Drop to slots.\n"); //debugging helper
        DropToSlots(x, y);
    }
}

void MobDeath()
{
    for (size_t i = 0; i < allmobs.size(); i++)
    {
        while (i < allmobs.size() && allmobs[i].blood <= 0)
        {
            allmobs.erase(allmobs.begin() + i); //logic needs optimisation
            //following code = add to invt
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