
#pragma once
#include <vector>
#include "constants.hpp"
class LPIterater
{
    std::vector<uint64_t> *sa;
    std::vector<uint64_t> *isa;
    std::vector<uint64_t> *lcpArr;
    SINDEX i;
    int64_t lowerIndex;
    int64_t upperIndex;
    uint64_t lowerLCP;
    uint64_t upperLCP;
    uint64_t n;

public:
    LPIterater(SINDEX _i, std::vector<uint64_t> *_sa, std::vector<uint64_t> *_isa, std::vector<uint64_t> *_lcpArr)
    {
        this->i = _i;
        this->sa = _sa;
        this->isa = _isa;
        this->lcpArr = _lcpArr;
        this->lowerIndex = i;
        this->upperIndex = i;
        this->lowerLCP = UINT64_MAX;
        this->upperLCP = UINT64_MAX;
        this->n = (*sa).size();
    }

    std::pair<SINDEX, uint64_t> take()
    {
        int64_t lowerNextIndex = lowerIndex + 1;
        int64_t upperNextIndex = upperIndex - 1;

        int64_t nextLowerLCP = lowerNextIndex >= n ? -1 : (*lcpArr)[lowerNextIndex] < lowerLCP ? (*lcpArr)[lowerNextIndex] : lowerLCP;
        int64_t nextUpperLCP = upperNextIndex < 0 ? -1 : (*lcpArr)[upperNextIndex + 1] < upperLCP ? (*lcpArr)[upperNextIndex + 1] : upperLCP;
        uint64_t upperDist = this->i - upperNextIndex;
        uint64_t lowerDist = lowerNextIndex - this->i;

        if (nextUpperLCP > nextLowerLCP || (nextUpperLCP == nextLowerLCP && upperDist <= lowerDist))
        {
            upperIndex = upperNextIndex;
            upperLCP = (uint64_t)nextUpperLCP;
            return std::pair<SINDEX, uint64_t>(upperIndex, upperLCP);
        }
        else
        {
            lowerIndex = lowerNextIndex;
            lowerLCP = (uint64_t)nextLowerLCP;
            return std::pair<SINDEX, uint64_t>(lowerIndex, lowerLCP);
        }
    }
    bool end()
    {
        if (this->lowerIndex == n - 1 && this->upperIndex == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};