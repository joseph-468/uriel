#pragma once

#include <uriel.h>

#include "world.h"

constexpr const Sint64 TILE_SIZE = 32;

struct CollisionResult {
	bool collidedX;
	bool collidedY;
};

class Entity {
public:
	Entity(const SDL_FRect collisionBox, const Uint16 spriteId) : collisionBox(collisionBox), sprite(spriteId, 0) {}

	SDL_FRect collisionBox;
	AnimatedSprite sprite;
};

class MovingEntity : public Entity {
public:
	MovingEntity(const SDL_FRect collisionBox, const Uint16 spriteId) : Entity(collisionBox, spriteId), xVel(0), yVel(0) {}

	float xVel;
	float yVel;
};


CollisionResult moveAndResolveCollision(World &world, Entity &target, float &xVel, float &yVel);
bool isTouchingGround(World &world, const Entity &entity);
