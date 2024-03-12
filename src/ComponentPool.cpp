#include "ComponentPool.h"
#include <stdexcept>

using namespace SimpleECS;

template<typename T>
void SimpleECS::ComponentPool<T>::createComponent(uint32_t entityID)
{
	if (entityID >= sparseList.size())
	{
		sparseList.resize(entityID + 1, -1);
	}

	sparseList[entityID] = componentList.size() - 1;
	componentList.push_back(T());
}

template<typename T>
void SimpleECS::ComponentPool<T>::deleteComponent(uint32_t entityID)
{
	// Check if entity has this component
	if (sparseList[entityID] == -1)
	{
		throw std::logic_error("Entity does not have component to delete.");
	}

	// Mark entity as not having this component.
	componentList[sparseList[entityID]] = NULL;
	sparseList[entityID] = -1;
}

template<typename T>
T* SimpleECS::ComponentPool<T>::getComponent(uint32_t entityID)
{
	if (sparseList[entityID] == -1)
	{
		return nullptr;
	}
	else
	{
		return componentList[sparseList[entityID]];
	}
}

template<typename T>
std::vector<T>* SimpleECS::ComponentPool<T>::getComponents()
{
	return componentList;
}
