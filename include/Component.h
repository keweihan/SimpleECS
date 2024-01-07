#ifndef COMPONENT_H
#define COMPONENT_H

namespace SimpleECS 
{
	// Forward declaration of Entity
	class Entity;
	class Collider;

	// Component class
	class Component {
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
