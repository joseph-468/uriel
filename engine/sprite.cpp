#include "uriel.h"
#include "internal.h"

#include <iostream>
#include <vector>
#include <SDL_image.h>

namespace Uriel {
	extern SDL_Renderer *renderer;
	std::vector<SpriteSheet> spriteSheets;
	std::vector<Sprite> sprites;
	std::vector<AnimatedSprite> animatedSprites;

	Uint64 SpriteSheet::idCounter = 0;
	Uint64 Sprite::idCounter = 0;
	Uint64 AnimatedSprite::idCounter = 0;

	SpriteSheet::SpriteSheet(const std::string &filepath) : id(idCounter++), width(-1), height(-1) {
		texture = IMG_LoadTexture(renderer, filepath.c_str());
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	}

	SpriteSheet SpriteSheet::createInstance(const std::string &filepath) {
		return SpriteSheet(filepath);
	}

	Uint64 createSpriteSheet(const std::string &filepath) {
		spriteSheets.push_back(SpriteSheet::createInstance(filepath));
		return spriteSheets.size() - 1;
	}

	Sprite::Sprite(const Uint64 spriteSheetId, const SDL_Rect src) : id(idCounter++), spriteSheetId(spriteSheetId), src(src) {}

	Sprite Sprite::createInstance(const Uint64 spriteSheetId, const SDL_Rect src) {
		return Sprite(spriteSheetId, src);
	}

	Uint64 createSprite(const Uint64 spriteSheetId, const SDL_Rect src) {
		sprites.push_back(Sprite::createInstance(spriteSheetId, src));
		return sprites.size();
	}

	AnimatedSprite::AnimatedSprite(const Uint64 spriteSheetId, const SDL_Rect src, const Uint64 frameCount, const float frameRate)
		: id(idCounter++), spriteSheetId(spriteSheetId), src(src), frameCount(frameCount), frameRate(frameRate), state(AnimationState::STOPPED), playing(false), looping(false), currentFrameOffset(0), startTime(0) {}

	AnimatedSprite AnimatedSprite::createInstance(const Uint64 spriteSheetId, const SDL_Rect src, const Uint64 frameCount, const float frameRate) {
		return AnimatedSprite(spriteSheetId, src, frameCount, frameRate);
	}

	Uint64 createAnimatedSprite(const Uint64 spriteSheetId, const SDL_Rect src, const Uint64 frameCount, const float frameRate) {
		animatedSprites.push_back(AnimatedSprite::createInstance(spriteSheetId, src, frameCount, frameRate));
		return animatedSprites.size() - 1;
	}

	Uint64 AnimatedSprite::getCurrentFrame() {
		Uint64 currentOffset = static_cast<Uint64>((getCurrentTime()  - startTime) / (1000 / frameRate));
		if (currentOffset < frameCount || looping) {
			return currentOffset % frameCount;
		}
		stopAnimatedSprite(id);
		return 0;
	}

	void playAnimatedSprite(const Uint64 id, bool loop) {
		AnimatedSprite &animatedSprite = animatedSprites[id];
		animatedSprite.looping = loop;
		if (animatedSprite.state == AnimationState::PLAYING) return;
		animatedSprite.state = AnimationState::PLAYING;
		animatedSprite.playing = true;
		animatedSprite.currentFrameOffset = 0;
		animatedSprite.startTime = getCurrentTime();
	}

	void stopAnimatedSprite(const Uint64 id) {
		AnimatedSprite &animatedSprite = animatedSprites[id];
		animatedSprite.state = AnimationState::STOPPED;
		animatedSprite.playing = false;
		animatedSprite.currentFrameOffset = 0;
		animatedSprite.looping = false;
	}

	void resumeAnimatedSprite(const Uint64 id, bool loop) {
		AnimatedSprite &animatedSprite = animatedSprites[id];
		animatedSprite.looping = loop;
		if (animatedSprite.state == AnimationState::PLAYING) return;
		animatedSprite.state = AnimationState::PLAYING;
		animatedSprite.playing = true;
		animatedSprite.startTime = getCurrentTime();
	}

	void pauseAnimatedSprite(const Uint64 id) {
		AnimatedSprite &animatedSprite = animatedSprites[id];
		if (animatedSprite.state == AnimationState::PAUSED) return;
		animatedSprite.state = AnimationState::PAUSED;
		animatedSprite.playing = false;
		animatedSprite.currentFrameOffset = animatedSprite.getCurrentFrame() + animatedSprite.currentFrameOffset % animatedSprite.frameCount;
		animatedSprite.startTime = getCurrentTime();
	}
}