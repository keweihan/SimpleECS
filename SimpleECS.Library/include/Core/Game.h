#pragma once
#include "Core/Scene.h"
#include <vector>
#include <string>

#include "SimpleECSAPI.h"

namespace SimpleECS
{
	/**
	* A game instance represents the main game controller.
	* Sets up initialization and runs the main game loop. 
	*/
	class Game {
	public:
		// Singleton
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

		/**
		 * Singleton
		 */
		static SIMPLEECS_API Game& getInstance() {
			static Game instance;  
			return instance;
		}

		/**
		 * Configures window dimensions. Must be called before start game. 
		 *
		 */
		void SIMPLEECS_API configureWindow(int width, int height);

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

		/**
		 * Sets name of window associated with this game.
		 */
		void SIMPLEECS_API setName(std::string name);

		/**
		 * Sets name of window associated with this game.
		 */
		SIMPLEECS_API Scene* getCurrentScene();

	private:

		SIMPLEECS_API Game();
		
		std::vector<Scene*> sceneList;

		int activeSceneIndex = 0;

		std::string gameName = "";

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