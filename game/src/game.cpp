#include "../include/game.h"
#include <iostream>

void moveAndResolveCollision(World &currentWorld, SDL_FRect &target, float &xVel, float &yVel) {
	Uint64 iterations = static_cast<Uint64>(ceil(deltaTime * 4));
	if (iterations > 512) iterations = 512;
	float prevX = target.x;
	float prevY = target.y;
	bool collidedX = false;
	bool collidedY = false;
	bool collided;

	for (int i = 0; i < iterations; i++) {
		target.x += xVel / iterations;
		collided = isCollidingWithTile(currentWorld, target, 0.02f);
		if (collided) {
			target.x = prevX;
			collidedX = true;
		}
		else {
			prevX = target.x;
		}

		target.y += yVel / iterations;
		collided = isCollidingWithTile(currentWorld, target, 0.02f);
		if (collided) {
			target.y = prevY;
			collidedY = true;
		}
		else {
			prevY = target.y;
		}

		if (collidedX && collidedY) return;
	} 
}
