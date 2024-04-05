#include "../include/uriel.h"


namespace Uriel {
	SDL_Window *window;
	SDL_Renderer *renderer;

	void init(const int width, const int height, const char *title) {
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_DisplayMode dm;
		SDL_GetDisplayMode(0, 0, &dm);
		int windowWidth = (width > 0) ? width : dm.w / 2;
		int windowHeight = (height > 0) ? height : dm.h / 2;

		Uriel::window = SDL_CreateWindow(title,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	}

	void init(const int width, const int height) {
		init(width, height, "Uriel Window");
	}

	void init(const char *title) {
		init(0, 0, title);
	}

	void init() {
		init("Uriel Window");
	}

	void quit() {
		SDL_Quit();
	}
}