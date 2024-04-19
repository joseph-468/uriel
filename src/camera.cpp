#include "../include/camera.h"

Uint64 Camera::idCounter = 0;

Camera::Camera(float x, float y, float width, float height) : x(x), y(y), width(width), height(height), id(idCounter++) {}

Camera::~Camera() {}

bool Camera::operator==(const Camera &otherCamera) {
	return this->id == otherCamera.id;
}