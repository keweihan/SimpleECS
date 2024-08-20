#pragma once

#include "SimpleECSAPI.h"

#include "Core/ComponentPool.h"
#include <cstdint>

namespace SimpleECS
{
	class Entity;

	/**
	 * Handler wrapper class for components
	 * Provides stable pointer semantics for a given component type 
	 * and associated entity.
	 */
	template <typename T>
	class Handle {
	public:
		Handle();
		Handle(ComponentPool<T>* pool, Entity* entity) : _pool(pool), _entity(entity) {}
		
		T& operator*() {
			return *_pool->getComponent(_entity->id);
		}

		// Overload arrow operator
		T* operator->() {
			return _pool->getComponent(_entity->id);
		}

		// Explicit conversion operator to bool
		explicit operator bool() const {
			return _entity != nullptr;
		}

	private:
		Entity* _entity;
		ComponentPool<T>* _pool;
	};

	template<typename T>
	inline Handle<T>::Handle()
	{
		_pool = nullptr;
		_entity = nullptr;
	}
}