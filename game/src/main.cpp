#include <uriel.h>
#include <iostream>

#include "../include/tile.h"
#include "../include/world.h"

using namespace Uriel;

int main(int argc, char *argv[]) {
	init(1280, 720, "Uriel Test");

	Camera camera(0, 0, 1280, 720);
	setActiveCamera(camera);

	World currentWorld = loadWorldFromFile("assets/test.map");

	Uint64 charactersSheet = createSpriteSheet("assets/characters.png");
	Uint64 backgroundsSheet = createSpriteSheet("assets/backgrounds.png");
	Uint64 tilesSheet = createSpriteSheet("assets/tiles.png");

	createSprite(tilesSheet, { 8, 0, 8, 8});
	createSprite(tilesSheet, { 0, 8, 8, 8});
	createSprite(tilesSheet, { 8, 8, 8, 8});

	Uint64 playerSprite = createSprite(charactersSheet, { 0, 0, 12, 24 });
	Uint64 backgroundSprite = createSprite(backgroundsSheet, { 0, 0, 192, 108 });

	bool controllingCamera = false;
	bool collided;
	SDL_FRect player = { 0, 0, 32, 64 };

	while (tick()) {
		// Input
		while (getEvent()) {
			switch (event.type) {
				case SDL_MOUSEWHEEL: {
					if (!controllingCamera) break;
					float distance = event.wheel.y * deltaTime * -250;
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

		float prevX = player.x;
		float prevY = player.y;
		if (controllingCamera) {
			if (keyIsDown(SDL_SCANCODE_W)) camera.y += 1 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_A)) camera.x -= 1 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_S)) camera.y -= 1 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_D)) camera.x += 1 * deltaTime;
		}
		else {
			if (keyIsDown(SDL_SCANCODE_W)) player.y += 0.4 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_A)) player.x -= 0.4 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_S)) player.y -= 0.4 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_D)) player.x += 0.4 * deltaTime;
		}

		collided = isCollidingWithTile(currentWorld, player, 0.1);
		if (collided) {
			player.x = prevX;
			player.y = prevY;
		}

		// Rendering 
		drawSprite(backgroundSprite, camera.x, camera.y, camera.width, camera.height);

		for (int y = 0; y < currentWorld.height; y++) {
			for (int x = 0; x < currentWorld.width; x++) {
				currentWorld.displayTile(x, y);
			}
		}

		drawSprite(playerSprite, player.x, player.y, player.w, player.h);
	}

	quit();
	return 0;
}