#pragma once

//#include "other/lzr.hpp"
#include "lz_factor.hpp"
#include <cassert>
#include "union_find.hpp"
#include <unordered_map>
//#include "treap/dynamic_string.hpp"


using namespace std;
using namespace stool;

namespace stool{
struct LNFInfo
{
    SINDEX begin;
    SINDEX end;
    uint64_t lcp;
    bool isBeg;

    LNFInfo() : begin(UINT64_MAX), end(UINT64_MAX), lcp(0)
    {
    }
    LNFInfo(SINDEX _begin, SINDEX _end, uint64_t upperLCP, uint64_t lowerLCP)
    {
        this->begin = _begin;
        this->end = _end;

        if (this->begin != UINT64_MAX && this->end != UINT64_MAX)
        {
            if (upperLCP > lowerLCP)
            {
                this->isBeg = true;
                this->lcp = upperLCP;
            }
            else
            {
                this->isBeg = false;
                this->lcp = lowerLCP;
            }
        }
        else if (this->begin != UINT64_MAX)
        {
            this->isBeg = true;
            this->lcp = upperLCP;
        }
        else if (this->end != UINT64_MAX)
        {
            this->isBeg = false;
            this->lcp = lowerLCP;
        }
        else
        {
            this->lcp = 0;
        }
    }

    uint64_t getArea()
    {
        int64_t _beg = this->begin == UINT64_MAX ? 0 : this->begin;
        int64_t tate = (this->end - _beg - 2);
        if (tate <= 0)
        {
            return 0;
        }
        else
        {
            int64_t pSize = tate * (this->lcp + 1);
            return pSize;
        }
    }
    TINDEX reference(vector<uint64_t> &sa)
    {
        if (this->isBeg)
        {
            if (this->begin < sa.size())
            {
                return sa[this->begin];
            }
            else
            {
                return UINT64_MAX;
            }
        }
        else
        {
            if (this->end < sa.size())
            {
                return sa[this->end];
            }
            else
            {
                return UINT64_MAX;
            }
        }
    }
};


namespace LNF{

    void computeLNFArray(string &text, vector<uint64_t> &sa, vector<uint64_t> &lcpArr, vector<LNFInfo> &output);
    void computeLNFArrayForDebug(string &text, vector<uint64_t> &sa, vector<uint64_t> &lcpArr, uint64_t threshold, vector<TINDEX> &output);
    void computeLNFArray(string &text, vector<uint64_t> &sa, vector<uint64_t> &lcpArr, uint64_t threshold, vector<TINDEX> &output);
}
}