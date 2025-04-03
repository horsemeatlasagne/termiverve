#pragma once
#include <vector>
#include <string>
#include <map>

// Game object types
enum GameObject
{
    GROUND,
    OBSTACLE,
    TREE,
    WORKBENCH,
    PLAYER
};

// Item types
enum GameDrops
{
    EMPTY,
    WOOD,
    STONE,
    LEAF,
    WORK,
    STICK
};
// Global variable declarations
std::vector<std::string> bag;
GameDrops LeftHand;
GameDrops RightHand;
GameDrops onPlayer[3];
GameDrops Bar[5];
extern std::string DropsName[];
extern std::string ModelName[];

// Function prototypes
std::map<std::string, int> CountBagItems();



