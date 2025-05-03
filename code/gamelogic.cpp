#include <cmath>
#include <ctime>
#include "constants.h"
#include "gamelogic.h"
#include "main.h"
#include "inventory.h"
#include "globals.h"

short CheckPos(float X, float Y, bool isPlayer)
{
    // First check if we're within map boundaries
    if (X < 0 || X > MAP_WIDTH - 1 || Y < 0 || Y > MAP_HEIGHT - 1)
        return -1;

    // Define player hitbox corners (using a consistent offset value)
    const float offset = 0.8f; // Player size within a cell

    // Check all four corners of the player's hitbox
    int cornerX1 = floor(X);
    int cornerY1 = floor(Y);
    int cornerX2 = floor(X + offset);
    int cornerY2 = floor(Y + offset);
    // if (isPlayer)
    //     printf("cornerX1: %d, cornerY1: %d, cornerX2: %d, cornerY2: %d\n, Ground status: %d\n",
    //            cornerX1, cornerY1, cornerX2, cornerY2, gameMap[cornerY1][cornerX1].type == GROUND);
    // Make sure all corners are within bounds
    if (cornerX2 >= MAP_WIDTH || cornerY2 >= MAP_HEIGHT)
        return -1;

    // Check if any corner is colliding with a non-ground object
    if (gameMap[cornerY1][cornerX1].type != GROUND)
        return false;
    if (gameMap[cornerY1][cornerX2].type != GROUND)
        return false;
    if (gameMap[cornerY2][cornerX1].type != GROUND)
        return false;
    if (gameMap[cornerY2][cornerX2].type != GROUND)
        return false;

    return true;
}

void SummonMobs()
{
    int SummonRand = rand() % 50;
    if (SummonRand == 0)
    {
        printf("mob spawned\n");
        mobs newMob;
        newMob.x = rand() % MAP_WIDTH;
        newMob.y = rand() % MAP_HEIGHT;
        newMob.speed = 0.1;
        newMob.damage = 1;
        newMob.blood = 10;
        while (gameMap[(int)newMob.y][(int)newMob.x].type != GROUND)
        {
            newMob.x = rand() % MAP_WIDTH;
            newMob.y = rand() % MAP_HEIGHT;
        }
        allmobs.push_back(newMob);
    }
    for (size_t i = 0; i < allmobs.size(); i++)
    {
        bool moved = false;
        for (int attempt = 0; attempt < 3; attempt++)
        {
            float dx = (rand() % 3 - 1) * allmobs[i].speed;
            float dy = (rand() % 3 - 1) * allmobs[i].speed;
            float newX = allmobs[i].x + dx;
            float newY = allmobs[i].y + dy;
            if (CheckPos(newX, newY))
            {
                allmobs[i].x = newX;
                allmobs[i].y = newY;
                moved = true;
                break;
            }
        }
        if (!moved)
        {
            // If after multiple attempts the mob can't move, stay in place
        }
    }
}


bool isDestroyed(float x, float y)
{
    // Check if the object at the given position is destroyed
    return gameMap[(int)y][(int)x].health <= 0;
}
void attackTarget(int x, int y)
{
    if (gameMap[y][x].type != GROUND)
    {
        gameMap[y][x].health -= attackPower;
        // printf("x: %d, y: %d, health: %d\n", x, y, gameMap[y][x].health);
    }
}

void attackMobs(double x, double y)
{
    for (size_t i = 0; i < allmobs.size(); i++)
    {
        double mobX = allmobs[i].x, mobY = allmobs[i].y;
        if (mobX <= x && mobX + GRID_SIZE >= x && mobY <= y && mobY + GRID_SIZE >= y)
        {
            allmobs[i].blood -= attackPower;
            if (LeftHand == STICK || RightHand == STICK)
                allmobs[i].blood -= stickAttackPower;
        }
    }
}

// Handle mouse click for attacks
void handleMouseClick()
{
    int currentTime = clock();
    if (currentTime - lastAttackTime >= 200)
    { // Attack interval is 0.2 seconds
        // std::cout<<1;
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        ScreenToClient(GetForegroundWindow(), &cursorPos);

        float targetX = (cursorPos.x - 400 + playerX * GRID_SIZE) / GRID_SIZE;
        float targetY = (cursorPos.y - 400 + playerY * GRID_SIZE) / GRID_SIZE;
        // std::cout<< targetX << ' ' << targetY << ' ' << playerX << ' ' << playerY <<"\n";

        if (targetX >= 0 && targetX < MAP_WIDTH && targetY >= 0 && targetY < MAP_HEIGHT &&
            isPathClear(playerX, playerY, (int)targetX, (int)targetY) && KEY_DOWN(MOUSE_MOVED))
        {
            attackTarget((int)targetX, (int)targetY); // std::cout<<"A\n";
            if (isDestroyed(targetX, targetY) && gameMap[(int)targetY][(int)targetX].type != GROUND)
            {
                bag.push_back(mp[gameMap[(int)targetY][(int)targetX].type]); //temporary
            }
            attackMobs(targetX, targetY);
            lastAttackTime = currentTime; // Update last attack time
        }
    }
}