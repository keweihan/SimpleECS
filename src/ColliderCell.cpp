#include "ColliderCell.h"
#include "boost/container/static_vector.hpp"

using namespace SimpleECS;

class ColliderCell::ColliderCellImpl
{
public:
    ColliderCellImpl(){}
    ~ColliderCellImpl(){}

    // dense list of colliders in this cell 
    // std::vector<Collider*> colList;
    std::vector<Collider*> colList;
};

SimpleECS::ColliderCell::ColliderCell(const ColliderCell& other)
{
    pImpl = std::make_unique<ColliderCellImpl>();
    pImpl->colList      = other.pImpl->colList;
}

ColliderCell::ColliderCell(int defaultSize)
{
    pImpl = std::make_unique<ColliderCellImpl>();
    pImpl->colList.reserve(defaultSize);
}

ColliderCell::ColliderCell()
{
    pImpl = std::make_unique<ColliderCellImpl>();
    pImpl->colList.reserve(30);
}

ColliderCell::~ColliderCell() {}

ColliderCell& SimpleECS::ColliderCell::operator=(const ColliderCell& other)
{
    pImpl = std::make_unique<ColliderCellImpl>();

    pImpl->colList = other.pImpl->colList;
    return *this;
}

int ColliderCell::size()
{
    return pImpl->colList.size();
}

void ColliderCell::insert(Collider* col)
{
    if (find(col) == end()) 
    {
        // Insert to the end
        pImpl->colList.push_back(col);
    }
}

ColliderCellIterator ColliderCell::erase(ColliderCellIterator o)
{
    // Special case if last element
    if (o - pImpl->colList.begin() == pImpl->colList.size() - 1) {
        pImpl->colList.pop_back();
        return pImpl->colList.end();
    }

    // Replace element with back element and remove from back
    *o = pImpl->colList.back();
    pImpl->colList.pop_back();

    return o;
}

ColliderCellIterator ColliderCell::erase(Collider* col)
{
    return this->erase(find(col));
}

ColliderCellIterator ColliderCell::find(Collider* col)
{
    auto res = pImpl->colList.end();
    for (auto iter = pImpl->colList.begin(); iter != pImpl->colList.end(); ++iter)
    {
        if (*iter == col) { return iter;  }
    }
    return res;
}

ColliderCellIterator ColliderCell::begin() const
{
    return pImpl->colList.begin();
}

ColliderCellIterator ColliderCell::begin()
{
    return pImpl->colList.begin();
}

Collider* ColliderCell::back()
{
    return pImpl->colList.back();
}

ColliderCellIterator ColliderCell::end()
{
    return pImpl->colList.end();
}

ColliderCellIterator ColliderCell::end() const
{
    return pImpl->colList.end();
}