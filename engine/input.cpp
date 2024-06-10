#include "uriel.h"

#include <array>

namespace Uriel {
	constexpr const size_t MAX_EVENTS = 256;
	constexpr const SDL_Event NULL_EVENT = { .type = 0 };

	size_t currentEvent;
	std::array<SDL_Event, MAX_EVENTS> eventQueue;
	std::array<Uint8, SDL_NUM_SCANCODES> currentKeyboardState;
	std::array<Uint8, SDL_NUM_SCANCODES> previousKeyboardState;
	std::array<Uint8, 8> currentMouseState;
	std::array<Uint8, 8> previousMouseState;

	bool getEvent(SDL_Event &event) {
		if (eventQueue[currentEvent].type == SDL_POLLSENTINEL) {
			return false;
		}
		event = eventQueue[currentEvent];
		currentEvent++;
		return true;
	}

	bool isKeyDown(const SDL_Scancode key) {
		return currentKeyboardState[key] == SDL_PRESSED;
	}

	bool isKeyUp(const SDL_Scancode key) {
		return currentKeyboardState[key] == SDL_RELEASED;
	}

	bool isKeyPressed(const SDL_Scancode key) {
		return currentKeyboardState[key] == SDL_PRESSED && previousKeyboardState[key] == SDL_RELEASED;
	}

	bool isKeyReleased(const SDL_Scancode key) {
		return currentKeyboardState[key] == SDL_RELEASED && previousKeyboardState[key] == SDL_PRESSED;
	}

	bool isMouseDown(const MouseButton button) {
		return currentMouseState[button] == SDL_PRESSED;
	}

	bool isMouseUp(const MouseButton button) {
		return currentMouseState[button] == SDL_RELEASED;
	}

	bool isMousePressed(const MouseButton button) {
		return currentMouseState[button] == SDL_PRESSED && previousMouseState[button] == SDL_RELEASED;
	}

	bool isMouseReleased(const MouseButton button) {
		return currentMouseState[button] == SDL_RELEASED && previousMouseState[button] == SDL_PRESSED;
	}
}