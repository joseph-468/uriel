#include "uriel.h"

#include <array>

namespace Uriel {
	constexpr const size_t MAX_EVENTS = 256;
	constexpr const SDL_Event NULL_EVENT = { .type = 0 };

	SDL_Event event;
	size_t currentEvent;
	std::array<SDL_Event, MAX_EVENTS> eventQueue;
	std::array<Uint8, SDL_NUM_SCANCODES> currentKeyboardState;
	std::array<Uint8, SDL_NUM_SCANCODES> previousKeyboardState;

	bool getEvent() {
		if (eventQueue[currentEvent].type == SDL_POLLSENTINEL) {
			return false;
		}
		event = eventQueue[currentEvent];
		currentEvent++;
		return true;
	}

	bool keyIsDown(const SDL_Scancode key) {
		return currentKeyboardState[key] == SDL_PRESSED;
	}

	bool keyIsUp(const SDL_Scancode key) {
		return currentKeyboardState[key] == SDL_RELEASED;
	}

	bool keyIsPressed(const SDL_Scancode key) {
		return currentKeyboardState[key] == SDL_PRESSED && previousKeyboardState[key] == SDL_RELEASED;
	}

	bool keyIsReleased(const SDL_Scancode key) {
		return currentKeyboardState[key] == SDL_RELEASED && previousKeyboardState[key] == SDL_PRESSED;
	}

}