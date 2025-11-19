#pragma once

#include "../common/lz_factor.hpp"
#include "../common/constants.hpp"
#include "libdivsufsort/sa.hpp"
#include "stool/include/strings/array_constructor.hpp"

#include <cassert>
#include <unordered_map>

namespace stool
{
    namespace lzrr
    {
        class PointCompare
        {
        public:
            static inline std::vector<uint64_t> *lcpArr = nullptr;
            static inline std::vector<uint64_t> *sa = nullptr;
            bool operator()(const uint64_t &pos1, const uint64_t &pos2) const
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

            // std::vector<MSFactor> factors;
            // unordered_map<uint64_t> heap;
            uint64_t size()
            {
                return this->text.size();
            }
            MSFactor getFactor()
            {
                uint64_t maxLCP = UINT64_MAX;
                SINDEX index = UINT64_MAX;

                index = *this->rankSet.begin();
                maxLCP = this->lcpArr[index];
                // std::cout << maxLCP << "/" << index << std::endl;
                /*
                for(SINDEX i=0;i<this->size();i++){
                    if(this->lcpArr[i] != UINT64_MAX){
                        if(maxLCP == UINT64_MAX || maxLCP < this->lcpArr[i]){
                            maxLCP = this->lcpArr[i];
                            index = i;
                        }
                    }
                }
                */
                if (maxLCP == 0)
                {
                    return MSFactor(this->sa[index], this->text[this->sa[index]]);
                }
                else
                {
                    return MSFactor(this->sa[index], this->sa[index - 1], this->lcpArr[index]);
                }
            }
            void initialize()
            {
                auto _sa = libdivsufsort::construct_suffix_array(this->text);
                this->sa.swap(_sa);
                auto _isa = stool::ArrayConstructor::construct_ISA(this->sa);
                this->isa.swap(_isa);
                // stool::constructSA(this->text, this->sa, this->isa);
                auto _lcpArr = stool::ArrayConstructor::construct_LCP_array(this->text, this->sa, this->isa);
                this->lcpArr.swap(_lcpArr);
                PointCompare::lcpArr = &this->lcpArr;
                PointCompare::sa = &this->sa;

                for (uint64_t i = 1; i < this->text.size(); i++)
                {
                    this->rankSet.insert(i);
                }
            }
            void compress(LZWriter &writer)
            {
                //uint64_t x = 0;
                //uint64_t count = 0;

                while (true)
                {
                    MSFactor f = this->getFactor();
                    //x += f.getLength();
                    //count++;

                    // std::cout << f.toString();

                    if (f.isEmpty())
                        break;
                    writer.write2(f);
                    // this->factors.push_back(f);
                    this->update(f);
                }
            }
            void update(MSFactor f)
            {
                if (f.isChar())
                {
                    SINDEX i = this->isa[f.index];
                    this->rankSet.erase(i);
                    this->lcpArr[i] = UINT64_MAX;
                }
                else
                {
                    uint64_t len = f.length;

                    for (uint64_t x = 0; x < len; x++)
                    {
                        SINDEX i = this->isa[f.index + x];
                        this->rankSet.erase(i);
                        this->lcpArr[i] = UINT64_MAX;
                    }

                    for (uint64_t x = 0; x < len; x++)
                    {
                        if (f.index < x + 1)
                            break;
                        SINDEX i = this->isa[f.index - x - 1];
                        if (this->lcpArr[i] > x + 1 && this->lcpArr[i] != UINT64_MAX)
                        {
                            this->rankSet.erase(i);
                            this->lcpArr[i] = x + 1;
                            this->rankSet.insert(i);
                        }
                    }
                }
            }
            static void compress(std::string &text, LZWriter &writer)
            {

                LCPCompData2 d = LCPCompData2();
                d.text = text;
                d.initialize();
                d.compress(writer);
                // MSFactor::toLZFactors(d.factors, output);
            }
        };
    } // namespace stool

}