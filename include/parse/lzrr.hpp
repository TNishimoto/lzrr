#pragma once

#include "../common/lz_factor.hpp"
#include <cassert>
#include "../common/union_find.hpp"
#include "../common/dependency_array.hpp"
#include "../common/lnf_array.hpp"
#include "../common/lp.hpp"


namespace stool{
/*
 LZRR factorization algorithm
*/
class LZRR
{
  private:
    std::string *text;
    std::vector<uint64_t> sa;
    std::vector<uint64_t> isa;
    std::vector<uint64_t> lcpArr;
    DependencyArrayManager depper;
    std::vector<uint64_t> longestRightReferenceArr;

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
    static std::pair<SINDEX, uint64_t> getLongestOccurrence(SINDEX i, std::vector<uint64_t> *lcpArr, DependencyArrayManager &depper, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa, std::string &text);

    public:
    // Compute the LZRR factorization of the input text.
    static void compress(std::string &text, uint64_t threshold, bool usingLCPArray, bool usingDependArray, LZWriter &writer)
    {
        LZRR md = LZRR();
        md.text = &text;
        md.initialize(threshold, usingLCPArray, usingDependArray);
        md.compress(writer);
    }
    // Compute the LZRR factorization of the input text.
    static void compress(std::string &text, LZWriter &writer)
    {
        return LZRR::compress(text, UINT64_MAX, true, true, writer);
    }

};
}