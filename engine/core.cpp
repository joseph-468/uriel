#include <vector>
#include <array>

#include "uriel.h"
#include "internal.h"

namespace Uriel {
	constexpr size_t MAX_EVENTS = 256;
	constexpr SDL_Event NULL_EVENT = { .type = 0 };

	bool running = false;
	Uint64 initTime = 0;
	float deltaTime = 0;
	SDL_Window *window = nullptr;
	int windowWidth, windowHeight;
	float windowHalfWidth, windowHalfHeight;

	extern SDL_Renderer *renderer;
	extern Camera *activeCamera;
	extern Uint16 customCursor;

	extern size_t currentEvent;
	extern std::array<SDL_Event, MAX_EVENTS> eventQueue;
	extern std::array<Uint8, SDL_NUM_SCANCODES> currentKeyboardState;
	extern std::array<Uint8, SDL_NUM_SCANCODES> previousKeyboardState;
	extern std::array<Uint8, 8> currentMouseState;
	extern std::array<Uint8, 8> previousMouseState;

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

	int getWindowWidth() {
		return windowWidth;
	}

	int getWindowHeight() {
		return windowHeight;
	}

	void toggleFullscreen(const bool toggle) {
		SDL_SetWindowFullscreen(window, toggle ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	}

	void setWindowSize(const int width, const int height) {
		SDL_SetWindowSize(window, width, height);
		updateWindowSize(width, height);
		resizeViewport();
	}

	void setCursor(const Uint16 spriteId) {
		if (spriteId) SDL_ShowCursor(SDL_DISABLE);
		else SDL_ShowCursor(SDL_ENABLE);
		customCursor = spriteId;
	}

	Uint64 getFPS() {
		static float startTime = getCurrentTime();
		static float currentTime = 0;
		static int mostRecentUpdate = 0;
		static int frames = 0;
		static int fps = 0;

		frames++;
		currentTime = getCurrentTime();
		if ((currentTime - startTime) / 500 > mostRecentUpdate) {
			fps = frames * 2;
			frames = 0;
			mostRecentUpdate++;
		}

		return fps;
	}

	void init(const int width, const int height, const char *title) {
		SDL_Init(SDL_INIT_EVERYTHING);
		TTF_Init();
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
		previousMouseState = currentMouseState;

		size_t i = 0;
		eventQueue.fill(NULL_EVENT);
		while (SDL_PollEvent(&eventQueue[i++])) {}
		currentEvent = 0;
	
		SDL_Event event;
		for (size_t i = 0; i < MAX_EVENTS; i++) {
			event = eventQueue[i];
			switch (event.type) {
				case SDL_POLLSENTINEL: {
					goto endloop;
				} break;
				case SDL_QUIT: {
					quit();
				} break;
				case SDL_WINDOWEVENT: {
					if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
						updateWindowSize(event.window.data1, event.window.data2);
						resizeViewport();
					}
				} break;
				case SDL_KEYDOWN: {
					currentKeyboardState[event.key.keysym.scancode] = SDL_PRESSED;
				} break;
				case SDL_KEYUP: {
					currentKeyboardState[event.key.keysym.scancode] = SDL_RELEASED;
				} break;
				case SDL_MOUSEBUTTONDOWN: {
					currentMouseState[event.button.button] = SDL_PRESSED;
				} break;
				case SDL_MOUSEBUTTONUP: {
					currentMouseState[event.button.button] = SDL_RELEASED;
				} break;
			}
		}
		endloop:

		drawCursor();

		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		previousTime = currentTime;
		currentTime = SDL_GetPerformanceCounter();
		deltaTime = static_cast<float>(currentTime - previousTime) * 1000 / SDL_GetPerformanceFrequency();

		return running;
	}
}
