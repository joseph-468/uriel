#pragma once

#include <SDL.h>

namespace Uriel {
	class Camera {
	public:
		Camera(float x, float y, float width, float height);
		~Camera();
		bool operator==(const Camera& otherCamera);

		const Uint64 id;
		float x, y;
		float width, height;

	private:
		static Uint64 idCounter;
	};
}
