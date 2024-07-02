#pragma once

#include "world.h"

constexpr const Sint64 TILE_SIZE = 32;

void moveAndResolveCollision(World &world, SDL_FRect &target, float &xVel, float &yVel);
bool isTouchingGround(World &world, SDL_FRect target);
