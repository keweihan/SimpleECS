#pragma once
#include <SimpleECS_Core.h>

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	/**
	Provides basic physics properties to Entity such as velocity.
	If collider is present on this entity, also adds collision resolution.
	*/
	class SIMPLEECS_API PhysicsBody : public Component
	{
	public:
		PhysicsBody() : velocity() {}

		Vector velocity;

		void update() override;
		void initialize() override {};

		void onCollide(const Collider& other) override;
		void onCollide(const Collision& collide) override;
	};
}
