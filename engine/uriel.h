#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>

namespace Uriel {
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
	/// Draws a sprite to the screen with the x and y coordinates 0, 0 being in the complete center of the world.
	/// The sprite is automatically centered. This means that a sprite at coordinates 0, 0 would be completely centered in the screen (assuming camera is also at 0, 0).
	/// </summary>
	/// <param name="sprite">A reference to the sprite that will be drawn.</param>
	/// <param name="x">The x position of the sprite in game units.</param>
	/// <param name="y">The y position of the sprite in game units.</param>
	/// <param name="width">The width of the sprite in game units.</param>
	/// <param name="height">The height of the sprite in game units.</param>
	void drawSprite(const Uint64 spriteId, const float x, const float y, const float width, const float height);

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
	/// Basically a wrapper for SDL_Texture that the Sprite class can be linked to.
	/// </summary>
	class SpriteSheet {
	public:
		/// <summary>
		/// Basic constructor for spritesheet. Should probably not be accessible to the user.
		/// </summary>
		/// <param name="filepath">Path to the spritesheet's source image.</param>
		SpriteSheet(const std::string &filepath);

		/// <summary>
		/// An auto incrementing id. Should probably be made private.
		/// </summary>
		static Uint64 idCounter;

		/// <summary>
		/// The unique id of the sprite sheet.
		/// </summary>
		const Uint64 id;

		/// <summary>
		/// The width of the sprite sheet in pixels.
		/// </summary>
		int width;

		/// <summary>
		/// The height of the sprite sheet in pixels.
		/// </summary>
		int height;

		/// <summary>
		/// The sprite sheet's texture. Used internally in rendering code but should probably be private.
		/// </summary>
		SDL_Texture *texture;
	};

	/// <summary>
	/// Creates a sprite sheet and pushes it to the internal array.
	/// </summary>
	/// <param name="filepath">The path to the sprite sheet's source image.</param>
	/// <returns>The id of the sprite sheet</returns>
	Uint64 createSpriteSheet(const std::string &filepath);

	/// <summary>
	/// A sprite defined as a width, height, x, and y within a spritesheet.
	/// </summary>
	class Sprite {
	public:
		/// <summary>
		/// Basic constructor. Should probably not be accessible to the user.
		/// </summary>
		/// <param name="spriteSheetId">The id of the spritesheet that contains the sprite's texture.</param>
		/// <param name="src">The top left originating rectangle that defines the texture.</param>
		Sprite(const Uint64 spriteSheetId, const SDL_Rect src);

		/// <summary>
		/// An auto incrementing id. Should probably be made private.
		/// </summary>
		static Uint64 idCounter;

		/// <summary>
		/// The unique id of the sprite.
		/// </summary>
		const Uint64 id;

		/// <summary>
		/// The id of the sprite sheet that contains the sprite's texture.
		/// </summary>
		const Uint64 spriteSheetId;

		/// <summary>
		/// The top left originating bounds of the sprite within the spritesheet. 
		/// </summary>
		const SDL_Rect src;
	};

	/// <summary>
	/// Creates a sprite and pushes it to the internal array.
	/// </summary>
	/// <param name="spriteSheetId">The id of the sprite sheet which contains' the sprite's texture.</param>
	/// <param name="src">The top left originating bounds of the sprite within the spritesheet.</param>
	/// <returns>The id of the sprite.</returns>
	Uint64 createSprite(const Uint64 spriteSheetId, const SDL_Rect src);
}
