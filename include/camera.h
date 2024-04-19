#pragma once

#include <SDL.h>

class Camera {
public:
	Camera(float x, float y, float width, float height);
	~Camera();
	bool operator==(const Camera& otherCamera);

	static Uint64 idCounter;
	Uint64 id;
	float x, y;
	float width, height;
};
