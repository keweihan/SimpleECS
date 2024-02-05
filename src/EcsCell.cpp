#include "EcsCell.h"

using namespace SimpleECS;

class EcsCell::EcsCellImpl
{
public:
    EcsCellImpl(){}
    ~EcsCellImpl(){}

    // index of rightmost populated in colList
    int backIndex = 0;

    // first cell with value
    int beginIndex = 0;

    // sparse list of colliders in this cell 
    std::vector<Collider*> colList;

    // map of collider to index position in colList
    std::unordered_map<Collider*, int> colMap;

    // set of empty indices in colList 
    std::unordered_set<int> openIndices;
};

SimpleECS::EcsCell::EcsCell(const EcsCell& other)
{
    pImpl = std::make_unique<EcsCellImpl>();

    pImpl->backIndex    = other.pImpl->backIndex;
    pImpl->beginIndex   = other.pImpl->beginIndex;
    pImpl->colList      = other.pImpl->colList;
    pImpl->colMap       = other.pImpl->colMap;
    pImpl->openIndices  = other.pImpl->openIndices;
}



EcsCell::EcsCell(int defaultSize)
{
    pImpl = std::make_unique<EcsCellImpl>();
    pImpl->colList.reserve(defaultSize);
}

EcsCell::EcsCell()
{
    pImpl = std::make_unique<EcsCellImpl>();
    pImpl->colList.reserve(10);
}

EcsCell::~EcsCell() {}

EcsCell& SimpleECS::EcsCell::operator=(const EcsCell& other)
{
    pImpl = std::make_unique<EcsCellImpl>();

    pImpl->backIndex = other.pImpl->backIndex;
    pImpl->beginIndex = other.pImpl->beginIndex;
    pImpl->colList = other.pImpl->colList;
    pImpl->colMap = other.pImpl->colMap;
    pImpl->openIndices = other.pImpl->openIndices;
}

int EcsCell::size()
{
    return pImpl->colMap.size();
}

void EcsCell::insert(Collider* col)
{
    if (find(col) != end()) return;

    // Check if there is an index available to insert to
    int insertIndex;
    if (pImpl->openIndices.size() > 0)
    {
        // retrieve a random available index to insert to
        insertIndex = *pImpl->openIndices.begin();
        pImpl->openIndices.erase(pImpl->openIndices.begin());

        // Insert
        pImpl->colMap.insert({ col, insertIndex });
        pImpl->colList[insertIndex] = col;
    }
    else
    {
        // Insert to the end
        pImpl->colList.push_back(col);
        insertIndex = pImpl->colList.size() - 1;
        pImpl->colMap.insert({ col, insertIndex });
    }

    // Update begin and back trackers
    pImpl->beginIndex  = std::min(pImpl->beginIndex, insertIndex);
    pImpl->backIndex   = std::max(pImpl->backIndex, insertIndex);
}

EcsCellIterator EcsCell::begin()
{
    return const_cast<const EcsCell*>(this)->begin();
}

EcsCellIterator EcsCell::back()
{
    EcsCellIterator back(this);
    back.index = pImpl->backIndex;
    return back;
}

EcsCellIterator EcsCell::begin() const
{
    EcsCellIterator begin(this);
    if (pImpl->colMap.size() == 0) {
        return end();
    }
    else
    {
        begin.index = pImpl->beginIndex;
    }
    return begin;
}

EcsCellIterator EcsCell::end()
{
    return const_cast<const EcsCell*>(this)->end();
}

EcsCellIterator EcsCell::end() const
{
    EcsCellIterator end(this);
    if (pImpl->colMap.size() == 0) {
        end.index = 1;
    }
    else
    {
        end.index = pImpl->backIndex + 1;
    }
    return end;
}

EcsCellIterator EcsCell::erase(EcsCellIterator o)
{
    pImpl->colMap.erase(pImpl->colMap.find(pImpl->colList[o.index]));
    pImpl->colList[o.index] = nullptr;

    // Re-add to list of available indices
    pImpl->openIndices.insert(o.index);

    // Case erase both begin and back (array size one)
    if (pImpl->colMap.size() == 0)
    {
        pImpl->beginIndex = pImpl->colList.size();  // set to end
        pImpl->backIndex = -1;              // set to before beginning
        return end();
    }
    // Case erasing back
    else if (o.index == pImpl->backIndex)
    {
        pImpl->backIndex = (o.index - 1) < 0 ? 0 : o.index - 1;
        while (pImpl->backIndex > 0 && pImpl->colList[pImpl->backIndex] == nullptr) {
            --pImpl->backIndex;
        }
        return end();
    }
    // Case erasing begin
    else if (o.index == pImpl->beginIndex)
    {
        ++o;
        pImpl->beginIndex = o.index;
    }
    else
    {
        // Return next available index.
        ++o;
    }

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
        return this->end();
    }
    else
    {
        EcsCellIterator found(this);
        found.index = (*findRes).second;
        return found;
    }
}

EcsCellIterator EcsCellIterator::operator++(int)
{
    EcsCellIterator temp = *this;
    ++index;
    return temp;
}

EcsCellIterator& EcsCellIterator::operator++()
{
    int newIndex = index + 1;
    while (newIndex <= cellPtr->pImpl->backIndex && cellPtr->pImpl->colList[newIndex] == nullptr)
    {
        ++newIndex;
    }
    index = newIndex;

    return *this;
}

EcsCellIterator& EcsCellIterator::operator--()
{
    while (cellPtr->pImpl->colList[index] == nullptr)
    {
        --index;
        if (cellPtr->pImpl->colList[index] != nullptr)
        {
            return *this;
        }
    }
    return *this;
}

bool EcsCellIterator::operator!=(const EcsCellIterator& other) const
{
    return this->index != other.index;
}

bool EcsCellIterator::operator==(const EcsCellIterator& other) const
{
    return !(*this != other);
}

bool EcsCellIterator::operator<(const EcsCellIterator& other) const
{
    return this->index < other.index;
}

Collider* EcsCellIterator::operator*()
{
    return cellPtr->pImpl->colList[index];
}