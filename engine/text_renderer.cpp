#include "uriel.h"
#include "internal.h"

#include <vector>

namespace Uriel {
	constexpr int FONT_INTERNAL_SIZE = 32;

	Uint16 activeFont = 0;
	std::vector<std::vector<SDL_Texture*>> fonts;

	extern SDL_Renderer *renderer;

	Uint16 loadFont(const std::string &filepath) {
		TTF_Font *font = TTF_OpenFont(filepath.c_str(), FONT_INTERNAL_SIZE);
		if (!font) {
			// Error handling across this whole module sucks. In fact, error handling across this whole project sucks.
			// Will sort that out soon.
			std::cerr << "Couldn't open font \"" << filepath << "\"" << std::endl;
			return -1;
		}

		fonts.push_back({});
		std::vector<SDL_Texture*> &characters = fonts[fonts.size() - 1];
		characters.resize(static_cast<Uint16>(-1) + 1);

		const SDL_Color white = { 255, 255, 255, 255 };
		SDL_Surface *surface;
		for (Uint16 ch = 0; ch < characters.size()-1; ch++) {
			if (TTF_GlyphIsProvided(font, ch)) {
				surface = TTF_RenderGlyph_Solid(font, ch, white);
				characters[ch] = SDL_CreateTextureFromSurface(renderer, surface);
				int w, h;
				SDL_QueryTexture(characters[ch], NULL, NULL, &w, &h);
				SDL_FreeSurface(surface);
			}
			else {
				characters[ch] = nullptr;
			}
		}

		TTF_CloseFont(font);
		return fonts.size() - 1;
	}

	void setActiveFont(const Uint16 fontId) {
		activeFont = fontId;
	}

	void renderText(const std::string &text, const int x, const int y, const int fontSize, const Color color) {
		// Doesn't support UTF-8 yet. 
		const SDL_Rect src = { 0, 0, FONT_INTERNAL_SIZE, FONT_INTERNAL_SIZE };
		SDL_Rect dst = { x, y, fontSize, fontSize };
		for (Uint16 ch : text) {
			SDL_SetTextureColorMod(fonts[activeFont][ch], color.r, color.g, color.b);
			SDL_RenderCopy(renderer, fonts[activeFont][ch], &src, &dst);
			dst.x += fontSize;
		}
	}
}
