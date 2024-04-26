#pragma once

#include <SDL.h>
#include <string>

namespace Uriel {
	class SpriteSheet {
	public:
		SpriteSheet() = delete;
		SpriteSheet(const std::string &filepath);
		~SpriteSheet();

		SDL_Texture *texture;
		int width, height;
	};

	class Sprite {
	public:
		Sprite(const SpriteSheet &spriteSheet, const SDL_Rect src);

		const SpriteSheet &spriteSheet;
		const SDL_Rect src;
	};
}
