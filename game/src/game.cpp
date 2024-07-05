#include "../include/game.h"

CollisionResult moveAndResolveCollision(World &world, Entity &target, float &xVel, float &yVel) {
	SDL_FRect &collisionBox = target.collisionBox;
	Uint64 iterations = static_cast<Uint64>(ceil(deltaTime * 4));
	if (iterations > 512) iterations = 512;
	float prevX = collisionBox.x;
	float prevY = collisionBox.y;
	bool collidedX = false;
	bool collidedY = false;
	bool collided;

	for (int i = 0; i < iterations; i++) {
		collisionBox.x += xVel / iterations;
		collided = isCollidingWithTile(world, collisionBox, 0.02);
		if (collided) {
			collisionBox.x = prevX;
			collidedX = true;
		}
		else {
			prevX = collisionBox.x;
		}

		collisionBox.y += yVel / iterations;
		collided = isCollidingWithTile(world, collisionBox, 0.02);
		if (collided) {
			collisionBox.y = prevY;
			collidedY = true;
		}
		else {
			prevY = collisionBox.y;
		}

		if (collidedX && collidedY) return { true, true };
	} 
	return { collidedX, collidedY };
}

bool isTouchingGround(World &world, const Entity &target) {
	SDL_FRect collisionBox = target.collisionBox;
	collisionBox.y -= collisionBox.h / 2 + 4;
	collisionBox.h = 8;
	return isCollidingWithTile(world, collisionBox, 0.02);
}
