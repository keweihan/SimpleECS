#include "EcsCell.h"

using namespace SimpleECS;
EcsCell::EcsCell(int defaultSize)
{
    colList.reserve(defaultSize);
}

EcsCell::Iterator SimpleECS::EcsCell::find(Collider* col)
{
    auto findRes = colMap.find(col);
    if (findRes == colMap.end())
    {
        return this->end();
    }
    else
    {
        EcsCell::Iterator found(this);
        found.index = (*findRes).second;
        return found;
    }
}

EcsCell::Iterator SimpleECS::EcsCell::Iterator::operator++(int)
{
    Iterator temp = *this;
    ++index;
    return temp;
}

EcsCell::Iterator& EcsCell::Iterator::operator++()
{
    int newIndex = index +  1;
    while (newIndex <= cellPtr->backIndex && cellPtr->colList[newIndex] == nullptr)
    {
        ++newIndex;
    }
    index = newIndex;

    return *this;
}

EcsCell::Iterator& SimpleECS::EcsCell::Iterator::operator--()
{
    while (cellPtr->colList[index] == nullptr)
    {
        --index;
        if (cellPtr->colList[index] != nullptr)
        {
            return *this;
        }
    }
    return *this;
}

bool SimpleECS::EcsCell::Iterator::operator!=(const Iterator& other) const
{
    return this->index != other.index;
}

bool SimpleECS::EcsCell::Iterator::operator==(const Iterator& other) const
{
    return !(*this != other);
}

bool SimpleECS::EcsCell::Iterator::operator<(const Iterator& other) const
{
    return this->index < other.index;
}

Collider* SimpleECS::EcsCell::Iterator::operator*()
{
    return cellPtr->colList[index];
}

EcsCell::Iterator SimpleECS::EcsCell::begin()
{
    return const_cast<const EcsCell*>(this)->begin();
}

EcsCell::Iterator SimpleECS::EcsCell::back()
{
    Iterator back(this);
    back.index = backIndex;
    return back;
}

EcsCell::Iterator SimpleECS::EcsCell::begin() const
{
    Iterator begin(this);
    if (colMap.size() == 0) {
        return end();
    }
    else
    {
        begin.index = beginIndex;
    }
    return begin;
}

EcsCell::Iterator SimpleECS::EcsCell::end()
{
    return const_cast<const EcsCell*>(this)->end();
}

EcsCell::Iterator SimpleECS::EcsCell::end() const
{
    Iterator end(this);
    if (colMap.size() == 0) {
        end.index = 1;
    }
    else
    {
        end.index = backIndex + 1;
    }
    return end;
}

EcsCell::Iterator SimpleECS::EcsCell::erase(Iterator o)
{
    ++erasures;

    colMap.erase(colMap.find(colList[o.index]));
    colList[o.index] = nullptr;

    // Re-add to list of available indices
    openIndices.insert(o.index);

    // Case erase both begin and back (array size one)
    if (colMap.size() == 0)
    {
        beginIndex = colList.size();  // set to end
        backIndex  = -1;              // set to before beginning
        return end();
    }
    // Case erasing back
    else if (o.index == backIndex) 
    {
        backIndex = (o.index - 1) < 0 ? 0 : o.index - 1;
        while (backIndex > 0 && colList[backIndex] == nullptr) {
            --backIndex;
        }
        return end();
    }
    // Case erasing begin
    else if (o.index == beginIndex)
    {
        ++o;
        beginIndex = o.index;
    }
    else
    {
        // Return next available index.
        ++o;
    }
    
    return o;
}

EcsCell::Iterator SimpleECS::EcsCell::erase(Collider* col)
{
    return this->erase(find(col));
}

int SimpleECS::EcsCell::size()
{
    return colMap.size();
}

void SimpleECS::EcsCell::insert(Collider* col)
{
    if (find(col) != end()) return;
    ++insertions;

    // Check if there is an index available to insert to
    int insertIndex;
    if (openIndices.size() > 0)
    {
        // retrieve a random available index to insert to
        insertIndex = *openIndices.begin();
        openIndices.erase(openIndices.begin());

        // Insert
        colMap.insert({ col, insertIndex });
        colList[insertIndex] = col;
    }
    else
    {
        // Insert to the end
        colList.push_back(col);
        insertIndex = colList.size() - 1;
        colMap.insert({ col, insertIndex });
    }

    // Update begin and back trackers
    beginIndex  = std::min(beginIndex, insertIndex);
    backIndex   = std::max(backIndex, insertIndex);
}
