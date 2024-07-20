#include <uriel.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <string>
#include <random>

#include "../include/game.h"
#include "../include/world.h"

using namespace Uriel;

World::World() : width(0), height(0), horizontalOffset(0), verticalOffset(0) {}

World::World(const Sint64 width, const Sint64 height, const std::vector<Tile> tiles) : width(width), height(height), tiles(tiles) {
	horizontalOffset = TILE_SIZE / 2 + (width / 2 * -TILE_SIZE);
	verticalOffset = (height / 2 * TILE_SIZE) - TILE_SIZE / 2;
}

void World::displayTile(const Uint64 x, const Uint64 y) {
	Tile &tile = tiles[y * width + x];
	if (tile.typeId == 0) return;

	Sint64 xPos = horizontalOffset + x * TILE_SIZE;
	Sint64 yPos = verticalOffset - y * TILE_SIZE;
	if (tileTypes[tile.typeId].animated) {
		drawAnimatedSprite(tile.animatedSprite, static_cast<float>(xPos), static_cast<float>(yPos), TILE_SIZE, TILE_SIZE);
	}
	else {
		drawSprite(tile.animatedSprite.getSpriteId(), static_cast<float>(xPos), static_cast<float>(yPos), TILE_SIZE, TILE_SIZE);
	}
}

void World::displayTiles(const Camera &camera) {
	Sint64 top = static_cast<Sint64>(floor(height / 2 - (camera.y + (camera.height / 2)) / TILE_SIZE));
	Sint64 bottom = static_cast<Sint64>(floor(height / 2 - (camera.y - (camera.height / 2)) / TILE_SIZE));
	Sint64 left = static_cast<Sint64>(floor(width / 2 + (camera.x - (camera.width / 2)) / TILE_SIZE));
	Sint64 right = static_cast<Sint64>(floor(width / 2 + (camera.x + (camera.width / 2)) / TILE_SIZE));

	top = std::clamp(top, static_cast<Sint64>(0), height - 1);
	bottom  = std::clamp(bottom, static_cast<Sint64>(0), height - 1);
	left = std::clamp(left, static_cast<Sint64>(0), width - 1);
	right = std::clamp(right, static_cast<Sint64>(0), width - 1);

	if (top == bottom || left == right) return;
	for (Sint64 y = top; y <= bottom; y++) {
		for (Sint64 x = left; x <= right; x++) {
			displayTile(x, y);
		}
	}
}

World generateWorld() {
	std::vector<Tile> tiles;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> oreVeinChance(0, 100); 
    std::uniform_int_distribution<std::mt19937::result_type> oreTypeChance(0, 1); 
	std::uniform_int_distribution<std::mt19937::result_type> oreChance(0, 8);

	tiles.resize(64 * 256);
	for (int y = 0; y < 64; y++) {
		for (int x = 0; x < 256; x++) {
			std::string tileType;
			if (y == 33) tileType = "Grass";
			if (y > 33) tileType = "Dirt";
			if (y > 39) tileType = "Cobblestone";
			tiles[y * 256 + x] = Tile(getTileTypeId(tileType), getSpriteId(tileType));
		}
	}

	for (int y = 45; y < 64; y++) {
		for (int x = 0; x < 256; x++) {
			if (!oreVeinChance(rng)) {
				std::string oreType = oreTypeChance(rng) ? "CopperOre" : "IronOre";
				tiles[y * 256 + x] = Tile(getTileTypeId(oreType), getSpriteId(oreType));
				for (int y2 = -5; y2 < 5; y2++) {
					for (int x2 = -5; x2 < 5; x2++) {
						if (!oreChance(rng)) {
							for (int i = 0; i < oreChance(rng) * 2; i++) {
								int offset = oreChance(rng);
								int posX = std::clamp(x + 5 - offset + x2, 1, 254);
								int posY = std::clamp(y + 5 - offset + y2, 1, 62);
								if (tiles[posY * 256 + posX - 1].typeId == getTileTypeId(oreType) ||
								tiles[posY * 256 + posX + 1].typeId == getTileTypeId(oreType) ||
								tiles[(posY - 1) * 256 + posX].typeId == getTileTypeId(oreType) ||
								tiles[(posY + 1) * 256 + posX].typeId == getTileTypeId(oreType) || 
								tiles[(posY - 1) * 256 + posX - 1].typeId == getTileTypeId(oreType) ||
								tiles[(posY - 1) * 256 + posX + 1].typeId == getTileTypeId(oreType) ||
								tiles[(posY - 1) * 256 + posX + 1].typeId == getTileTypeId(oreType) ||
								tiles[(posY + 1) * 256 + posX - 1].typeId == getTileTypeId(oreType)) {
									tiles[posY * 256 + posX] = Tile(getTileTypeId(oreType), getSpriteId(oreType));
								}
							}
						}
					}
				}
			}
		}
	}
	
	return World(256, 64, tiles);
}

