#pragma once
#include <vector>
#include "Component.h"

namespace SimpleECS
{
	class Entity {
		// Component list
		std::vector<Component> components;
	};
}