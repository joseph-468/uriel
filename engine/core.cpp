#include <vector>
#include <array>

#include "uriel.h"

namespace Uriel {
	extern constexpr const size_t MAX_EVENTS = 256;
	extern constexpr const SDL_Event NULL_EVENT = { .type = 0 };

	bool running = false;
	float initTime = 0;
	float deltaTime = 0;
	SDL_Window *window = nullptr;
	int windowWidth, windowHeight;
	float windowHalfWidth, windowHalfHeight;

	extern SDL_Renderer *renderer;
	extern Camera *activeCamera;

	extern size_t currentEvent;
	extern std::array<SDL_Event, MAX_EVENTS> eventQueue;
	extern std::array<Uint8, SDL_NUM_SCANCODES> currentKeyboardState;
	extern std::array<Uint8, SDL_NUM_SCANCODES> previousKeyboardState;

	void resizeViewport();

	float getCurrentTime() {
		return static_cast<float>((SDL_GetPerformanceCounter() - initTime)) * 1000 / SDL_GetPerformanceFrequency();
	}

	void updateWindowSize(int width, int height) {
		windowWidth = width;
		windowHeight = height;
		windowHalfWidth = static_cast<float>(width) / 2;
		windowHalfHeight= static_cast<float>(height) / 2;
	}

	void init(const int width, const int height, const char *title) {
		SDL_Init(SDL_INIT_EVERYTHING);
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		SDL_DisplayMode dm;
		SDL_GetDisplayMode(0, 0, &dm);
		int newWidth = (width > 0) ? width : dm.w / 2;
		int newHeight = (height > 0) ? height : dm.h / 2;
		updateWindowSize(newWidth, newHeight);

		window = SDL_CreateWindow(title,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, newWidth, newHeight,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

		renderer = SDL_CreateRenderer(window, -1, 0);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		running = true;

		initTime = SDL_GetPerformanceCounter();
	}

	void init(const int width, const int height) {
		init(width, height, "Uriel Window");
	}

	void init(const char *title) {
		init(0, 0, title);
	}

	void init() {
		init(0, 0, "Uriel Window");
	}

	void quit() {
		running = false;
		SDL_Quit();
	}

	bool tick() {
		static Uint64 currentTime = SDL_GetPerformanceCounter();
		static Uint64 previousTime = 0;

		previousKeyboardState = currentKeyboardState;

		size_t i = 0;
		eventQueue.fill(NULL_EVENT);
		while (SDL_PollEvent(&eventQueue[i++])) {}
		currentEvent = 0;
		
		SDL_Event event;
		for (size_t i = 0; i < MAX_EVENTS; i++) {
			event = eventQueue[i];
			if (event.type == 0) break;

			else if (event.type == SDL_QUIT) {
				quit();
			}
			else if (event.type == SDL_WINDOWEVENT) {
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					updateWindowSize(event.window.data1, event.window.data2);
					resizeViewport();
				}
			}
			else if (event.type == SDL_KEYDOWN) {
				currentKeyboardState[event.key.keysym.scancode] = SDL_PRESSED;
			}
			else if (event.type == SDL_KEYUP) {
				currentKeyboardState[event.key.keysym.scancode] = SDL_RELEASED;
			}
		}

		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);

		previousTime = currentTime;
		currentTime = SDL_GetPerformanceCounter();
		deltaTime = static_cast<float>(currentTime - previousTime) * 1000 / SDL_GetPerformanceFrequency();

		return running;
	}
}