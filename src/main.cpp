#include "../include/uriel.h"

int main(int argc, char *argv[]) {
	Uriel::init(1280, 720, "Uriel Test");

	Uriel::Camera camera = Uriel::Camera(0, 0, 1280, 720);
	Uriel::setActiveCamera(camera);

	Uriel::Sprite player = Uriel::Sprite("assets/human.png");
	Uriel::Sprite background = Uriel::Sprite("assets/background.png");

	bool controllingCamera = false;

	while (Uriel::tick()) {
		SDL_Event event;
		while (Uriel::getEvent(event)) {
			switch (event.type) {
				case SDL_MOUSEWHEEL: {
					if (!controllingCamera) break;
					float distance = event.wheel.y * Uriel::deltaTime * 250;
					float widthRatio = camera.width / camera.height;
					camera.width += distance * widthRatio;
					camera.height += distance;
				} break;
			}
		}

		if (Uriel::keyIsPressed(SDL_SCANCODE_ESCAPE)) {
			if (controllingCamera) {
				controllingCamera = false;
			}
			else {
				controllingCamera = true;
			}
		}
		if (controllingCamera) {
			if (Uriel::keyIsDown(SDL_SCANCODE_W)) camera.y += 5 * Uriel::deltaTime;
			if (Uriel::keyIsDown(SDL_SCANCODE_A)) camera.x -= 5 * Uriel::deltaTime;
			if (Uriel::keyIsDown(SDL_SCANCODE_S)) camera.y -= 5 * Uriel::deltaTime;
			if (Uriel::keyIsDown(SDL_SCANCODE_D)) camera.x += 5 * Uriel::deltaTime;
		}

		Uriel::drawSprite(background, camera.x, camera.y, camera.width, camera.height);
		Uriel::drawSprite(player, 0, 0, 120, 240);
	}

	Uriel::quit();
	return 0;
}