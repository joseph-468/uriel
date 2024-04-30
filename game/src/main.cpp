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

	SpriteSheet characters("assets/characters.png");
	SpriteSheet backgrounds("assets/backgrounds.png");
	SpriteSheet tiles("assets/tiles.png");

	Sprite playerSprite(characters, { 0, 0, 12, 24 });
	Sprite hillsBackground(backgrounds, { 0, 0, 192, 108});

	Sprite air(tiles, { 0, 0, 8, 8});
	Sprite cobble(tiles, { 8, 0, 8, 8});
	Sprite dirt(tiles, { 0, 8, 8, 8});
	Sprite grass(tiles, { 8, 8, 8, 8});

	bool controllingCamera = false;
	float walking = 0;
	float stoppingWalking = 0;
	bool walkingPlaying = false;
	Mix_Chunk *walkingSounds = Mix_LoadWAV("assets/walking.wav");
	int walkingChannel = -1;

	bool collided;
	SDL_FRect player = { 0, 0, 32, 64 };

	while (tick()) {
		// Should make event a default member of Uriel
		SDL_Event event;
		while (getEvent(event)) {
			switch (event.type) {
				case SDL_MOUSEWHEEL: {
					if (!controllingCamera) break;
					float distance = event.wheel.y * Uriel::deltaTime * -250;
					float widthRatio = camera.width / camera.height;
					camera.width += distance * widthRatio;
					camera.height += distance;
				} break;
			}
		}

		if (keyIsDown(SDL_SCANCODE_W) || keyIsDown(SDL_SCANCODE_A) || keyIsDown(SDL_SCANCODE_S) || keyIsDown(SDL_SCANCODE_D)) {
			stoppingWalking = 0;
			if (!walkingPlaying && walking > 100) {
				walkingChannel = Mix_PlayChannel(-1, walkingSounds, -1);
				walkingPlaying = true;
			}
			walking += deltaTime;
		}
		else {
			walking = 0;
			stoppingWalking += deltaTime;
			if (stoppingWalking > 250) {
				walkingPlaying = false;
			}
		}

		if (!walkingPlaying) {
			Mix_HaltChannel(walkingChannel);
		}

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

		drawSprite(hillsBackground, camera.x, camera.y, camera.width, camera.height);

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