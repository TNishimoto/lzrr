#pragma once

#include "lz_factor.hpp"
#include <cassert>
#include <unordered_map>

using namespace std;

namespace stool{
/*
https://arxiv.org/abs/1702.07577
Note that our implementation runs slowly.
*/
class LCPCompData2
{
  public:
    string text;
    vector<uint64_t> sa;
    vector<uint64_t> isa;
    vector<uint64_t> lcpArr;
    //vector<MSFactor> factors;
    //unordered_map<uint64_t> heap;
    uint64_t size()
    {
        return this->text.size();
    }
    MSFactor getFactor();
    void initialize();
    void compress(LZWriter &writer);
    void update(MSFactor f);
    static void compress(string &text, LZWriter &writer);
};
}
