#pragma once

#include <SDL.h>
#include <string>

namespace Uriel {
	float getCurrentTime();
	void drawCursor();

	class SpriteSheet {
	public:
		static SpriteSheet createInstance(const std::string &filepath);

		static Uint16 idCounter;
		const Uint16 id;
		Sint32 width;
		Sint32 height;
		SDL_Texture *texture;

	private:
		SpriteSheet(const std::string &filepath);
	};

	class Sprite {
	public:
		static Sprite createInstance(const Uint16 spriteSheetId, const SDL_Rect src, const Uint16 frameCount);

		static Uint16 idCounter;
		const SDL_Rect src;
		const Uint16 id;
		const Uint16 spriteSheetId;
		const Uint16 frameCount;

	private:
		Sprite(const Uint16 spriteSheetId, const SDL_Rect src, const Uint16 frameCount);
	};
}
