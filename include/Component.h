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
		/*
		Called on every frame of rendering
		*/
		virtual void update() = 0;

		/*
		Called on instantiation of this component.
		*/
		virtual void initialize() = 0;

		/*
		Called on collision with another entity
		*/
		virtual void onCollide(const Collider& collide) {}

		/*
		Called on collision with another entity. Gets collision information.
		*/
		virtual void onCollide(const Collision& collide) {}

		/*
		The entity this component is attached to
		*/
		Entity* entity = nullptr;

	private:
		/*
		Called on instantiation of this component.
		*/
		void setEntity(Entity* entity);

	};
}

#endif
