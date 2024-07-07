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

bool isCollidingWithEntity(World &world, Entity &target, Entity &otherTarget) {
	SDL_FRect collisionBox1 = target.collisionBox;
	collisionBox1.x -= target.collisionBox.w / 2;
	collisionBox1.y -= target.collisionBox.h / 2;
	SDL_FRect collisionBox2 = otherTarget.collisionBox;
	collisionBox2.x -= otherTarget.collisionBox.w / 2;
	collisionBox2.y -= otherTarget.collisionBox.h / 2;
	return SDL_HasIntersectionF(&collisionBox1, &collisionBox2);
}

bool isTouchingGround(World &world, const Entity &target) {
	SDL_FRect collisionBox = target.collisionBox;
	collisionBox.y -= collisionBox.h / 2 + 4;
	collisionBox.h = 8;
	return isCollidingWithTile(world, collisionBox, 0.02);
}
