#include "uriel.h"
#include "internal.h"

#include <vector>
#include <unordered_map>
#include <SDL_image.h>

namespace Uriel {
	extern SDL_Renderer *renderer;

	std::vector<SpriteSheet> spriteSheets;
	std::unordered_map<std::string, Uint16> spriteSheetsMap;
	std::vector<Sprite> sprites;
	std::unordered_map<std::string, Uint16> spritesMap;

	Uint16 SpriteSheet::idCounter = 0;
	Uint16 Sprite::idCounter = 0;

	SpriteSheet::SpriteSheet(const std::string &filepath) : id(idCounter++), width(-1), height(-1) {
		texture = IMG_LoadTexture(renderer, filepath.c_str());
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	}

	SpriteSheet SpriteSheet::createInstance(const std::string &filepath) {
		return SpriteSheet(filepath);
	}

	Uint16 createSpriteSheet(const std::string &id, const std::string &filepath) {
		spriteSheets.push_back(SpriteSheet::createInstance(filepath));
		spriteSheetsMap.insert(std::make_pair(id, spriteSheets.size() - 1));
		return spriteSheets.size() - 1;
	}

	Uint16 getSpriteSheetIndex(const std::string &name) {
		return spriteSheetsMap[name];
	}

	Sprite::Sprite(const Uint16 spriteSheetId, const SDL_Rect src, const Uint16 frameCount) : id(idCounter++), spriteSheetId(spriteSheetId), src(src), frameCount(frameCount) {}

	Sprite Sprite::createInstance(const Uint16 spriteSheetId, const SDL_Rect src, const Uint16 frameCount) {
		return Sprite(spriteSheetId, src, frameCount);
	}

	Uint16 createSprite(const std::string &name, const Uint16 spriteSheetId, const SDL_Rect src, const Uint16 frameCount) {
		sprites.push_back(Sprite::createInstance(spriteSheetId, src, frameCount));
		spritesMap.insert(std::make_pair(name, sprites.size()));
		return sprites.size();
	}

	Uint16 getSpriteIndex(const std::string &name) {
		return spritesMap[name];
	}

	AnimatedSprite::AnimatedSprite(const Uint16 spriteId, const float frameRate)
		: frameRate(frameRate), startTime(0), currentFrameOffset(0),  playing(false), looping(false), spriteId(spriteId), status(AnimationStatus::STOPPED) {}

	Uint16 AnimatedSprite::getCurrentFrame() {
		Uint16 currentOffset = static_cast<Uint16>((getCurrentTime()  - startTime) / (1000 / frameRate));
		Uint16 frameCount = sprites[spriteId - 1].frameCount;
		if (currentOffset < frameCount || looping) {
			return currentOffset % frameCount;
		}
		stop();
		return 0;
	}

	void AnimatedSprite::play(bool loop) {
		looping = loop;
		if (status == AnimationStatus::PLAYING) return;
		status = AnimationStatus::PLAYING;
		playing = true;
		currentFrameOffset = 0;
		startTime = getCurrentTime();
	}

	void AnimatedSprite::stop() {
		status = AnimationStatus::STOPPED;
		playing = false;
		currentFrameOffset = 0;
		looping = false;
	}

	void AnimatedSprite::resume(bool loop) {
		looping = loop;
		if (status == AnimationStatus::PLAYING) return;
		status = AnimationStatus::PLAYING;
		playing = true;
		startTime = getCurrentTime();
	}

	// Probably shouldn't have to pass in frameCount...
	void AnimatedSprite::pause() {
		if (status == AnimationStatus::PAUSED) return;
		Uint16 frameCount = sprites[spriteId - 1].frameCount;
		status = AnimationStatus::PAUSED;
		playing = false;
		currentFrameOffset = getCurrentFrame() + currentFrameOffset % frameCount;
		startTime = getCurrentTime();
	}
}