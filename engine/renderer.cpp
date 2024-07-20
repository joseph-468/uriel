#include "uriel.h"
#include "internal.h"

#include <SDL_image.h>
#include <vector>

namespace Uriel {
	SDL_Renderer *renderer = nullptr;
	Camera *activeCamera = nullptr;
	SDL_Rect viewport;
	bool maintainAspectRatio = true;
	Uint16 customCursor = 0;

	extern std::vector<SpriteSheet> spriteSheets;
	extern std::vector<Sprite> sprites;
	extern SDL_Window *window;
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

	SDL_FRect getScreenSpaceRect(const float x, const float y, const float width, const float height) {
		float cameraScaleX = viewport.w / activeCamera->width;
		float cameraScaleY = viewport.h / activeCamera->height;
		SDL_FRect dst;
		// Round is more accurate but if extra performance is needed floor should be used.
		dst.x = round(viewport.w / 2 + ((x - activeCamera->x) * cameraScaleX));
		dst.y = round(viewport.h / 2 - ((y - activeCamera->y) * cameraScaleY) - height * cameraScaleY);
		// We use ceil as it's better to have overlapping sprites than gaps between them.
		dst.w = ceil(width * cameraScaleX);
		dst.h = ceil(height * cameraScaleY);
		return dst;
	}

	SDL_FRect getScreenSpaceRectCentered(const float x, const float y, const float width, const float height) {
		float cameraScaleX = viewport.w / activeCamera->width;
		float cameraScaleY = viewport.h / activeCamera->height;
		SDL_FRect dst;
		// Round is more accurate but if extra performance is needed floor should be used.
		dst.x = round(viewport.w / 2 + ((x - activeCamera->x) * cameraScaleX) - (width / 2) * cameraScaleX);
		dst.y = round(viewport.h / 2 - ((y - activeCamera->y) * cameraScaleY) - (height / 2) * cameraScaleY);
		// We use ceil as it's better to have overlapping sprites than gaps between them.
		dst.w = ceil(width * cameraScaleX);
		dst.h = ceil(height * cameraScaleY);
		return dst;
	}

	void drawCursor() {
		static bool cursorVisible = true;
		if (!customCursor) return;

		int windowX, windowY;
		int x, y;
		SDL_GetWindowPosition(window, &windowX, &windowY);
		SDL_GetGlobalMouseState(&x, &y);
		x -= windowX;
		y -= windowY;

		bool prevCursorVisible = cursorVisible;
		if (x < viewport.x || x >= viewport.x + viewport.w) cursorVisible = true;
		else if (y < viewport.y || y >= viewport.y + viewport.h) cursorVisible = true;
		else cursorVisible = false;
		if (cursorVisible != prevCursorVisible) SDL_ShowCursor(cursorVisible);

		x -= viewport.x;
		y -= viewport.y;
		drawSpriteInPixels(customCursor, x, y);
	}


	void drawSpriteInPixels(const Uint16 spriteId, const int x, const int y, const int width, const int height) {
		if (spriteId == 0) return;

		SDL_Rect src = sprites[spriteId - 1].src;
		SDL_FRect dst = { x, y, width, height };

		SDL_RenderCopyExF(renderer, spriteSheets[sprites[spriteId - 1].spriteSheetId].texture, &src, &dst, NULL, NULL, SDL_FLIP_NONE);
	}

	void drawSpriteInPixels(const Uint16 spriteId, const int x, const int y) {
		SDL_Rect src = sprites[spriteId - 1].src;
		drawSpriteInPixels(spriteId, x, y, src.w, src.h);
	}

	void drawAnimatedSpriteInPixels(AnimatedSprite &animatedSprite, const int x, const int y, const int width, const int height) {
		if (animatedSprite.getSpriteId() == 0) return;
		Sprite &sprite = sprites[animatedSprite.getSpriteId()  - 1];

		SDL_Rect src = sprite.src;
		Uint16 totalOffset = animatedSprite.getCurrentFrameOffset();
		if (animatedSprite.getPlaying()) {
			totalOffset += animatedSprite.getCurrentFrame();
		}
		src.x += static_cast<int>(totalOffset % sprite.frameCount * src.w);
		SDL_FRect dst = { x, y, width, height };

		SDL_RenderCopyExF(renderer, spriteSheets[sprite.spriteSheetId].texture, &src, &dst, NULL, NULL, SDL_FLIP_NONE);
	}

	void drawAnimatedSpriteInPixels(AnimatedSprite &animatedSprite, const int x, const int y) {
		if (animatedSprite.getSpriteId() == 0) return;
		Sprite &sprite = sprites[animatedSprite.getSpriteId()  - 1];

		SDL_Rect src = sprite.src;
		Uint16 totalOffset = animatedSprite.getCurrentFrameOffset();
		if (animatedSprite.getPlaying()) {
			totalOffset += animatedSprite.getCurrentFrame();
		}
		src.x += static_cast<int>(totalOffset % sprite.frameCount * src.w);
		SDL_FRect dst = { x, y, src.w, src.h };

		SDL_RenderCopyExF(renderer, spriteSheets[sprite.spriteSheetId].texture, &src, &dst, NULL, NULL, SDL_FLIP_NONE);
	}


	// Rotation implementation is only temporary.
	void drawSprite(const Uint16 spriteId, const float x, const float y, const float width, const float height, const float rotation) {
		if (spriteId == 0) return;
		SDL_Rect src = sprites[spriteId - 1].src;
		SDL_FRect dst = getScreenSpaceRectCentered(x, y, width, height);
		SDL_FPoint pivot = { dst.w / 2, dst.h };
		SDL_RenderCopyExF(renderer, spriteSheets[sprites[spriteId - 1].spriteSheetId].texture, &src, &dst, rotation, &pivot, SDL_FLIP_NONE);
	}

	void drawAnimatedSprite(AnimatedSprite &animatedSprite, const float x, const float y, const float width, const float height) {
		if (animatedSprite.getSpriteId() == 0) return;
		float cameraScaleX = viewport.w / activeCamera->width;
		float cameraScaleY = viewport.h / activeCamera->height;
		float halfWidth = width / 2;
		float halfHeight = height / 2;

		Sprite &sprite = sprites[animatedSprite.getSpriteId()  - 1];
		SDL_Rect src = sprite.src;
		Uint16 totalOffset = animatedSprite.getCurrentFrameOffset();
		if (animatedSprite.getPlaying()) {
			totalOffset += animatedSprite.getCurrentFrame();
		}
		src.x += static_cast<int>(totalOffset % sprite.frameCount * src.w);

		SDL_FRect dst = getScreenSpaceRectCentered(x, y, width, height);

		SDL_RenderCopyExF(renderer, spriteSheets[sprite.spriteSheetId].texture, &src, &dst, NULL, NULL, SDL_FLIP_NONE);
	}

	void drawFilledRectangle(const Color color, const float x, const float y, const float width, const float height) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_FRect dst = getScreenSpaceRect(x, y, width, height);
		SDL_RenderFillRectF(renderer, &dst);
	}

	void drawFilledRectangleInPixels(const Color color, const int x, const int y, const int width, const int height) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_FRect dst = { x, y, width, height };
		SDL_RenderFillRectF(renderer, &dst);
	}

	void drawRectangle(const Color color, const float x, const float y, const float width, const float height, const float thickness) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_FRect rect = getScreenSpaceRect(x, y, width, height);
		SDL_FRect dst = getScreenSpaceRect(x, y, width, height);
		float cameraScaleX = viewport.w / activeCamera->width;
		float cameraScaleY = viewport.h / activeCamera->height;
		float dstThickness = round(thickness * cameraScaleY);

		dst.h = dstThickness;
		SDL_RenderFillRectF(renderer, &dst);
		dst.y += rect.h - dstThickness;
		SDL_RenderFillRectF(renderer, &dst);
		dstThickness = round(thickness * cameraScaleX);
		dst.w = dstThickness;
		dst.h = rect.h - dstThickness * 2;
		dst.y = rect.y + dstThickness;
		SDL_RenderFillRectF(renderer, &dst);
		dst.x += rect.w - dstThickness;
		SDL_RenderFillRectF(renderer, &dst);
	}

	void drawRectangleInPixels(const Color color, const int x, const int y, const int width, const int height, const int thickness) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_FRect rect = { x, y, width, height };
		SDL_FRect dst = { x, y, width, height };
		float dstThickness = round(thickness);

		dst.h = dstThickness;
		SDL_RenderFillRectF(renderer, &dst);
		dst.y += rect.h - dstThickness;
		SDL_RenderFillRectF(renderer, &dst);
		dst.w = dstThickness;
		dst.h = rect.h - dstThickness * 2;
		dst.y = rect.y + dstThickness;
		SDL_RenderFillRectF(renderer, &dst);
		dst.x += rect.w - dstThickness;
		SDL_RenderFillRectF(renderer, &dst);
	}
}