#pragma once

#include "../common/lz_factor.hpp"
#include <cassert>
#include "../common/union_find.hpp"
#include "../common/dependency_array.hpp"
#include "../common/lnf_array.hpp"
#include "../common/lp.hpp"
#include "libdivsufsort/sa.hpp"
#include "stool/include/sa_bwt_lcp.hpp"
#include "stool/include/string_functions.hpp"

namespace stool{
    namespace lzrr{
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
    void initialize(uint64_t _threshold, bool usingLCPArray, bool usingDependArray)
    {
            auto _sa = libdivsufsort::construct_suffix_array(*this->text);
            this->sa.swap(_sa);
            auto _isa = stool::constructISA(*this->text, this->sa);
            this->isa.swap(_isa);

            // constructSA(*this->text, this->sa, this->isa);

            if (usingLCPArray)
            {
                auto _lcpArr = stool::constructLCP(*this->text, this->sa, this->isa);
                this->lcpArr.swap(_lcpArr);
            }

            this->threshold = _threshold;
            if (this->threshold != UINT64_MAX)
            {
                if (this->lcpArr.size() == 0)
                {
                    auto _lcpArr = stool::constructLCP(*this->text, this->sa, this->isa);
                    this->lcpArr.swap(_lcpArr);
                }
                LNF::computeLNFArray(*this->text, this->sa, this->lcpArr, this->threshold, this->longestRightReferenceArr);
            }
            if (!usingLCPArray && this->lcpArr.size() > 0)
            {
                this->lcpArr.resize(0);
                this->lcpArr.shrink_to_fit();
            }

            std::cout << "Initializing the dependency array manager...";
            this->depper.initialize(this->size(), usingDependArray);
            std::cout << "[END]" << std::endl;
        }
    // Compute the factor starting with i-th position.
    MSFactor getLZRRFactor(SINDEX i)
    {
            TINDEX x = this->sa[i];
            std::pair<SINDEX, uint64_t> z = LZRR::getLongestOccurrence(i, this->lcpArr.size() == 0 ? NULL : &this->lcpArr, this->depper, this->sa, this->isa, *this->text);
            return z.second == 0 ? MSFactor(x, (*this->text)[x]) : MSFactor(x, this->sa[z.first], z.second);
        }
    // Compute the LZRR factorization of the input text.
    void compress(LZWriter &writer)
    {
            uint64_t nowPosition = 0;
            uint64_t p = 0;
            while (nowPosition < this->size())
            {
                if (p % 10000 == 0)
                    std::cout << "\r"
                              << "compressing text.. : [" << nowPosition << "/" << this->size() << "]" << std::flush;

                // auto startLZ = std::chrono::system_clock::now();
                SINDEX i = this->isa[nowPosition];
                MSFactor f;
                if (this->threshold == UINT64_MAX)
                {
                    f = this->getLZRRFactor(i);
                }
                else
                {
                    if (this->longestRightReferenceArr[i] == UINT64_MAX)
                    {
                        f = this->getLZRRFactor(i);
                    }
                    else
                    {
                        TINDEX refIndex = this->longestRightReferenceArr[i];
                        uint64_t lce = stool::StringFunctions::LCE(*this->text, nowPosition, refIndex);
                        if (lce == 0)
                        {
                            f = MSFactor(nowPosition, (*this->text)[nowPosition]);
                        }
                        else
                        {
                            f = MSFactor(nowPosition, refIndex, lce);
                        }
                    }
                }

                // this->factors.push_back(f.toLZFactor());
                this->depper.update(f);
                nowPosition += f.getLength();
                p++;
                writer.write(f.toLZFactor());
            }
            std::cout << std::endl;
        }
    /*
    Compute the reference and length of the factor starting with i-th position (on SA).
    The first element : the position of the reference (on SA). 
    If the factor does not have a reference then the value is the maximal value (UINT64_MAX).
    The second element : the length of the factor. 
    */
    static std::pair<SINDEX, uint64_t> getLongestOccurrence(SINDEX i, std::vector<uint64_t> *lcpArr, DependencyArrayManager &depper, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa, std::string &text)
    {
            LPIterater lpi = LPIterater(i, &sa, &isa, lcpArr);
            // uint64_t n = sa.size();
            uint64_t longestFactorLength = 0;
            SINDEX index = UINT64_MAX;

            while (true)
            {
                uint64_t jFactorLength = 0;
                std::pair<SINDEX, uint64_t> candidate = lpi.take();
                uint64_t lcp = candidate.second;
                uint64_t j = candidate.first;
                if (longestFactorLength >= lcp)
                {
                    break;
                }

                MSFactor f = MSFactor(sa[i], sa[j], lcp);
                if (f.reference > f.index)
                {
                    jFactorLength = f.length;
                }
                else
                {
                    uint64_t fakeLen = depper.getFakeLCPWR(f);

                    if (fakeLen > longestFactorLength)
                    {
                        jFactorLength = depper.getLCPWR(f);
                    }
                }
                if (longestFactorLength < jFactorLength)
                {
                    longestFactorLength = jFactorLength;
                    index = j;
                }
            }
            return std::pair<SINDEX, uint64_t>(index, longestFactorLength);
        }

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
}