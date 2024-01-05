#pragma once
#include <memory>
#include <SDL.h>

namespace SimpleECS
{
	class GameRenderer {
	public:
		/**
		 * Initializes game window and renderer
		 */
		static void initGameRenderer();

	protected:
		/**
		 * SDL Renderer access
		 */
		static const int SCREEN_WIDTH = 640;
		static const int SCREEN_HEIGHT = 480;

		static SDL_Window* window;
		static SDL_Renderer* renderer;
		static SDL_Surface* screenSurface;
	};
}