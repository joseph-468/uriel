#pragma once

#include <SDL.h>
#include <string>

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
}
