#pragma once
#include <SimpleECS_Core.h>

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
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
