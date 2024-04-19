#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "../include/uriel.h"

int main(int argc, char *argv[]) {
	Uriel::init(1280, 720, "Uriel Test");

	Camera camera = Camera(0, 0, 16000, 9000);
	Uriel::activeCamera = &camera;

	Uriel::Sprite sprite = Uriel::Sprite("assets/texture.png");

	while (Uriel::tick()) {
		SDL_Event event;
		while (Uriel::getEvent(event)) {
			switch (event.type) {
				default: {
					//std::cout << event.type << std::endl;
				} break;
			}
		}

		if (Uriel::keyIsDown(SDL_SCANCODE_W)) camera.y += 5 * Uriel::deltaTime;
		if (Uriel::keyIsDown(SDL_SCANCODE_A)) camera.x -= 5 * Uriel::deltaTime;
		if (Uriel::keyIsDown(SDL_SCANCODE_S)) camera.y -= 5 * Uriel::deltaTime;
		if (Uriel::keyIsDown(SDL_SCANCODE_D)) camera.x += 5 * Uriel::deltaTime;

		Uriel::drawSprite(sprite, 0, 0, 1000, 1000);
	}

	Uriel::quit();
	return 0;
}