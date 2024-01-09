#pragma once
#include "Vector.h"
namespace SimpleECS
{
	class Transform
	{
	public:
		/**
		 * Position of this entity in world space
		 */
		Vector position = Vector(0, 0);

		/**
		 * TODO: Rotation of transform 
		 */
		//double rotation = 0;
	};
}