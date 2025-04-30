#pragma once


short CheckPos(float X, float Y, bool isPlayer = false);
bool isDestroyed(float x, float y);
void attackTarget(int x, int y);
void attackMobs(double x, double y);
void handleMouseClick();