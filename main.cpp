#include <uriel.h>

using namespace Uriel;

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 800;
constexpr Color	RECT_COLOR = { 0, 255, 0, 255 };
constexpr float RECT_WIDTH = 20;
constexpr float RECT_HEIGHT = 20;

int main(int argc, char *argv[]) {
	init(SCREEN_WIDTH, SCREEN_HEIGHT, "Uriel Window");

	Camera camera(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	setActiveCamera(camera);

	while (tick()) {
		const SDL_Point mousePos = getMousePosition();
		const SDL_FPoint squarePos = camera.convertScreenToWorldCoords(mousePos.x, mousePos.y);
		drawFilledRectangle(RECT_COLOR, squarePos.x, squarePos.y, RECT_WIDTH, RECT_HEIGHT);
	}

	quit();
	return 0;
}
