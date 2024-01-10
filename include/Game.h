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
		void SIMPLEECS_API startGame();

		/**
		 * Adds a scene containing entities to game instance.
		 *
		 * @param scene: Scene to be added.
		 */
		int SIMPLEECS_API addScene(Scene* scene);

	private:
		std::vector<Scene*> sceneList;

		int activeSceneIndex = 0;

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