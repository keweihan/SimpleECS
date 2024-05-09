#pragma once

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

#include "ComponentPool.h"
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