#include <iostream>
#include <SDL.h>

#include "../include/uriel.h"

int main(int argc, char *argv[]) {
	Uriel::init("Uriel Test");

	while (Uriel::tick()) {
		SDL_Event event;
		while (Uriel::getEvent(event)) {
			switch (event.type) {
				default: {
					std::cout << event.type << std::endl;
				} break;
			}
		}
	}

	Uriel::quit();
	return 0;
}