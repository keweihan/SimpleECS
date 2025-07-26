#pragma once
#include "SimpleECS.h"

#include "SimpleECSAPI.h"

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

		double mass = 1;
		Vector acceleration;

		void update() override;
		void initialize() override {};

		void applyForce(Vector direction, double magnitude);

		void onCollide(const Collider& other) override;
		void onCollide(const Collision& collide) override;

		void lateUpdate() override;

		Vector getVelocity() {
			return velocity;
		}

		void setVelocity(const Vector& v) {
			velocity = v;
			futureVelocity = v;
		}
	
	private:
		Vector velocity;
		Vector futureVelocity;
	};
}
