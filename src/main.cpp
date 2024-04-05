#include <SDL.h>

#include "../include/uriel.h"

int main(int argc, char *argv[]) {
	Uriel::init("Uriel Test");

	bool running = true;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					running = false;
				} break;
			}
		}
	}

	Uriel::quit();
	return 0;
}