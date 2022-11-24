#include "union_find.hpp"
#include "print.hpp"

//const uint64_t UnionFind::ROOT = UINT64_MAX;

namespace stool{
void UnionFind::initialize(uint64_t size)
{
    this->array.clear();
    this->array.resize(size);
    this->findTmpVec.resize(512);

    this->rank_array.resize(size, 1);

    for (uint64_t i = 0; i < size; i++)
    {
        this->array[i] = i;
    }
}
UnionFind::GINDEX UnionFind::find(uint64_t i)
{
    uint64_t p=0;
    while (true)
    {
        if (this->array[i] == i)
        {
            break;
        }
        else
        {
            this->findTmpVec[p++] = i;
            i = this->array[i];
        }
    }
    
    for (uint64_t x = 0; x < p; x++)
    {
        this->array[this->findTmpVec[x]] = i;
    }
    
    return i;
}
UnionFind::GINDEX UnionFind::findWithoutUpdate(uint64_t i)
{
    while (true)
    {
        if (this->array[i] == i)
        {
            break;
        }
        else
        {
            i = this->array[i];
        }
    }
    return i;
}

UnionFind::GINDEX UnionFind::unionOperation(UnionFind::GINDEX i, UnionFind::GINDEX j)
{
    if(this->rank_array[i] >= this->rank_array[j]){
        this->array[j] = i;
        if(this->rank_array[i] == this->rank_array[j]) this->rank_array[i]++;
    }else{
        return this->unionOperation(j, i);
    }
    return this->find(i);
}
UnionFind::GINDEX UnionFind::unionOperationWithBackup(UnionFind::GINDEX i, UnionFind::GINDEX j)
{
    this->stack.push(std::pair<uint64_t, uint64_t>(j, this->array[j]));
    this->array[j] = i;
    return this->findWithoutUpdate(i);
}
void UnionFind::back()
{
    while (this->stack.size() > 0)
    {
        std::pair<uint64_t, uint64_t> top = this->stack.top();
        this->array[top.first] = top.second;
        this->stack.pop();
    }
}
string UnionFind::toString()
{
    return stool::Printer::toString(this->array, 2);
}
}