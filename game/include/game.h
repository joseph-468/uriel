#pragma once

#include "world.h"

constexpr const int TILE_SIZE = 32;

void moveAndResolveCollision(World &currentWorld, SDL_FRect &target, float &xVel, float &yVel);
