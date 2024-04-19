#pragma once

#include <SDL.h>

#include "uriel.h"

namespace Uriel {
	enum class TextureMappingStyle {
		STRETCH,
		CROP,
		REPEAT,
	};

	class Sprite {
	public:
		Sprite() = delete;
		Sprite(const std::string &filepath);
		Sprite(const std::string &filepath, const TextureMappingStyle mappingStyle);
		~Sprite();

		SDL_Texture *texture;
		TextureMappingStyle mappingStyle;
	};

	void drawSprite(const Sprite &sprite, const float x, const float y, const float width, const float height);
}
