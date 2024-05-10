#include <uriel.h>
#include <iostream>
#include <vector>

#include "../include/game.h"
#include "../include/tile.h"
#include "../include/world.h"

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

	World currentWorld = loadWorldFromFile("assets/test.map");
	//World currentWorld = generateWorld();

	Uint64 charactersSheet = createSpriteSheet("assets/characters.png");
	Uint64 backgroundsSheet = createSpriteSheet("assets/backgrounds.png");
	Uint64 tilesSheet = createSpriteSheet("assets/tiles.png");

	createSprite(tilesSheet, { 8, 0, 8, 8});
	createSprite(tilesSheet, { 0, 8, 8, 8});
	createSprite(tilesSheet, { 8, 8, 8, 8});

	Uint64 orb = createAnimatedSprite(tilesSheet, { 0, 16, 8, 8 }, 4, 3);

	Uint64 playerSprite = createSprite(charactersSheet, { 0, 0, 12, 24 });
	Uint64 backgroundSprite = createSprite(backgroundsSheet, { 0, 0, 192, 108 });

	bool controllingCamera = false;
	SDL_FRect player = { 0, 0, 32, 64};

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

		if (keyIsPressed(SDL_SCANCODE_F1)) playAnimatedSprite(orb);
		if (keyIsPressed(SDL_SCANCODE_F2)) stopAnimatedSprite(orb);
		if (keyIsPressed(SDL_SCANCODE_F3)) resumeAnimatedSprite(orb);
		if (keyIsPressed(SDL_SCANCODE_F4)) pauseAnimatedSprite(orb);

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
		drawAnimatedSprite(orb, player.x, player.y + 64, 32, 32);

		displayFPS();
	}

	quit();
	return 0;
}