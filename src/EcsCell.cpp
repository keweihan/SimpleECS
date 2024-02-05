#include "EcsCell.h"

using namespace SimpleECS;

class EcsCell::EcsCellImpl
{
public:
    EcsCellImpl(){}
    ~EcsCellImpl(){}

    // dense list of colliders in this cell 
    std::vector<Collider*> colList;

    // map of collider to index position in colList
    std::unordered_map<Collider*, int> colMap;

    int backIndex = 0;
};

SimpleECS::EcsCell::EcsCell(const EcsCell& other)
{
    pImpl = std::make_unique<EcsCellImpl>();

    pImpl->colList      = other.pImpl->colList;
    pImpl->colMap       = other.pImpl->colMap;

    pImpl->backIndex    = other.pImpl->backIndex;
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
    pImpl->colMap = other.pImpl->colMap;

    pImpl->backIndex = other.pImpl->backIndex;

    return *this;
}

int EcsCell::size()
{
    return pImpl->colMap.size();
}

void EcsCell::insert(Collider* col)
{
    if (find(col) == end()) 
    {
        // Insert to the end
        pImpl->colList.push_back(col);
        pImpl->colMap.insert({ col, pImpl->colList.size() - 1 });
    }
}

EcsCellIterator EcsCell::erase(EcsCellIterator o)
{
    // Special case if last element
    if (o - pImpl->colList.begin() == pImpl->colList.size() - 1) {
        pImpl->colMap.erase(*o);
        pImpl->colList.pop_back();
        return pImpl->colList.end();
    }

    // Overwrite with last element and pop_back
    int index = o - pImpl->colList.begin();
    pImpl->colMap.erase(*o);

    // Replace element with back element and update back ele index
    *o = pImpl->colList.back();
    pImpl->colMap[*o] = index;

    // Remove from back
    pImpl->colList.pop_back();

    // Get valid iterator
    o = pImpl->colList.begin() + index;
    return o;
}

EcsCellIterator EcsCell::erase(Collider* col)
{
    return this->erase(find(col));
}

EcsCellIterator EcsCell::find(Collider* col)
{
    auto findRes = pImpl->colMap.find(col);
    if (findRes == pImpl->colMap.end())
    {
        return pImpl->colList.end();
    }
    else
    {
        EcsCellIterator found = pImpl->colList.begin() + (*findRes).second;
        return found;
    }
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