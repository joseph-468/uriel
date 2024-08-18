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
	renderText(text.c_str(), 0, 0, 24, { 128, 255, 128 });
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
	bool attacking = false;
	float rot = 0;
	bool down = true;
	float jumping = 0;
	float jumpTime = 0;

	int prevButton = -69;
	int prevBlockX = -6969;
	int prevBlockY = -696969;
	std::string equippedBlock = "Cobblestone";

	float mouseWorldX, mouseWorldY;
	int mouseX, mouseY;

	setCursor(getSpriteId("DefaultCursor"));

	float batTimer = 0;
	float batTimerX = -1500;
	int batX = 1;
	int batY = 1;
	double healthMax = 200;
	double health = healthMax;
	MovingEntity bat({0, TILE_SIZE, 24 * 2, 16 * 2}, getSpriteId("Bat"));
	bat.sprite.frameRate = 3;
	bat.sprite.play();

	// font test
	Uint16 font1 = loadFont("assets/fonts/bittypix.ttf");
	Uint16 font2 = loadFont("assets/fonts/ComicMono.ttf");

	Mix_Music *music = Mix_LoadMUS("assets/sounds/day.mp3");
	Mix_PlayMusic(music, -1);

	// Console
	std::string consoleText;

	while (tick()) {
		// Might make into a uriel function.
		float currentTime = getCurrentTime();

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
			case SDL_KEYDOWN: {
				if (controllingCamera) {
					consoleText += event.key.keysym.sym;
				}
			} break;
			}
		}

		SDL_GetMouseState(&mouseX, &mouseY);
		SDL_FPoint mouseWorldPos = camera.convertScreenToWorldCoords(mouseX, mouseY);
		mouseWorldX = mouseWorldPos.x;
		mouseWorldY = mouseWorldPos.y;

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

		if (isKeyPressed(SDL_SCANCODE_F1)) equippedBlock = "Log";
		if (isKeyPressed(SDL_SCANCODE_F2)) equippedBlock = "Leaves";

		if (isMouseDown(MouseButton::LEFT)) {
			attacking = true;
			int worldX = floor(currentWorld.width / 2 + mouseWorldX / TILE_SIZE);
			int worldY = floor(currentWorld.height / 2 - mouseWorldY / TILE_SIZE);
			if (worldX >= 0 && worldX < currentWorld.width && worldY >= 0 && worldY < currentWorld.height) {
				if (prevButton != MouseButton::LEFT || prevBlockX != worldX || prevBlockY != worldY) {
					currentWorld.tiles[worldX + worldY * currentWorld.width].typeId = 0;
					prevButton = MouseButton::LEFT;
					prevBlockX = worldX;
					prevBlockY = worldY;
				}
			}
		}
		if (isMouseReleased(MouseButton::LEFT)) {
			attacking = false;
		}
		if (isMouseDown(MouseButton::RIGHT)) {
			int worldX = floor(currentWorld.width / 2 + mouseWorldX / TILE_SIZE);
			int worldY = floor(currentWorld.height / 2 - mouseWorldY / TILE_SIZE);
			if (worldX >= 0 && worldX < currentWorld.width && worldY >= 0 && worldY < currentWorld.height) {
				if (prevButton != MouseButton::RIGHT || prevBlockX != worldX || prevBlockY != worldY) {
					currentWorld.tiles[worldX + worldY * currentWorld.width] = Tile(getTileTypeId(equippedBlock), getSpriteId(equippedBlock));
					currentWorld.tiles[worldX + worldY * currentWorld.width].animatedSprite.play();
					prevButton = MouseButton::RIGHT;
					prevBlockX = worldX;
					prevBlockY = worldY;
				}
			}
		}

		if (attacking) {
			rot += deltaTime;
		}
		else {
			rot = 0;
		}

		player.xVel = 0;
		player.yVel = -0.3 * deltaTime;

		if (controllingCamera) {
			if (isKeyDown(SDL_SCANCODE_W)) camera.y += 1 * deltaTime;
			if (isKeyDown(SDL_SCANCODE_A)) camera.x -= 1 * deltaTime;
			if (isKeyDown(SDL_SCANCODE_S)) camera.y -= 1 * deltaTime;
			if (isKeyDown(SDL_SCANCODE_D)) camera.x += 1 * deltaTime;
		}
		else {
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
				jumpTime = currentTime;
			}
		}
		else if (isKeyDown(SDL_SCANCODE_SPACE)) {
			if (currentTime - jumpTime < 250) {
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
		if (currentTime - batTimer > 500) {
			batTimer = currentTime;
			batY *= -1;
		}
		if (batCollided.collidedY) {
			batY *= -1;
			batTimer = currentTime;
		}
		if (batCollided.collidedX) {
			batX *= -1;
		}
		if (currentTime - batTimerX > 4000) {
			batX *= -1;
			batTimerX = currentTime;
		}

		bool collided = isCollidingWithEntity(currentWorld, player, bat);
		if (collided && attacking) {
			health -= 0.1 * deltaTime;
			if (health < 0) health = 0;
		}

		// Rendering 
		Color green = { 0, 255, 0, 255 };
		Color red = { 255, 0, 0, 255 };
		drawSprite(backgroundSprite, camera.x, camera.y, camera.width, camera.height);
		currentWorld.displayTiles(camera);

		drawSprite(player.sprite.getSpriteId(), player.collisionBox.x, player.collisionBox.y, player.collisionBox.w, player.collisionBox.h);
		drawSprite(getSpriteId("Pickaxe"), player.collisionBox.x + 32, player.collisionBox.y + 12, 40, 40, rot, { -1, -1 });
		drawRectangle(green, player.collisionBox.x - player.collisionBox.w / 2, player.collisionBox.y - player.collisionBox.h / 2, player.collisionBox.w, player.collisionBox.h, 1);

		drawAnimatedSprite(bat.sprite, bat.collisionBox.x, bat.collisionBox.y, bat.collisionBox.w, bat.collisionBox.h);
		drawRectangle(green, bat.collisionBox.x - bat.collisionBox.w / 2, bat.collisionBox.y - bat.collisionBox.h / 2, bat.collisionBox.w, bat.collisionBox.h, 1);

		// Health bar test
		float healthBarSize = health * ((TILE_SIZE * 2) / static_cast<float>(healthMax));
		drawFilledRectangle(red, bat.collisionBox.x - TILE_SIZE, bat.collisionBox.y + bat.collisionBox.h / 2 + 4, TILE_SIZE * 2, TILE_SIZE / 6);
		drawFilledRectangle(green, bat.collisionBox.x - TILE_SIZE, bat.collisionBox.y + bat.collisionBox.h / 2 + 4, healthBarSize, TILE_SIZE / 6);


		// Console test 
		Color black = { 0, 0, 0 };
		Color translucentGrey = { 128, 128, 128, 192 };
		drawFilledRectangleInPixels(translucentGrey, 0, 24, 720, 28);
		renderText(">", 2, 26, 24, black);
		renderText(consoleText, 26, 26, 24, black);

		displayFPS();
	}

	quit();
	return 0;
}