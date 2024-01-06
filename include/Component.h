#ifndef COMPONENT_H
#define COMPONENT_H

namespace SimpleECS 
{
	// Forward declaration of Entity
	class Entity;

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
