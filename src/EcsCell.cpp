#include "EcsCell.h"

using namespace SimpleECS;

class EcsCell::EcsCellImpl
{
public:
    EcsCellImpl(){}
    ~EcsCellImpl(){}

    // dense list of colliders in this cell 
    std::vector<Collider*> colList;
};

SimpleECS::EcsCell::EcsCell(const EcsCell& other)
{
    pImpl = std::make_unique<EcsCellImpl>();
    pImpl->colList      = other.pImpl->colList;
}

EcsCell::EcsCell(int defaultSize)
{
    pImpl = std::make_unique<EcsCellImpl>();
    pImpl->colList.reserve(defaultSize);
}

EcsCell::EcsCell()
{
    pImpl = std::make_unique<EcsCellImpl>();
    pImpl->colList.reserve(30);
}

EcsCell::~EcsCell() {}

EcsCell& SimpleECS::EcsCell::operator=(const EcsCell& other)
{
    pImpl = std::make_unique<EcsCellImpl>();

    pImpl->colList = other.pImpl->colList;
    return *this;
}

int EcsCell::size()
{
    return pImpl->colList.size();
}

void EcsCell::insert(Collider* col)
{
    if (find(col) == end()) 
    {
        // Insert to the end
        pImpl->colList.push_back(col);
    }
}

EcsCellIterator EcsCell::erase(EcsCellIterator o)
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

EcsCellIterator EcsCell::erase(Collider* col)
{
    return this->erase(find(col));
}

EcsCellIterator EcsCell::find(Collider* col)
{
    auto res = pImpl->colList.end();
    for (auto iter = pImpl->colList.begin(); iter != pImpl->colList.end(); ++iter)
    {
        if (*iter == col) { return iter;  }
    }
    return res;
}

EcsCellIterator EcsCell::begin() const
{
    return pImpl->colList.begin();
}

EcsCellIterator EcsCell::begin()
{
    return pImpl->colList.begin();
}

Collider* EcsCell::back()
{
    return pImpl->colList.back();
}

EcsCellIterator EcsCell::end()
{
    return pImpl->colList.end();
}

EcsCellIterator EcsCell::end() const
{
    return pImpl->colList.end();
}