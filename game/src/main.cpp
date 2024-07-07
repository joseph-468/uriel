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
	renderText(text.c_str(), 24, { 128, 255, 128 });
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

	Uint16 backgroundSprite = getSpriteId("Sky");
	World currentWorld = generateWorld();

	bool controllingCamera = false;
	bool fullscreen = false;

	MovingEntity player({ 0, TILE_SIZE, (TILE_SIZE * 1.5) - (TILE_SIZE * 1.5 / 8), (TILE_SIZE * 3) - (TILE_SIZE * 3 / 8) }, getSpriteId("Player"));
	int healthMax = 200;
	int health = healthMax;
	float jumping = 0;
	float playerLastHit = 0;
	float jumpTime = 0;

	int prevButton = -69;
	int prevBlockX = -6969;
	int prevBlockY = -696969;
	std::string equippedBlock = "Cobblestone";

	float mouseWorldX, mouseWorldY;
	int mouseX, mouseY;

	setCursor(getSpriteId("DefaultCursor"));

	float batTimer = 0;
	int batX = 1;
	int batY = 1;
	MovingEntity bat({0, TILE_SIZE, 24 * 2, 16 * 2}, getSpriteId("Bat"));
	bat.sprite.frameRate = 3;
	bat.sprite.play();


	// font test
	Uint16 font1 = loadFont("assets/fonts/bittypix.ttf");
	Uint16 font2 = loadFont("assets/fonts/ComicMono.ttf");

	while (tick()) {
		// Input
		SDL_Event event;
		while (getEvent(event)) {
			switch (event.type) {
			case SDL_MOUSEWHEEL: {
				if (controllingCamera) {
					float distance = event.wheel.y * -100;
					float widthRatio = camera.width / camera.height;
					camera.width += distance * widthRatio;
					camera.height += distance;
				}
				else {
					health += healthMax / 100 * event.wheel.y * 2;
					if (health < 0) health = 0;
					if (health > healthMax) health = healthMax;
				}
			} break;
			case SDL_MOUSEBUTTONDOWN: {
			} break;
			}
		}

		{
			SDL_GetMouseState(&mouseX, &mouseY);
			SDL_FPoint temp = camera.convertScreenToWorldCoords(mouseX, mouseY);
			mouseWorldX = temp.x;
			mouseWorldY = temp.y;
		}

		if (isKeyPressed(SDL_SCANCODE_F11)) {
			fullscreen = fullscreen ? false : true;
			toggleFullscreen(fullscreen);
		}

		// Logic
		if (isKeyPressed(SDL_SCANCODE_ESCAPE)) {
			if (controllingCamera) {
				controllingCamera = false;
			}
			else {
				controllingCamera = true;
			}
		}

		if (isKeyPressed(SDL_SCANCODE_F1)) equippedBlock = "Cobblestone";
		if (isKeyPressed(SDL_SCANCODE_F2)) equippedBlock = "Dirt";

		if (isMouseDown(MouseButton::LEFT)) {
			int worldX = currentWorld.width - floor(currentWorld.width / 2 - mouseWorldX / TILE_SIZE) - 1;
			int worldY = floor(currentWorld.height / 2 - mouseWorldY / TILE_SIZE);
			if (prevButton != MouseButton::LEFT || prevBlockX != worldX || prevBlockY != worldY) {
				currentWorld.tiles[worldX + worldY * currentWorld.width].typeId = 0;
				prevButton = MouseButton::LEFT;
				prevBlockX = worldX;
				prevBlockY = worldY;
			}
		}
		if (isMousePressed(MouseButton::RIGHT)) {
			int worldX = currentWorld.width - floor(currentWorld.width / 2 - mouseWorldX / TILE_SIZE) - 1;
			int worldY = floor(currentWorld.height / 2 - mouseWorldY / TILE_SIZE);
			if (prevButton != MouseButton::RIGHT || prevBlockX != worldX || prevBlockY != worldY) {
				currentWorld.tiles[worldX + worldY * currentWorld.width] = Tile(getTileTypeId(equippedBlock), getSpriteId(equippedBlock));
				currentWorld.tiles[worldX + worldY * currentWorld.width].animatedSprite.play();
				prevButton = MouseButton::RIGHT;
				prevBlockX = worldX;
				prevBlockY = worldY;
			}
		}

		player.xVel = 0;
		player.yVel = 0;
		if (controllingCamera) {
			if (isKeyDown(SDL_SCANCODE_W)) camera.y += 1 * deltaTime;
			if (isKeyDown(SDL_SCANCODE_A)) camera.x -= 1 * deltaTime;
			if (isKeyDown(SDL_SCANCODE_S)) camera.y -= 1 * deltaTime;
			if (isKeyDown(SDL_SCANCODE_D)) camera.x += 1 * deltaTime;
		}
		else {
			player.yVel = -0.3 * deltaTime;
			if (isKeyDown(SDL_SCANCODE_A)) {
				player.xVel = -0.1 * deltaTime;
			}
			if (isKeyDown(SDL_SCANCODE_D)) {
				player.xVel = 0.1 * deltaTime;
			}
		}

		if (jumping > 0) jumping -= 0.01 * deltaTime;
		if (isKeyPressed(SDL_SCANCODE_SPACE)) {
			if (isTouchingGround(currentWorld, player)) {
				jumping = 1;
				jumpTime = getCurrentTime();
			}
		}
		else if (isKeyDown(SDL_SCANCODE_SPACE)) {
			if (getCurrentTime() - jumpTime < 250) {
				jumping += 0.01 * deltaTime;
			}
		}
		if (jumping > 0) {
			player.yVel = 0.3 * deltaTime;
		}

		moveAndResolveCollision(currentWorld, player, player.xVel, player.yVel);

		if (!controllingCamera) {
			camera.x = player.collisionBox.x;
			camera.y = player.collisionBox.y;
		}


		// Bat test
		bat.xVel = batX * deltaTime * 0.2;
		bat.yVel = batY * deltaTime * 0.2;
		CollisionResult batCollided = moveAndResolveCollision(currentWorld, bat, bat.xVel, bat.yVel);
		float currTime = getCurrentTime();
		if (currTime - batTimer > 500) {
			batTimer = currTime;
			batY *= -1;
		}
		if (batCollided.collidedY) {
			batY *= -1;
			batTimer = currTime;
		}
		if (batCollided.collidedX) {
			batX *= -1;
		}

		bool collided = isCollidingWithEntity(currentWorld, player, bat);
		if (collided && getCurrentTime() - playerLastHit > 200) {
			health -= 10;
			if (health < 0) health = 0;
			playerLastHit = getCurrentTime();
		}

		// Rendering 
		Color green = { 0, 255, 0, 255 };
		Color red = { 255, 0, 0, 255 };
		drawSprite(backgroundSprite, camera.x, camera.y, camera.width, camera.height);
		currentWorld.displayTiles(camera);

		//drawRectangle(col, player.collisionBox.x - player.collisionBox.w / 2, player.collisionBox.y - player.collisionBox.h / 2, player.collisionBox.w, player.collisionBox.h);
		drawSprite(player.sprite.getSpriteId(), player.collisionBox.x, player.collisionBox.y, player.collisionBox.w, player.collisionBox.h);
		//drawRectangle(col, bat.collisionBox.x - bat.collisionBox.w / 2, bat.collisionBox.y - bat.collisionBox.h / 2, bat.collisionBox.w, bat.collisionBox.h);
		drawAnimatedSprite(bat.sprite, bat.collisionBox.x, bat.collisionBox.y, bat.collisionBox.w, bat.collisionBox.h);

		// Health bar test
		drawRectangle(red, player.collisionBox.x - TILE_SIZE, player.collisionBox.y + player.collisionBox.h - TILE_SIZE * 1.25, TILE_SIZE * 2, TILE_SIZE / 6);
		float healthBarSize = health * ((TILE_SIZE * 2) / static_cast<float>(healthMax));
		drawRectangle(green, player.collisionBox.x - TILE_SIZE, player.collisionBox.y + player.collisionBox.h - TILE_SIZE * 1.25, healthBarSize, TILE_SIZE / 6);

		displayFPS();
	}

	quit();
	return 0;
}