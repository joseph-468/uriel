#include <uriel.h>
#include <iostream>
#include <vector>

#include "../include/game.h"
#include "../include/tile.h"
#include "../include/world.h"
#include "../include/external_data.h"

using namespace Uriel;

void displayFPS() {
	std::string text = std::to_string(getFPS());
	text.append(" FPS");
	renderText(text.c_str());
}

int main(int argc, char *argv[]) {
	init(1280, 720, "Uriel Test");

	Camera camera(0, 0, 2560, 1440);
	setActiveCamera(camera);

	// Load sprite sheets 
	auto spriteSheetsResult = getTags("assets/data/sprite_sheets.dat");
	if (!spriteSheetsResult) return 1;
	std::vector<Tag> spriteSheetTags = spriteSheetsResult.value();
	for (auto &tag : spriteSheetTags) {
		createSpriteSheet(tag.getValue("name"), tag.getValue("filepath"));
	}

	// Load sprites
 	auto spritesResult = getTags("assets/data/sprites.dat");
	if (!spritesResult) return 1;
	std::vector<Tag> spriteTags = spritesResult.value();
	for (auto &tag : spriteTags) {
		std::string name = tag.getValue("name");
		Uint16 spriteSheet = getSpriteSheetId(tag.getValue("spriteSheet"));
		Uint16 frameCount = std::stoi(tag.getValue("frames"));
		SDL_Rect src;
		src.x = std::stoi(tag.getValue("x"));
		src.y = std::stoi(tag.getValue("y"));
		src.w = std::stoi(tag.getValue("w"));
		src.h = std::stoi(tag.getValue("h"));
		createSprite(name, spriteSheet, src, frameCount);
	}

	// Load tiles
	auto tilesResult = getTags("assets/data/tiles.dat");
	if (!tilesResult) return 1;
	std::vector<Tag> tileTags = tilesResult.value();
	for (auto &tag : tileTags) {
		createTileType(tag.getValue("name"), getSpriteId(tag.getValue("name")),
			std::stoi(tag.getValue("w")), std::stoi(tag.getValue("h")),
			std::stof(tag.getValue("defaultFrameRate")), tag.getValue("animated") == "true");
	}

	Uint16 playerSprite = getSpriteId("Player");
	Uint16 backgroundSprite = getSpriteId("Hills");

	World currentWorld = generateWorld();

	bool controllingCamera = false;

	SDL_FRect player = { 0, TILE_SIZE, (TILE_SIZE * 2) - (TILE_SIZE * 2 / 8), (TILE_SIZE * 4) - (TILE_SIZE * 4 / 8) };
	float xVel = 0;
	float yVel = 0;

	int prevButton = -69;
	int prevBlockX = -6969;
	int prevBlockY = -696969;
	std::string equippedBlock = "Cobblestone";

	while (tick()) {
		// Input
		while (getEvent()) {
			switch (event.type) {
				case SDL_MOUSEWHEEL: {
					if (!controllingCamera) break;
					float distance = event.wheel.y * -100;
					float widthRatio = camera.width / camera.height;
					camera.width += distance * widthRatio;
					camera.height += distance;
				} break;
				case SDL_MOUSEBUTTONDOWN: {
					int x, y;
					SDL_GetMouseState(&x, &y);
					SDL_FPoint mouseWorldPos = camera.convertScreenToWorldCoords(x, y);
					int worldX = currentWorld.width - floor(currentWorld.width / 2 - mouseWorldPos.x / TILE_SIZE) - 1;
					int worldY = floor(currentWorld.height / 2 - mouseWorldPos.y / TILE_SIZE);
					if (event.button.button == prevButton && prevBlockX == worldX && prevBlockY == worldY) break;
					if (event.button.button == SDL_BUTTON_LEFT) {
						currentWorld.tiles[worldX + worldY * currentWorld.width].typeId = 0;
					}
					else {
						currentWorld.tiles[worldX + worldY * currentWorld.width] = Tile(getTileTypeId(equippedBlock), getSpriteId(equippedBlock));
						currentWorld.tiles[worldX + worldY * currentWorld.width].animatedSprite.play();
					}
					prevButton = event.button.button;
					prevBlockX = worldX;
					prevBlockY = worldY;
				} break;
			}
		}

		// Logic
		if (keyIsPressed(SDL_SCANCODE_ESCAPE)) {
			if (controllingCamera) {
				controllingCamera = false;
			}
			else {
				controllingCamera = true;
			}
		}

		if (keyIsPressed(SDL_SCANCODE_F1)) equippedBlock = "Cobblestone";
		if (keyIsPressed(SDL_SCANCODE_F2)) equippedBlock = "Orb";

		xVel = 0;
		yVel = 0;
		if (controllingCamera) {
			if (keyIsDown(SDL_SCANCODE_W)) camera.y += 1 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_A)) camera.x -= 1 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_S)) camera.y -= 1 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_D)) camera.x += 1 * deltaTime;
		}
		else {
			if (keyIsDown(SDL_SCANCODE_W)) yVel = 0.2 * deltaTime;
			else yVel = -0.2 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_A)) {
				xVel = -0.1 * deltaTime;
			}
			if (keyIsDown(SDL_SCANCODE_D)) {
				xVel = 0.1 * deltaTime;
			}
		}

		moveAndResolveCollision(currentWorld, player, xVel, yVel);

		if (!controllingCamera) {
			camera.x = player.x;
			camera.y = player.y;
		}

		// Rendering 
		drawSprite(backgroundSprite, camera.x, camera.y, camera.width, camera.height);

		currentWorld.displayTiles(camera);

		drawSprite(playerSprite, player.x, player.y, player.w, player.h);

		displayFPS();
	}

	quit();
	return 0;
}