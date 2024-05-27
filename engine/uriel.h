#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

namespace Uriel {
	struct Uint16Rect {
		Uint16 x, y;
		Uint16 w, h;
	};

	/// <summary>
	/// Temporary.
	/// </summary>
	/// <param name="text"></param>
	void renderText(const char *text);

	/// <summary>
	/// Delta time in milliseconds.
	/// This is calculated within the tick function.
	/// </summary>
	extern float deltaTime;

	/// <summary> A pointer to the camera that will be rendered to the screen.
	/// If true then if the window doesn't have the same aspect ratio as the active camera there will be black bars. 
	/// </summary>
	extern bool maintainAspectRatio;

	/// <summary>
	/// SDL_Event struct containing the most recent return value of getEvent.
	/// </summary>
	extern SDL_Event event;

	/// <summary>
	/// Initializes SDL and creates a window.
	/// </summary>
	/// <param name="width">The width of the screen in pixels. A nonpositive value defaults to half the window width.</param>
	/// <param name="height">The height of the screen in pixels. A nonpositive value defaults to half the window height.</param>
	void init(const int width, const int height, const char *title);

	/// <summary>
	/// Initializes SDL and creates a window.
	/// </summary>
	/// <param name="width">The width of the screen in pixels. A nonpositive value defaults to half the window width.</param>
	/// <param name="height">The height of the screen in pixels. A nonpositive value defaults to half the window height.</param>
	void init(const int width, const int height);

	/// <summary>
	/// Initializes SDL and creates a window.
	/// </summary>
	/// <param name="title">The window title.</param>
	void init(const char *title);

	/// <summary>
	/// Initializes SDL and creates a window.
	/// </summary>
	void init();

	/// <summary>
	/// Uninitializes SDL and closes the current window.
	/// </summary>
	void quit();

	/// <summary>
	/// General update function that should be called every frame.
	/// Updates the event queue, presents the frame buffer, and handles window related events.
	/// The main game loop should be contained in a while loop with the condition Uriel::tick().
	/// </summary>
	/// <returns>The value of Uriel::running.</returns>
	bool tick();

	/// <summary>
	/// Fetches the next event from the event queue, stores it in Uriel::event, and then removes it from the queue.
	/// </summary>
	/// <returns>True if there is a pending event, otherwise false.</returns>
	bool getEvent();

	/// <summary>
	/// Gets the number of seconds since Uriel::init() was called. This doesn't include the time taken to initialize the engine as it's computed at the end of the function.
	/// </summary>
	/// <returns>The time in milliseconds.</returns>
	float getCurrentTime();

	/// <summary>
	/// Gets the current FPS. This is updated every 500ms.
	/// </summary>
	/// <returns>The current FPS.</returns>
	Uint64 getFPS();

	/// <summary>
	/// Checks whether a key is currently held down. 
	/// </summary>
	/// <param name="key">The SDL scancode of the key to be checked.</param>
	/// <returns>True if the key is being held down.</returns>
	bool keyIsDown(const SDL_Scancode key);

	/// <summary>
	/// Checks whether a key is currently not held down. 
	/// </summary>
	/// <param name="key">The SDL scancode of the key to be checked.</param>
	/// <returns>True if the key is not being held down.</returns>
	bool keyIsUp(const SDL_Scancode key);

	/// <summary>
	/// Checks whether a key has just been pressed. 
	/// </summary>
	/// <param name="key">The SDL scancode of the key to be checked.</param>
	/// <returns>True if the key is being held down and wasn't being held down the previous tick.</returns>
	bool keyIsPressed(const SDL_Scancode key);

	/// <summary>
	/// Checks whether a key has just been released. 
	/// </summary>
	/// <param name="key">The SDL scancode of the key to be checked.</param>
	/// <returns>True if the key isn't being held down and wasn't being held down the previous tick.</returns>
	bool keyIsReleased(const SDL_Scancode key);

	/// <summary>
	/// Basic camera class used for rending.
	/// </summary>
	class Camera {
	public:
		/// <summary>
		/// Basic camera class constructor.
		/// </summary>
		/// <param name="x">The x position of the camera in game units.</param>
		/// <param name="y">The y position of the camera in game units.</param>
		/// <param name="width">The width of the camera in game units.</param>
		/// <param name="height">The height of the camera in game units.</param>
		Camera(float x, float y, float width, float height);
		
		/// <summary>
		/// Compares the ids of two cameras.
		/// </summary>
		/// <param name="otherCamera">The other camera.</param>
		/// <returns>
		/// Whether the cameras have the same ids.
		/// If two cameras have the same width, height, x, and y but have different ids then it will still return false.
		/// </returns>
		bool operator==(const Camera& otherCamera);

		/// <summary>
		/// The x position of the camera in game units.
		/// </summary>
		float x;

		/// <summary>
		/// The y position of the camera in game units.
		/// </summary>
		float y;

		/// <summary>
		/// The width of the camera in game units.
		/// </summary>
		float width;

		/// <summary>
		/// The height of the camera in game units.
		/// </summary>
		float height;

	private:
		static Uint64 idCounter;
		const Uint64 id;
	};

	/// <summary>
	/// Sets the camera that will be used by the renderer.
	/// The camera is internally a pointer to the camera so it's important that the camera doesn't go out of scope.
	/// </summary>
	/// <param name="camera">A reference to the camera.</param>
	void setActiveCamera(Camera &camera);

	/// <summary>
	/// Creates a sprite sheet and pushes it to the internal array.
	/// </summary>
	/// <param name="filepath">The path to the sprite sheet's source image.</param>
	/// <returns>The id of the sprite sheet</returns>
	Uint16 createSpriteSheet(const std::string &id, const std::string &filepath);

	Uint16 getSpriteSheetIndex(const std::string &name);

	/// <summary>
	/// Creates a sprite and pushes it to the internal array.
	/// </summary>
	/// <param name="spriteSheetId">The id of the sprite sheet which contains' the sprite's texture.</param>
	/// <param name="src">The top left originating bounds of the sprite within the spritesheet.</param>
	/// <returns>The id of the sprite.</returns>
	Uint16 createSprite(const std::string &name, const Uint16 spriteSheetId, const SDL_Rect src, const Uint16 frameCount);

	Uint16 getSpriteIndex(const std::string &name);

	enum class AnimationStatus : Uint8 {
		PLAYING,
		PAUSED,
		STOPPED,
	};

	class AnimatedSprite {
	public:
		AnimatedSprite(const Uint16 spriteId, const float frameRate);
		Uint16 getCurrentFrame();

		void play(bool loop = true);
		void stop();
		void resume(bool loop = true);
		void pause();

		float frameRate;
		float startTime;
		Uint16 currentFrameOffset;
		bool playing;
		bool looping;
		const Uint16 spriteId;
		AnimationStatus status;
	};

	/// <summary>
	/// Draws a sprite to the screen with the x and y coordinates 0, 0 being in the complete center of the world.
	/// The sprite is automatically centered. This means that a sprite at coordinates 0, 0 would be completely centered in the screen (assuming camera is also at 0, 0).
	/// </summary>
	/// <param name="spriteId">The id of the sprite that will be drawn.</param>
	/// <param name="x">The x position of the sprite in game units.</param>
	/// <param name="y">The y position of the sprite in game units.</param>
	/// <param name="width">The width of the sprite in game units.</param>
	/// <param name="height">The height of the sprite in game units.</param>
	void drawSprite(const Uint16 spriteId, const float x, const float y, const float width, const float height);

	void drawAnimatedSprite(AnimatedSprite &animationState, const float x, const float y, const float width, const float height);
}
