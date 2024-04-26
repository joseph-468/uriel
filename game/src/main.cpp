#include <uriel.h>

#include <array>
#include <iostream>

using namespace Uriel;

class Tile {
public:
	Tile() : id(0) {}
	Tile(const Uint64 id) : id(id) {}

	Uint64 id;
};

constexpr const int mapWidth = 64;
constexpr const int mapHeight = 32;

int main(int argc, char *argv[]) {
	init(1280, 720, "Uriel Test");

	Camera camera(0, 0, 1280, 720);
	setActiveCamera(camera);

	std::array<Tile, mapWidth * mapHeight> map;
	for (int y = 20; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			map[y * mapWidth + x].id = 1;
		}
	}
	for (int y = 21; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			map[y * mapWidth + x].id = 2;
		}
	}
	for (int y = 24; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			map[y * mapWidth + x].id = 3;
		}
	}

	SpriteSheet characters("assets/characters.png");
	SpriteSheet backgrounds("assets/backgrounds.png");
	SpriteSheet tiles("assets/tiles.png");

	Sprite player(characters, { 0, 0, 12, 24 });
	Sprite hillsBackground(backgrounds, { 0, 0, 1280, 720 });

	Sprite air(tiles, { 0, 0, 8, 8});
	Sprite grass(tiles, { 0, 8, 8, 8});
	Sprite dirt(tiles, { 8, 8, 8, 8});
	Sprite cobble(tiles, { 8, 0, 8, 8});

	bool controllingCamera = false;
	float walking = 0;
	float stoppingWalking = 0;
	bool walkingPlaying = false;
	Mix_Chunk *walkingSounds = Mix_LoadWAV("assets/walking.wav");
	int walkingChannel = -1;

	float playerX = 0;
	float playerY = 0;

	while (tick()) {
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
		if (controllingCamera) {
			if (keyIsDown(SDL_SCANCODE_W)) camera.y += 5 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_A)) camera.x -= 5 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_S)) camera.y -= 5 * deltaTime;
			if (keyIsDown(SDL_SCANCODE_D)) camera.x += 5 * deltaTime;
		}
		else {
			if (keyIsDown(SDL_SCANCODE_W)) playerY += deltaTime;
			if (keyIsDown(SDL_SCANCODE_A)) playerX -= deltaTime;
			if (keyIsDown(SDL_SCANCODE_S)) playerY -= deltaTime;
			if (keyIsDown(SDL_SCANCODE_D)) playerX += deltaTime;
		}

		drawSprite(hillsBackground, camera.x, camera.y, camera.width, camera.height);

		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
				int id = map[y * mapWidth + x].id;
				if (id == 0) drawSprite(air, (mapWidth / 2 - x) * 32 - 16, (mapHeight / 2 - y) * 32 + 16, 32, 32);
				if (id == 1) drawSprite(grass, (mapWidth / 2 - x) * 32 - 16, (mapHeight / 2 - y) * 32 + 16, 32, 32);
				if (id == 2) drawSprite(dirt, (mapWidth / 2 - x) * 32 - 16, (mapHeight / 2 - y) * 32 + 16, 32, 32);
				if (id == 3) drawSprite(cobble, (mapWidth / 2 - x) * 32 - 16, (mapHeight / 2 - y) * 32 + 16, 32, 32);
			}
		}

		drawSprite(player, playerX, playerY, 96, 192);
	}

	quit();
	return 0;
}