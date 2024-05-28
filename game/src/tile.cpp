#include <uriel.h>
#include <algorithm>
#include <vector>
#include <unordered_map>

#include "../include/game.h"
#include "../include/tile.h"
#include "../include/world.h"

std::vector<TileType> tileTypes;
std::unordered_map<std::string, Uint16> tileTypesMap;

Tile::Tile() : typeId(0) {}

Tile::Tile(const Uint16 typeId) : typeId(typeId) {}

void createTileType(const std::string id, const Uint16 spriteId, const Uint8 width, const Uint8 height) {
	TileType tileType(id, spriteId, width, height);
	tileTypes.push_back(tileType);
	tileTypesMap.insert(std::make_pair(id, tileTypes.size() - 1));
}

Uint16 getTileTypeIndex(const std::string &name) {
	return tileTypesMap[name];
}

bool isCollidingWithTile(const World &world, SDL_FRect target, float collisionTolerance) {
	Sint64 top = static_cast<Sint64>(floor(world.height / 2 - (target.y + ((target.h - collisionTolerance) / 2)) / TILE_SIZE));
	Sint64 bottom = static_cast<Sint64>(floor(world.height / 2 - (target.y - ((target.h - collisionTolerance) / 2)) / TILE_SIZE));
	Sint64 left = static_cast<Sint64>(floor(world.width / 2 + (target.x - ((target.w - collisionTolerance) / 2)) / TILE_SIZE));
	Sint64 right = static_cast<Sint64>(floor(world.width / 2 + (target.x + ((target.w - collisionTolerance) / 2)) / TILE_SIZE));

	if (top < 0 && bottom < 0) return false;
	if (left < 0 && right < 0) return false;
	if (top >= world.height && bottom >= world.height) return false;
	if (left >= world.height && right >= world.width) return false;

	top = std::clamp(top, static_cast<Sint64>(0), world.height - 1);
	bottom  = std::clamp(bottom, static_cast<Sint64>(0), world.height - 1);
	left = std::clamp(left, static_cast<Sint64>(0), world.width - 1);
	right = std::clamp(right, static_cast<Sint64>(0), world.width - 1);

	for (Sint64 y = top; y <= bottom; y++) {
		for (Sint64 x = left; x <= right; x++) {
			if (world.tiles[y * world.width + x].typeId != 0) {
				return true;
			}
		}
	}

	return false;
}