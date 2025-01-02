#pragma once
#include "Collision/Collider.h"
#include "Core/ComponentPool.h"
#include <vector>

namespace SimpleECS
{	

    /*
    * Type-erased view into component pools containing colliders.
    * Provides uniform access to different collider types while
    * maintaining contiguous memory layout of underlying pools.
    * Views are non-owning and lightweight.
    */
    class ColliderPoolViewBase {
    public:
        virtual size_t size() const = 0;
        virtual Collider* get(size_t index) = 0;
        virtual ~ColliderPoolViewBase() = default;
    };

    template<typename T>
    class ColliderPoolView : public ColliderPoolViewBase {
    private:
        std::vector<T>* pool;
    public:
        ColliderPoolView(std::vector<T>* p) : pool(p) {}
        
        size_t size() const override { return pool->size(); }
        
        Collider* get(size_t index) override {
            return static_cast<Collider*>(&(*pool)[index]);
        }
    };
    
    /*
    * Provides sequential access to colliders stored in separate type-specific pools.
    * Hides implementation details of separate contiguous storage through iterator interface.
    */
    class ColliderList {
    public:
        class Iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = Collider;
            using pointer = Collider*;
            using reference = Collider&;
    
            Iterator& operator++();
            Iterator operator++(int);
            reference operator*();
            pointer operator->();
            bool operator==(const Iterator& other) const;
            bool operator!=(const Iterator& other) const;
    
        private:
            friend class ColliderList;
            Iterator(std::vector<ColliderPoolViewBase*>* pools, size_t poolIdx = 0, size_t elementIdx = 0);
            
            std::vector<ColliderPoolViewBase*>* pools;
            size_t currentPoolIndex;
            size_t currentElementIndex;
        };
    
        void addPool(ColliderPoolViewBase* pool);
        Iterator begin();
        Iterator end();
        size_t size() const;
    
    private:
        std::vector<ColliderPoolViewBase*> pools;
        size_t totalSize = 0;
    };
}