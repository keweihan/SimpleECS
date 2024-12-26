/*
Stress test of SimpleECS collision system. 

Kewei Han
*/
#include <SimpleECS.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <memory>
#include <cmath>

using namespace std;
using namespace SimpleECS;

// Run parameters
const bool RENDER_WINDOW = false;
const double DURATION = 10;

// Environment parameters
const int SCREEN_HEIGHT		= 720;
const int SCREEN_WIDTH		= 1280;
const int WALL_THICKNESS	= 50;

// Ball parameters
const int NUM_BALLS		= 13400;
const int MAX_SPEED		= 25;
const int MIN_SPEED		= 5;
const int SIDE_LENGTH	= 3;
const int RAND_SEED		= 42;


// Globals
Scene* mainScene;

class PerformanceLogger : public Component {
public:

	void initialize() override {
		entity->transform->position = Vector(0, -25);
	};

	void update() override {
		framesPassed++;
		int64_t lifeTime = Timer::getProgramLifetime();
		int64_t avgFPS = framesPassed / std::max(static_cast<int>(Timer::getProgramLifetime()) / 1000, 1);

		// Experiment finished. Print results
		if(Timer::getProgramLifetime() >= DURATION * 1000) {
			printPerformanceResults();
			exit(EXIT_SUCCESS);
		}

	}

	void printPerformanceResults()
	{
		cout << "```" << endl;

		double avgFps = framesPassed / DURATION;
		std::cout << "### Performance Statistics Collision Stress\n";
		std::cout << "* Total Objects: " << NUM_BALLS << "\n";
		std::cout << "* Frames Simulated: " << framesPassed << "\n";
		std::cout << "* Average FPS: " << avgFps << "\n";
		std::cout << "* Average frame time: " << DURATION / framesPassed * 1000 << "ms\n";
	}

	static uint64_t framesPassed;
};

uint64_t PerformanceLogger::framesPassed = 0;

Entity* createPerformanceLogger()
{
	Entity* counter = mainScene->createEntity();
	counter->addComponent<PerformanceLogger>();
	return counter;
}

// Create ball with initial position and inbuilt randomized velocity
Entity* createBall(const int& x, const int &y)
{
	Entity* newBall = mainScene->createEntity("ball");
	newBall->addComponent<RectangleRenderer>(SIDE_LENGTH, SIDE_LENGTH, Color(102, 102, 102, 102));
	newBall->addComponent<BoxCollider>(SIDE_LENGTH, SIDE_LENGTH);
	Handle<PhysicsBody> physics = newBall->addComponent<PhysicsBody>();

	// Set position
	newBall->transform->position.x = x;
	newBall->transform->position.y = y;

	// Randomize direction and speed
	physics->velocity.x = (MIN_SPEED + (rand() % static_cast<int>(MAX_SPEED - MIN_SPEED + 1))) * (rand() % 2 == 0 ? -1 : 1);
	physics->velocity.y = (MIN_SPEED + (rand() % static_cast<int>(MAX_SPEED - MIN_SPEED + 1))) * (rand() % 2 == 0 ? -1 : 1);

	return newBall;
}

// Create a floor/ceiling object with sound effect on collision
Entity* createFloorCeilingWall()
{
	Entity* wall = mainScene->createEntity();
	wall->addComponent<BoxCollider>(SCREEN_WIDTH + WALL_THICKNESS, WALL_THICKNESS);
	return wall;
}

// Create a side walls with sound effect and score tallying on collision
Entity* createSideWalls()
{
	Entity* wall = mainScene->createEntity();
	wall->addComponent<BoxCollider>(WALL_THICKNESS, SCREEN_HEIGHT + WALL_THICKNESS);
	return wall;
}

// Create upper and lower walls in scene, and side walls that keep score tracking
void addBounds()
{
	Entity* topBound = createFloorCeilingWall();
	topBound->transform->position.y = SCREEN_HEIGHT / 2 + WALL_THICKNESS / 2;
	
	Entity* bottomBound = createFloorCeilingWall();
	bottomBound->transform->position.y = -SCREEN_HEIGHT / 2 - WALL_THICKNESS / 2;
	
	Entity* leftBound = createSideWalls();
	leftBound->transform->position.x = -SCREEN_WIDTH / 2 - WALL_THICKNESS / 2;

	Entity* rightBound = createSideWalls();
	rightBound->transform->position.x = SCREEN_WIDTH / 2 + WALL_THICKNESS / 2;
}

// Spawn balls with physics in a grid across screen
// Returns number spawned
int spawnBalls(const int& numRow, const int& numColumn, const int& num)
{
	int rowSpacing		= (SCREEN_HEIGHT/ numRow);
	int columnSpacing	= (SCREEN_WIDTH / numColumn);

	// Center all objects
	int yOffset = (SCREEN_HEIGHT - rowSpacing * numRow)/2;
	int xOffset = (SCREEN_WIDTH - columnSpacing * numColumn)/2;

	int ySpawnPos = -SCREEN_HEIGHT/2 + rowSpacing + yOffset;
	int xSpawnPos = -SCREEN_WIDTH/2 + columnSpacing + xOffset;

	int numSpawned = 0;

	for (int i = 0; i < numRow && numSpawned < num; ++i)
	{
		for (int j = 0; j < numColumn; ++j)
		{
			Entity* newBall = createBall(xSpawnPos, ySpawnPos);
			xSpawnPos += columnSpacing;

			numSpawned++;
		}

		ySpawnPos += rowSpacing;
		xSpawnPos = -SCREEN_WIDTH / 2 + columnSpacing + xOffset;
	}
	return numSpawned;
}

int main() {
	try
	{
		srand(RAND_SEED);

		cout << "## `perfCollisionStress` World" << endl;
		cout << "```" << endl;

		// Create scene
		mainScene = new Scene(Color(0, 0, 0, 255));

		// Populate scene
		addBounds();

		// Get a grid of squares
		int columns = static_cast<int>(ceil(sqrt(NUM_BALLS / ((double)SCREEN_HEIGHT / (double)SCREEN_WIDTH))));
		int rows = static_cast<int>(ceil(NUM_BALLS / columns));
		int numSpawned = spawnBalls(rows, columns, NUM_BALLS);

		createPerformanceLogger();

		// Create game with scene
		RenderConfig config;
		config.width = SCREEN_WIDTH;
		config.height = SCREEN_HEIGHT;
		config.gameName = "Collider Stress Test";
		config.enableWindow = RENDER_WINDOW; // 

		Game::getInstance().configure(config);
		Game::getInstance().addScene(mainScene);

		// Start game loop
		Game::getInstance().startGame();
	}
	catch (const std::exception& e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
	}
}