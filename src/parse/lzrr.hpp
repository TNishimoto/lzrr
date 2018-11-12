#pragma once

#include "lz_factor.hpp"
#include <cassert>
#include "union_find.hpp"
#include "dependency_array.hpp"
#include "lnf_array.hpp"
#include "lp.hpp"

using namespace std;

namespace stool{
/*
 LZRR factorization algorithm
*/
class LZRR
{
  private:
    string *text;
    vector<uint64_t> sa;
    vector<uint64_t> isa;
    vector<uint64_t> lcpArr;
    DependencyArrayManager depper;
    vector<uint64_t> longestRightReferenceArr;

    uint64_t threshold;

    uint64_t size()
    {
        return this->text->size();
    }
    // Initialize this data structure.
    void initialize(uint64_t _threshold, bool usingLCPArray, bool usingDependArray);
    // Compute the factor starting with i-th position.
    MSFactor getLZRRFactor(SINDEX i);
    // Compute the LZRR factorization of the input text.
    void compress(LZWriter &writer);
    /*
    Compute the reference and length of the factor starting with i-th position (on SA).
    The first element : the position of the reference (on SA). 
    If the factor does not have a reference then the value is the maximal value (UINT64_MAX).
    The second element : the length of the factor. 
    */
    static std::pair<SINDEX, uint64_t> getLongestOccurrence(SINDEX i, vector<uint64_t> *lcpArr, DependencyArrayManager &depper, vector<uint64_t> &sa, vector<uint64_t> &isa, string &text);

    public:
    // Compute the LZRR factorization of the input text.
    static void compress(string &text, uint64_t threshold, bool usingLCPArray, bool usingDependArray, LZWriter &writer)
    {
        LZRR md = LZRR();
        md.text = &text;
        md.initialize(threshold, usingLCPArray, usingDependArray);
        md.compress(writer);
    }
    // Compute the LZRR factorization of the input text.
    static void compress(string &text, LZWriter &writer)
    {
        return LZRR::compress(text, UINT64_MAX, true, true, writer);
    }

};
}