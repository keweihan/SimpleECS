/*
Momentum test. Pi calculator. 

Kewei Han
*/
#include <SimpleECS.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <memory>

using namespace std;
using namespace SimpleECS;

// Environment parameters
const int SCREEN_HEIGHT = 720;
const int SCREEN_WIDTH = 1280;
const int WALL_THICKNESS = 50;

// Ball parameters
const int WEIGHT = pow(100, 2);
const int SPEED = 25;

// Globals
Scene* mainScene;

class CurrFrameCounter : public Component {
public:
	void initialize() {
		textRender = entity->getComponent<FontRenderer>();
		entity->transform->position = Vector(0, 25);
	};

	void update() {
		frameCount++;
		int currSecond = static_cast<int>(Timer::getProgramLifetime() / 1000);
		if (currSecond > prevSecond)
		{
			displayFrames = frameCount;
			frameCount = 0;
			prevSecond = currSecond;
		};

		string text = "Current FPS: " + std::to_string(displayFrames);
		textRender->text = text;
	}
	uint64_t displayFrames = 0;
	uint64_t prevSecond = 0;
	uint64_t frameCount = 0;
	Handle<FontRenderer> textRender;
};

class CollisionCounterBall : public Component {
public:
	void initialize() {};
	
	void onCollide(const Collider& other) {
		collideCount++;
	}

	void update() { }
	uint64_t collideCount = 0;
	Handle<FontRenderer> textRender;
};

class CollisionCounterText : public Component {
public:
	CollisionCounterText(Handle<CollisionCounterBall> _ballCounter) {
		ballCounter = _ballCounter;
	}

	void initialize() {
		textRender = entity->getComponent<FontRenderer>();
	};

	void update() {
		string text = "Collision Count: " + std::to_string(ballCounter->collideCount);
		textRender->text = text;
	}
	Handle<CollisionCounterBall> ballCounter;
	Handle<FontRenderer> textRender;
};

Entity* createCurrFramesCounter()
{
	Entity* counter = mainScene->createEntity();
	counter->addComponent<FontRenderer>("Default", "assets/bit9x9.ttf", 26, Color(124, 200, 211, 0xff));
	counter->addComponent<CurrFrameCounter>();
	return counter;
}	
	
// Create ball with initial position and given velocity
Entity* createBall(const int& x, const int& y, Vector vel, int side_length, double mass)
{
	Entity* newBall = mainScene->createEntity("ball");
	newBall->addComponent<RectangleRenderer>(side_length, side_length, Color(102, 102, 102, 102));
	newBall->addComponent<BoxCollider>(side_length, side_length);
	Handle<PhysicsBody> physics = newBall->addComponent<PhysicsBody>();

	// Set position
	newBall->transform->position.x = x;
	newBall->transform->position.y = y;

	physics->velocity.x = vel.x;
	physics->velocity.y = vel.y;
	physics->mass = mass;

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
}

int main() {
	try
	{
		cout << "Hello World!" << endl;

		// Create scene
		mainScene = new Scene(Color(0, 0, 0, 255));

		// Populate scene
		addBounds();

		createCurrFramesCounter();

		// Create game with scene
		Game::getInstance().configureWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
		Game::getInstance().setName("Collider Stress Test");
		Game::getInstance().addScene(mainScene);

		Entity* floor = mainScene->createEntity();
		floor->addComponent<LineRenderer>(Vector(- SCREEN_WIDTH / 2, -50), Vector(SCREEN_WIDTH / 2, -50), 3, Color(255, 255, 255, 1));

		// Create ball collide
		createBall(0, 0, { -SPEED, 0 }, 100, WEIGHT);
		Entity* left = createBall(-150, -25, { 0, 0 }, 50, 1);
		Handle<CollisionCounterBall> ballCounter = left->addComponent<CollisionCounterBall>();

		// Create Collision Counter
		Entity* counter = mainScene->createEntity();
		counter->addComponent<FontRenderer>("Default", "assets/bit9x9.ttf", 26, Color(124, 200, 211, 0xff));
		counter->addComponent<CollisionCounterText>(ballCounter);
		counter->transform->position = { 0, 75 };

		// Start game loop
		Game::getInstance().startGame();
	}
	catch (const std::exception& e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
	}
}