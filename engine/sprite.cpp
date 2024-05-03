#include "uriel.h"

#include <vector>
#include <SDL_image.h>

namespace Uriel {
	extern SDL_Renderer *renderer;
	std::vector<SpriteSheet> spriteSheets;
	std::vector<Sprite> sprites;

	Uint64 SpriteSheet::idCounter = 0;
	Uint64 Sprite::idCounter = 0;

	Uint64 createSpriteSheet(const std::string &filepath) {
		spriteSheets.emplace_back(filepath);
		return SpriteSheet::idCounter-1;
	}

	Uint64 createSprite(const Uint64 spriteSheetId, const SDL_Rect src) {
		sprites.emplace_back(spriteSheetId, src);
		return Sprite::idCounter;
	}

	SpriteSheet::SpriteSheet(const std::string &filepath) : id(idCounter++), width(-1), height(-1) {
		texture = IMG_LoadTexture(renderer, filepath.c_str());
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	}

	Sprite::Sprite(const Uint64 spriteSheetId, const SDL_Rect src) : id(idCounter++), spriteSheetId(spriteSheetId), src(src) {}
}