#pragma once

#include <uriel.h>
#include <vector>
#include <unordered_map>

#include "world.h"

using namespace Uriel;

class World;
class TileType;

extern std::vector<TileType> tileTypes;
extern std::unordered_map<std::string, Uint16> tileTypesMap;

class TileType {
public:
	const std::string id;
	const Uint16 spriteId;
	const Uint8 width;
	const Uint8 height;
	const float defaultFrameRate;
	const bool animated;
};

Uint16 getTileTypeId(const std::string &name);

void createTileType(const std::string id, const Uint16 spriteId, const Uint8 width, const Uint8 height, const float defaultFrameRate, const bool animated);

class Tile {
public:
	Tile();
	Tile(const Uint16 typeId, const Uint16 spriteId);

	Uint16 typeId;
	AnimatedSprite animatedSprite;
};

bool isCollidingWithTile(const World &world, SDL_FRect target, float collisionTolerance);
