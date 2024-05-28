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

	Camera camera(0, 0, 1280, 720);
	setActiveCamera(camera);

	// Load sprite sheets 
	auto spriteSheetsResult = getTags("assets/data/sprite_sheets.dat");
	if (!spriteSheetsResult) return 1;
	std::vector<Tag> spriteSheetTags = spriteSheetsResult.value();
	for (auto &tag : spriteSheetTags) {
		createSpriteSheet(tag.getValue("id"), tag.getValue("filepath"));
	}

	// Load sprites
 	auto spritesResult = getTags("assets/data/sprites.dat");
	if (!spritesResult) return 1;
	std::vector<Tag> spriteTags = spritesResult.value();
	for (auto &tag : spriteTags) {
		std::string name = tag.getValue("id");
		Uint16 spriteSheet = getSpriteSheetIndex(tag.getValue("spriteSheet"));
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
		createTileType(tag.getValue("id"), getSpriteIndex(tag.getValue("id")), std::stoi(tag.getValue("w")), std::stoi(tag.getValue("h")));
	}

	Uint16 orb = getSpriteIndex("Orb");
	AnimatedSprite animatedOrb(orb, 3);
	Uint16 playerSprite = getSpriteIndex("Player");
	Uint16 backgroundSprite = getSpriteIndex("Hills");

	bool controllingCamera = false;
	SDL_FRect player = { 0, 0, 32, 64};

	World currentWorld = generateWorld();

	while (tick()) {
		// Input
		while (getEvent()) {
			switch (event.type) {
				case SDL_MOUSEWHEEL: {
					if (!controllingCamera) break;
					float distance = event.wheel.y * -10;
					float widthRatio = camera.width / camera.height;
					camera.width += distance * widthRatio;
					camera.height += distance;
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

		if (keyIsPressed(SDL_SCANCODE_F1)) animatedOrb.play(true);
		if (keyIsPressed(SDL_SCANCODE_F2)) animatedOrb.stop();
		if (keyIsPressed(SDL_SCANCODE_F3)) animatedOrb.resume();
		if (keyIsPressed(SDL_SCANCODE_F4)) animatedOrb.pause();

		float xVel = 0;
		float yVel = 0;

		if (controllingCamera) {
			if (keyIsDown(SDL_SCANCODE_W)) camera.y += 1 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_A)) camera.x -= 1 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_S)) camera.y -= 1 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_D)) camera.x += 1 * deltaTime;
		}
		else {
			if (keyIsDown(SDL_SCANCODE_W)) yVel = 0.2 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_A)) xVel = -0.2 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_S)) yVel = -0.2 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_D)) xVel = 0.2 * deltaTime;
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
		drawAnimatedSprite(animatedOrb, player.x, player.y + 64, 32, 32);

		displayFPS();
	}

	quit();
	return 0;
}