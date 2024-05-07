#pragma once

#include <SDL.h>
#include <string>

namespace Uriel {
	float getCurrentTime();

	class SpriteSheet {
	public:
		static SpriteSheet createInstance(const std::string &filepath);

		static Uint64 idCounter;
		const Uint64 id;
		int width;
		int height;
		SDL_Texture *texture;

	private:
		SpriteSheet(const std::string &filepath);
	};

	class Sprite {
	public:
		static Sprite createInstance(const Uint64 spriteSheetId, const SDL_Rect src);

		static Uint64 idCounter;
		const Uint64 id;
		const Uint64 spriteSheetId;
		const SDL_Rect src;

	private:
		Sprite(const Uint64 spriteSheetId, const SDL_Rect src);
	};

	enum class AnimationState {
		PLAYING,
		PAUSED,
		STOPPED,
	};

	class AnimatedSprite {
	public:
		static AnimatedSprite createInstance(const Uint64 spriteSheetId, const SDL_Rect src, const Uint64 frameCount, const float frameRate);

		Uint64 getCurrentFrame();

		static Uint64 idCounter;
		const Uint64 id;
		const Uint64 spriteSheetId;
		const SDL_Rect src;
		const Uint64 frameCount;
		const float frameRate;
		AnimationState state;
		float startTime;
		Uint64 currentFrameOffset;
		bool playing;
		bool looping;

	private:
		AnimatedSprite(const Uint64 spriteSheetId, const SDL_Rect src, const Uint64 frameCount, const float frameRate);
	};
}
