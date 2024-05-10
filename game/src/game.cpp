#include "../include/game.h"
#include <iostream>

void moveAndResolveCollision(World &currentWorld, SDL_FRect &target, float &xVel, float &yVel) {
	Uint64 iterations = static_cast<Uint64>(ceil(deltaTime * 2));
	if (iterations > 128) iterations = 128;
	float prevX = target.x;
	float prevY = target.y;
	bool collided;

	for (int i = 0; i < iterations; i++) {
		target.x += xVel / iterations;
		collided = isCollidingWithTile(currentWorld, target, 0.1f);
		if (collided) {
			target.x = prevX;
			break;
		}
		prevX = target.x;
	}

	for (int i = 0; i < iterations; i++) {
		target.y += yVel / iterations;
		collided = isCollidingWithTile(currentWorld, target, 0.1f);
		if (collided) {
			target.y = prevY;
			break;
		}
		prevY = target.y;
	}
}