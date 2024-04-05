#pragma once

#include <SDL.h>
namespace Uriel {
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
}
