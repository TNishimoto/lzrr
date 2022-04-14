#pragma once
#include <cassert>
#include <unordered_map>
#include "./constants.hpp"
#include "lz_factor.hpp"
#include "union_find.hpp"


namespace stool{
    
    namespace lzrr
    {
    /*
    This is the data structure to emulate dependency array of the input text.
    */
class DependencyArrayManager{
    private:
    // uf.find(i) == uf.find(j) if DA[i] == DA[j].
    UnionFind uf;
    // DA[i] = dependencyArrayForGroups[uf.find(i)]
    std::vector<uint64_t> dependencyArrayForGroups;
    // If T[i] is a factor not having a reference then T[i] = factorChars[i].
    std::unordered_map<uint64_t, char> factorChars;
    std::vector<uint64_t> smallDA;
    std::stack<uint64_t> smallStack;


    bool isUsingDependencyArrayForGroups(){
        return this->dependencyArrayForGroups.size() > 0;
    }
    // Return DA[i].
    TINDEX access(TINDEX i);
    public:
    // Initialize this data structure.
    void initialize(uint64_t size, bool usingDependIndexes);
    // Return the size of the input text.
    uint64_t size();
    // Return T[i].
    char getChar(TINDEX i);

    // Update DA by f.
    uint64_t update(MSFactor &f);
    // Return the LCPWR(f.index, f.reference).
    uint64_t getLCPWR(MSFactor &f);
    // Return the LCPWR(f.index, f.reference).
    uint64_t getLCPWR_Y(MSFactor &f);
    // Return the FakeLCPWR(f.index, f.reference).
    uint64_t getFakeLCPWR(MSFactor &f);
};
    }
}