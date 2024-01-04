#pragma once
#include <vector>
#include "Scene.h"

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	/**
	* A game instance represents the main game controller.
	* Sets up initialization and runs the main game loop. 
	*/
	class Game {
	public:
		/**
		 * Game constructor. Creates an empty game.
		 *
		 */
		SIMPLEECS_API Game();

		/**
		 * Start the main game loop. Game must have at least one scene.
		 *
		 * @throws Exception: If no scenes exist in game
		 */
		SIMPLEECS_API void startGame();

		/**
		 * Adds a scene containing entities to game instance.
		 *
		 * @param scene: Scene to be added.
		 */
		SIMPLEECS_API int addScene(Scene scene);

	private:
		std::vector<Scene> sceneList;

		SDL_Window* window			= nullptr;
		SDL_Surface* screenSurface	= nullptr;

		const int SCREEN_WIDTH	= 640;
		const int SCREEN_HEIGHT = 480;

		/**
		 * Call initialization functions
		 * 
		 * @returns 0 on failure, 1 on success.
		 * @throws Exception: On failure
		 */
		void init();

		/**
		 * Constructs a window displaying current scene that closes on 'x'.
		 * Runs Component functions. 
		 */
		void mainLoop();
	};
}