#pragma once

#include "../common/lz_factor.hpp"
#include <cassert>
#include <unordered_map>

using namespace std;

namespace stool
{

    class PointCompare
    {
    public:
        static std::vector<uint64_t> *lcpArr;
        static std::vector<uint64_t> *sa;
        bool operator()(const uint64_t &pos1, const uint64_t &pos2)
        {
            uint64_t lcp1 = (*PointCompare::lcpArr)[pos1];
            uint64_t lcp2 = (*PointCompare::lcpArr)[pos2];
            if (lcp1 == lcp2)
            {
                return (*PointCompare::sa)[pos1] < (*PointCompare::sa)[pos2];
            }
            else
            {
                return lcp1 > lcp2;
            }
        }
    };
    /*
https://arxiv.org/abs/1702.07577
Note that our implementation runs slowly.
*/
    class LCPCompData2
    {
    public:
        std::string text;
        std::vector<uint64_t> sa;
        std::vector<uint64_t> isa;
        std::vector<uint64_t> lcpArr;
        std::set<uint64_t, PointCompare> rankSet;

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
        static void compress(std::string &text, LZWriter &writer)
        {

            LCPCompData2 d = LCPCompData2();
            d.text = text;
            d.initialize();
            d.compress(writer);
            //MSFactor::toLZFactors(d.factors, output);
        }
    };
} // namespace stool

