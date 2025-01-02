#include "Collision/ColliderList.h"

namespace SimpleECS {

ColliderList::Iterator::Iterator(std::vector<ColliderPoolViewBase*>* p, size_t poolIdx, size_t elementIdx)
    : pools(p), currentPoolIndex(poolIdx), currentElementIndex(elementIdx) {
}

ColliderList::Iterator& ColliderList::Iterator::operator++() {
    if (!pools || currentPoolIndex >= pools->size()) {
        return *this;
    }

    currentElementIndex++;
    if (currentElementIndex >= (*pools)[currentPoolIndex]->size()) {
        currentElementIndex = 0;
        currentPoolIndex++;
    }

    return *this;
}

ColliderList::Iterator ColliderList::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

ColliderList::Iterator::reference ColliderList::Iterator::operator*() {
    return *(*(*pools)[currentPoolIndex]).get(currentElementIndex);
}

ColliderList::Iterator::pointer ColliderList::Iterator::operator->() {
    return (*(*pools)[currentPoolIndex]).get(currentElementIndex);
}

bool ColliderList::Iterator::operator==(const Iterator& other) const {
    return pools == other.pools && 
           currentPoolIndex == other.currentPoolIndex && 
           currentElementIndex == other.currentElementIndex;
}

bool ColliderList::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

void ColliderList::addPool(ColliderPoolViewBase* pool) {
    pools.push_back(pool);
    totalSize += pool->size();
}

ColliderList::Iterator ColliderList::begin() {
    if (pools.empty()) {
        return end();
    }
    return Iterator(&pools, 0, 0);
}

ColliderList::Iterator ColliderList::end() {
    return Iterator(&pools, pools.size(), 0);
}

size_t ColliderList::size() const {
    return totalSize;
}

} // namespace SimpleECS
