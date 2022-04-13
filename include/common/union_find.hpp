#pragma once
#include "io.h"
#include <stack>
using namespace std;

namespace stool{

/*
Disjoint-set data structure
https://en.wikipedia.org/wiki/Disjoint-set_data_structure
*/
class UnionFind{
    private:
    vector<uint64_t> findTmpVec;
    std::stack<std::pair<uint64_t, uint64_t>> stack;
    vector<uint64_t> array;
    vector<uint8_t> rank_array;

    public:
    using GINDEX = uint64_t;


    uint64_t size(){
        return this->array.size();
    }
    UnionFind(){

    }
    // Construct size-groups whose each size is 1.
    void initialize(uint64_t size);
    // Return the group number of the i-th element.
    GINDEX find(uint64_t i);
    // Return the group number of the i-th element without updating the data structure.
    GINDEX findWithoutUpdate(uint64_t i);

    // Unify two groups of i-th and j-th element and return the group number.
    GINDEX unionOperation(GINDEX i, GINDEX j);
    // Unify two groups of i-th and j-th element and return the group number.
    GINDEX unionOperationWithBackup(GINDEX i, GINDEX j);
    void add(){
        this->array.push_back(this->size());
        this->rank_array.push_back(1);
    }
    // Get the data structure when we used the last unionOperationWithBackup operation.
    void back();
    string toString();

};
}

