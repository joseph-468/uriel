#include "uriel.h"
#include "internal.h"

#include <SDL_image.h>
#include <vector>

namespace Uriel {
	SDL_Renderer *renderer = nullptr;
	Camera *activeCamera = nullptr;
	SDL_Rect viewport;
	bool maintainAspectRatio = true;

	extern std::vector<SpriteSheet> spriteSheets;
	extern std::vector<Sprite> sprites;
	extern std::vector<AnimatedSprite> animatedSprites;
	extern int windowWidth, windowHeight;
	extern float windowHalfWidth, windowHalfHeight;

	Uint64 Camera::idCounter = 0;

	Camera::Camera(float x, float y, float width, float height) : x(x), y(y), width(width), height(height), id(idCounter++) {}

	bool Camera::operator==(const Camera &otherCamera) {
		return this->id == otherCamera.id;
	}

	void resizeViewport() {
		float windowWidthRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
		float cameraWidthRatio = activeCamera->width / activeCamera->height;
		float widthRatio = windowWidthRatio / cameraWidthRatio;
		float heightRatio = 1 / widthRatio;

		viewport.w = windowWidth;
		viewport.h = windowHeight;
		viewport.x = 0;
		viewport.y = 0;
		if (widthRatio > 1) {
			viewport.w = static_cast<int>(windowWidth / widthRatio);
			viewport.x = (windowWidth - viewport.w) / 2;
		}
		else if (heightRatio > 1) {
			viewport.h = static_cast<int>(windowHeight / heightRatio);
			viewport.y = (windowHeight - viewport.h) / 2;
		}

		SDL_RenderSetViewport(renderer, &viewport);
	}

	void setActiveCamera(Camera &camera) {
		activeCamera = &camera;
		resizeViewport();
	}

	void drawSprite(const Uint64 spriteId, const float x, const float y, const float width, const float height) {
		if (spriteId == 0) return;
		SDL_Rect screenView = { 0, 0, viewport.w, viewport.h };
		float cameraScaleX = viewport.w / activeCamera->width;
		float cameraScaleY = viewport.h / activeCamera->height;
		float halfWidth = width / 2;
		float halfHeight = height / 2;

		SDL_Rect src = sprites[spriteId - 1].src;
		SDL_Rect destination;
		// We use round for the most accurate position.
		destination.x = static_cast<int>(round(viewport.w / 2 + ((x - activeCamera->x) * cameraScaleX) - halfWidth * cameraScaleX));
		destination.y = static_cast<int>(round(viewport.h / 2 - ((y - activeCamera->y) * cameraScaleY) - halfHeight * cameraScaleY));
		// We use ceil as it's better to have overlapping sprites than gaps between them.
		destination.w = static_cast<int>(ceil(width * cameraScaleX));
		destination.h = static_cast<int>(ceil(height * cameraScaleY));

		if (SDL_HasIntersection(&screenView, &destination)) {
			SDL_RenderCopyEx(renderer, spriteSheets[sprites[spriteId - 1].spriteSheetId].texture, &src, &destination, NULL, NULL, SDL_FLIP_NONE);
		}
	}


	void drawAnimatedSprite(const Uint64 animatedSpriteId, const float x, const float y, const float width, const float height) {
		SDL_Rect screenView = { 0, 0, viewport.w, viewport.h };
		float cameraScaleX = viewport.w / activeCamera->width;
		float cameraScaleY = viewport.h / activeCamera->height;
		float halfWidth = width / 2;
		float halfHeight = height / 2;

		AnimatedSprite &animatedSprite = animatedSprites[animatedSpriteId];
		SDL_Rect src = animatedSprite.src;
		Uint64 totalOffset = animatedSprite.currentFrameOffset;
		if (animatedSprite.playing) {
			totalOffset += animatedSprite.getCurrentFrame();
		}
		src.x += totalOffset % animatedSprite.frameCount * src.w;

		SDL_Rect destination;
		// We use round for the most accurate position.
		destination.x = static_cast<int>(round(viewport.w / 2 + ((x - activeCamera->x) * cameraScaleX) - halfWidth * cameraScaleX));
		destination.y = static_cast<int>(round(viewport.h / 2 - ((y - activeCamera->y) * cameraScaleY) - halfHeight * cameraScaleY));
		// We use ceil as it's better to have overlapping sprites than gaps between them.
		destination.w = static_cast<int>(ceil(width * cameraScaleX));
		destination.h = static_cast<int>(ceil(height * cameraScaleY));

		if (SDL_HasIntersection(&screenView, &destination)) {
			SDL_RenderCopyEx(renderer, spriteSheets[animatedSprites[animatedSpriteId].spriteSheetId].texture, &src, &destination, NULL, NULL, SDL_FLIP_NONE);
		}
	}
}