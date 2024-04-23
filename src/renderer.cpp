#include "../include/renderer.h"
#include "../include/camera.h"

#include <SDL_image.h>

namespace Uriel {
	extern SDL_Rect viewport;
	extern Camera *activeCamera;
	extern SDL_Renderer *renderer;
	extern int windowWidth, windowHeight;
	extern float windowHalfWidth, windowHalfHeight;

	Sprite::Sprite(const std::string &filepath) {
		texture = IMG_LoadTexture(renderer, filepath.c_str());
		mappingStyle = TextureMappingStyle::STRETCH;
	}

	Sprite::Sprite(const std::string &filepath, const TextureMappingStyle mappingStyle) : mappingStyle(mappingStyle) {
		texture = IMG_LoadTexture(renderer, filepath.c_str());
	}

	Sprite::~Sprite() {
		SDL_DestroyTexture(texture);
	}

	void drawSprite(const Sprite &sprite, const float x, const float y, const float width, const float height) {
		SDL_Rect screenView = { 0, 0, viewport.w, viewport.h };
		float cameraScaleX = viewport.w / activeCamera->width;
		float cameraScaleY = viewport.h / activeCamera->height;
		float halfWidth = width / 2;
		float halfHeight = height / 2;
		
		SDL_Rect destination;
		destination.x = static_cast<int>(round(viewport.w / 2 + ((x - activeCamera->x) * cameraScaleX) - halfWidth * cameraScaleX));
		destination.y = static_cast<int>(round(viewport.h / 2 - ((y - activeCamera->y) * cameraScaleY) - halfHeight * cameraScaleY));
		destination.w = static_cast<int>(round(width * cameraScaleX));
		destination.h = static_cast<int>(round(height * cameraScaleY));

		if (SDL_HasIntersection(&screenView, &destination)) {
			SDL_RenderCopy(renderer, sprite.texture, NULL, &destination);
		}
	}
}