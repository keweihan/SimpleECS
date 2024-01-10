#ifndef COMPONENT_H
#define COMPONENT_H


#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS 
{
	// Forward declarations
	class Entity;
	class Collider;
	class Collision;

	// Component class
	class SIMPLEECS_API Component {
		friend class Entity;

	public:
		virtual ~Component() {};

		/*
		* Called on every frame of rendering
		*/
		virtual void update() = 0;

		/*
		* Called during start of game loop.
		*/
		virtual void initialize() = 0;

		/*
		* Called on collision with another entity
		*/
		virtual void onCollide(const Collider& collide) {}

		/*
		* Called on collision with another entity. Gets collision information.
		*/
		virtual void onCollide(const Collision& collide) {}

		/*
		* The entity this component is attached to
		*/
		Entity* entity = nullptr;

	private:
		/*
		* Called on adding this component to an entity.
		*/
		void setEntity(Entity* entity);
	};
}

#endif
