#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <list>
#include <memory>
#include <stack>
#include "dependency_array.hpp"

// #include "stool/include/light_stool.hpp"

// #include "print.hpp"
#include <set>

namespace stool
{

    namespace lzrr
    {
        class MSDecompressor
        {
        public:
            static void decompress_factors(std::vector<LZFactor> &factors, std::string &output)
            {
                uint64_t len = 0;
                bool isRightReference = false;
                for (uint64_t i = 0; i < factors.size(); i++)
                {
                    if (factors[i].isChar())
                    {
                        len += 1;
                    }
                    else
                    {
                        uint64_t pos = len;
                        if (pos < factors[i].reference)
                        {
                            isRightReference = true;
                        }
                        len += factors[i].length;
                    }
                }
                if (!isRightReference)
                {
                    output.resize(len);
                    uint64_t x = 0;
                    for (uint64_t i = 0; i < factors.size(); i++)
                    {
                        if (factors[i].isChar())
                        {
                            output[x] = factors[i].getChar();
                            x++;
                        }
                        else
                        {
                            for (uint64_t j = 0; j < factors[i].length; j++)
                            {
                                output[x] = output[factors[i].reference + j];
                                x++;
                            }
                        }
                    }
                }
                else
                {
                    DependencyArrayManager dp = DependencyArrayManager();

                    dp.initialize(len, true);
                    uint64_t pos = 0;
                    for (uint64_t i = 0; i < factors.size(); i++)
                    {
                        LZFactor f = factors[i];
                        MSFactor p;
                        if (f.isChar())
                        {
                            p = MSFactor(pos, f.getChar());
                        }
                        else
                        {
                            p = MSFactor(pos, f.reference, f.length);
                        }
                        dp.update(p);

                        pos += f.getLength();

                        if (i % 100000 == 0)
                            std::cout << "\r Constructing Dependency Array : [" << pos << "/" << len << "]" << std::flush;
                    }
                    std::cout << std::endl;

                    output.resize(len);
                    for (uint64_t i = 0; i < len; i++)
                    {
                        output[i] = dp.getChar(i);
                        if (i % 1000000 == 0)
                            std::cout << "\r Decompressing the compressed text : [" << i << "/" << len << "]" << std::flush;
                        // std::cout << dp.dependIndexes[i] << ",";
                    }
                    std::cout << std::endl;
                    // dp.decompress(output);
                }
            }

            static void decompose_MS(std::vector<MSFactor> &factors, std::string &output)
            {
                std::vector<LZFactor> lzf;
                MSFactor::toLZFactors(factors, lzf);
                decompress_factors(lzf, output);
                
            }
        };
    }
}