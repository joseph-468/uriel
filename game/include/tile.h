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
	Tile(const Uint64 typeId, const Uint64 spriteId);

	Uint64 typeId;
	Uint64 spriteId; // ID 0 is reserved for no texture
};

bool isCollidingWithTile(const World &world, SDL_FRect target, float collisionTolerance);
