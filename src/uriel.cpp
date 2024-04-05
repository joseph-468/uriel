#include "../include/uriel.h"

namespace Uriel {
	void init() {
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_CreateWindow("Window",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 400,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	}
}