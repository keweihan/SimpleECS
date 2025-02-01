#pragma once

#include "SimpleECSAPI.h"
#include "Core/Vector.h"

namespace SimpleECS
{
    class Collider;
    
    /**
    * Data container for collision information
    */
    class SIMPLEECS_API Collision {
    public:
        // Collision object is not to be directly copied.
        Collision(const Collision&) = delete;
        Collision& operator=(const Collision&) = delete;

        Collider* a = nullptr;
        Collider* b = nullptr;
        double penetration = 0;
        Vector normal;
    };
}