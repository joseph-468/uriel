#include <iostream>
#include <vector>
#include <array>

#include "../include/uriel.h"

namespace Uriel {
	constexpr const size_t MAX_EVENTS = 256; // Maximum number of events which can be processed per frame. Usually only about 32 is required.
	constexpr const SDL_Event NULL_EVENT = { .type = 0 };

	bool running;
	SDL_Window *window;
	SDL_Renderer *renderer;
	int windowWidth, windowHeight;
	float windowHalfWidth, windowHalfHeight;
	double deltaTime;
	Camera *activeCamera = nullptr;

	size_t currentEvent;
	std::array<SDL_Event, MAX_EVENTS> eventQueue;
	std::array<Uint8, SDL_NUM_SCANCODES> currentKeyboardState;
	std::array<Uint8, SDL_NUM_SCANCODES> previousKeyboardState;

	void updateWindowSize(int width, int height) {
		windowWidth = width;
		windowHeight = height;
		windowHalfWidth = static_cast<float>(width) / 2;
		windowHalfHeight= static_cast<float>(height) / 2;
	}

	void init(const int width, const int height, const char *title) {
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_DisplayMode dm;
		SDL_GetDisplayMode(0, 0, &dm);
		int newWidth = (width > 0) ? width : dm.w / 2;
		int newHeight = (height > 0) ? height : dm.h / 2;
		updateWindowSize(newWidth, newHeight);

		window = SDL_CreateWindow(title,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, newWidth, newHeight,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

		renderer = SDL_CreateRenderer(window, -1, 0);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		running = true;
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
		deltaTime = static_cast<double>(currentTime - previousTime) * 1000 / SDL_GetPerformanceFrequency();

		return running;
	}

	bool getEvent(SDL_Event &eventOut) {
		if (eventQueue[currentEvent].type == SDL_POLLSENTINEL) {
			return false;
		}
		eventOut = eventQueue[currentEvent];
		currentEvent++;
		return true;
	}

	bool keyIsDown(SDL_Scancode key) {
		return currentKeyboardState[key] == SDL_PRESSED;
	}

	bool keyIsUp(SDL_Scancode key) {
		return currentKeyboardState[key] == SDL_RELEASED;
	}

	bool keyIsPressed(SDL_Scancode key) {
		return currentKeyboardState[key] == SDL_PRESSED && previousKeyboardState[key] == SDL_RELEASED;
	}

	bool keyIsReleased(SDL_Scancode key) {
		return currentKeyboardState[key] == SDL_RELEASED && previousKeyboardState[key] == SDL_PRESSED;
	}
}