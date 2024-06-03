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
	extern int windowWidth, windowHeight;
	extern float windowHalfWidth, windowHalfHeight;

	Uint64 Camera::idCounter = 0;

	Camera::Camera(float x, float y, float width, float height) : x(x), y(y), width(width), height(height), id(idCounter++) {}

	bool Camera::operator==(const Camera &otherCamera) const {
		return this->id == otherCamera.id;
	}

	SDL_FPoint Camera::convertScreenToWorldCoords(const int x, const int y) const {
		int viewportX = x - viewport.x;
		int viewportY = y - viewport.y;
		float widthRatio = this->width / viewport.w;
		float heightRatio = this->height / viewport.h;
		float relativeX = viewportX * widthRatio - width / 2 + this->x;
		float relativeY = (viewportY * heightRatio - height / 2 - this->y) * -1;
		return { relativeX, relativeY };
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

	void drawSprite(const Uint16 spriteId, const float x, const float y, const float width, const float height) {
		if (spriteId == 0) return;
		SDL_Rect screenView = { 0, 0, viewport.w, viewport.h };
		float cameraScaleX = viewport.w / activeCamera->width;
		float cameraScaleY = viewport.h / activeCamera->height;
		float halfWidth = width / 2;
		float halfHeight = height / 2;

		SDL_Rect src = sprites[spriteId - 1].src;
		SDL_FRect destination;
		// Round is more accurate but if extra performance is needed floor should be used.
		destination.x = round(viewport.w / 2 + ((x - activeCamera->x) * cameraScaleX) - halfWidth * cameraScaleX);
		destination.y = round(viewport.h / 2 - ((y - activeCamera->y) * cameraScaleY) - halfHeight * cameraScaleY);
		// We use ceil as it's better to have overlapping sprites than gaps between them.
		destination.w = ceil(width * cameraScaleX);
		destination.h = ceil(height * cameraScaleY);

		SDL_RenderCopyExF(renderer, spriteSheets[sprites[spriteId - 1].spriteSheetId].texture, &src, &destination, NULL, NULL, SDL_FLIP_NONE);
	}

	void drawAnimatedSprite(AnimatedSprite &animatedSprite, const float x, const float y, const float width, const float height) {
		if (animatedSprite.spriteId == 0) return;
		SDL_Rect screenView = { 0, 0, viewport.w, viewport.h };
		float cameraScaleX = viewport.w / activeCamera->width;
		float cameraScaleY = viewport.h / activeCamera->height;
		float halfWidth = width / 2;
		float halfHeight = height / 2;

		Sprite &sprite = sprites[animatedSprite.spriteId - 1];
		SDL_Rect src = sprite.src;
		Uint16 totalOffset = animatedSprite.currentFrameOffset;
		if (animatedSprite.playing) {
			totalOffset += animatedSprite.getCurrentFrame();
		}
		src.x += static_cast<int>(totalOffset % sprite.frameCount * src.w);

		SDL_FRect destination;
		// Round is more accurate but if extra performance is needed floor should be used.
		destination.x = round(viewport.w / 2 + ((x - activeCamera->x) * cameraScaleX) - halfWidth * cameraScaleX);
		destination.y = round(viewport.h / 2 - ((y - activeCamera->y) * cameraScaleY) - halfHeight * cameraScaleY);
		// We use ceil as it's better to have overlapping sprites than gaps between them.
		destination.w = ceil(width * cameraScaleX);
		destination.h = ceil(height * cameraScaleY);

		SDL_RenderCopyExF(renderer, spriteSheets[sprite.spriteSheetId].texture, &src, &destination, NULL, NULL, SDL_FLIP_NONE);
	}

	// Temporary
	void renderText(const char *text) {
		static TTF_Font* font = TTF_OpenFont("assets/fonts/bittypix.ttf", 24);
		static SDL_Color fontColor = { 128, 128, 128 };
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, fontColor);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_Rect src = { 0, 0, 0, 0 };
		SDL_QueryTexture(textTexture, NULL, NULL, &src.w, &src.h);
		SDL_RenderCopy(renderer, textTexture, &src, &src);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
	}
}