#pragma once
#include <vector>
#include "Component.h"

namespace SimpleECS
{
	class Entity {
	public:
		// Component list
		std::vector<Component*> components;
	};
}