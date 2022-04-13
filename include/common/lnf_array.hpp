#pragma once

//#include "other/lzr.hpp"
#include "lz_factor.hpp"
#include <cassert>
#include "union_find.hpp"
#include <unordered_map>
//#include "treap/dynamic_string.hpp"

namespace stool
{

    namespace lzrr
    {
        struct LNFInfo
        {
            SINDEX begin;
            SINDEX end;
            uint64_t lcp;
            bool isBeg;

            LNFInfo() : begin(UINT64_MAX), end(UINT64_MAX), lcp(0)
            {
            }
            LNFInfo(SINDEX _begin, SINDEX _end, uint64_t upperLCP, uint64_t lowerLCP);

            uint64_t getArea();
            TINDEX reference(std::vector<uint64_t> &sa);
        };

        namespace LNF
        {

            void computeLNFArray(std::string &text, std::vector<uint64_t> &sa, std::vector<uint64_t> &lcpArr, std::vector<LNFInfo> &output);
            void computeLNFArrayForDebug(std::string &text, std::vector<uint64_t> &sa, std::vector<uint64_t> &lcpArr, uint64_t threshold, std::vector<TINDEX> &output);
            void computeLNFArray(std::string &text, std::vector<uint64_t> &sa, std::vector<uint64_t> &lcpArr, uint64_t threshold, std::vector<TINDEX> &output);
        }
    }
}