#pragma once

#include <uriel.h>
#include <vector>

#include "world.h"

class World;
class TileType;

extern std::vector<TileType> tileTypes;

class TileType {
public:
	const std::string id;
	const Uint64 spriteId;
	const Uint8 width;
	const Uint8 height;
	const bool animated;
};

void createTileType(const std::string id, const Uint64 spriteId, const Uint8 width, const Uint8 height, const bool animated);

class Tile {
public:
	Tile();
	Tile(const Uint64 typeId, const Uint64 spriteId);

	Uint64 typeId;
	Uint64 spriteId; // ID 0 is reserved for no texture, probably will use tile type tedture id
};

bool isCollidingWithTile(const World &world, SDL_FRect target, float collisionTolerance);
