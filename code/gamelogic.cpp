#include "gamelogic.h"

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
bool isDestroyed(float x, float y)
{
    if (gameMap[(int)y][(int)x].health <= 0)
        return 1;
    return 0;
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
    for (int i = 0; i < allmobs.size(); i++)
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
