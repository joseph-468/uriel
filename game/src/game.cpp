#include "../include/game.h"

void moveAndResolveCollision(World &currentWorld, SDL_FRect &target, float &xVel, float &yVel) {
	int iterations = ceil(deltaTime * 2);
	if (iterations > 128) iterations = 128;
	float prevX = target.x;
	float prevY = target.y;
	bool collided;

	for (int i = 0; i < iterations; i++) {
		target.x += xVel / iterations;
		collided = isCollidingWithTile(currentWorld, target, 0.1);
		if (collided) {
			target.x = prevX;
			break;
		}
		prevX = target.x;
	}

	for (int i = 0; i < iterations; i++) {
		target.y += yVel / iterations;
		collided = isCollidingWithTile(currentWorld, target, 0.1);
		if (collided) {
			target.y = prevY;
			break;
		}
		prevY = target.y;
	}
}