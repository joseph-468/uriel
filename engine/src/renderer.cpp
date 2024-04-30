#include "../include/renderer.h"
#include "../include/camera.h"

#include <SDL_image.h>

namespace Uriel {
	extern SDL_Rect viewport;
	extern Camera *activeCamera;
	extern SDL_Renderer *renderer;
	extern int windowWidth, windowHeight;
	extern float windowHalfWidth, windowHalfHeight;

	SpriteSheet::SpriteSheet(const std::string &filepath) : width(-1), height(-1) {
		texture = IMG_LoadTexture(renderer, filepath.c_str());
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	}

	SpriteSheet::~SpriteSheet() {
		SDL_DestroyTexture(texture);
	}

	Sprite::Sprite(const SpriteSheet &spriteSheet, const SDL_Rect src) : spriteSheet(spriteSheet), src(src) {}

	void drawSprite(const Sprite &sprite, const float x, const float y, const float width, const float height) {
		SDL_Rect screenView = { 0, 0, viewport.w, viewport.h };
		float cameraScaleX = viewport.w / activeCamera->width;
		float cameraScaleY = viewport.h / activeCamera->height;
		float halfWidth = width / 2;
		float halfHeight = height / 2;
		
		SDL_Rect destination;
		// We use round for the most accurate position.
		destination.x = static_cast<int>(round(viewport.w / 2 + ((x - activeCamera->x) * cameraScaleX) - halfWidth * cameraScaleX));
		destination.y = static_cast<int>(round(viewport.h / 2 - ((y - activeCamera->y) * cameraScaleY) - halfHeight * cameraScaleY));
		// We use ceil as it's better to have overlapping sprites than gaps between them.
		destination.w = static_cast<int>(ceil(width * cameraScaleX));
		destination.h = static_cast<int>(ceil(height * cameraScaleY));

		if (SDL_HasIntersection(&screenView, &destination)) {
			SDL_RenderCopyEx(renderer, sprite.spriteSheet.texture, &sprite.src, &destination, NULL, NULL, SDL_FLIP_NONE);
		}
	}
}