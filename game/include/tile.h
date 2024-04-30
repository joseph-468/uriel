#pragma once

#include <uriel.h>

#include "world.h"

class World;

enum TileType {
	AIR,
	COBBLESTONE,
	DIRT,
	GRASS
};

class Tile {
public:
	Tile();
	Tile(const Uint64 typeID);

	Uint64 typeID;
};

bool isCollidingWithTile(const World &world, SDL_FRect target, float collisionTolerance);
