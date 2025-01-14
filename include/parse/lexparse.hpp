#pragma once

#include "../common/lz_factor.hpp"
#include <cassert>
#include <unordered_map>


namespace stool{
  namespace lzrr{
/*
 LexParse Algorithm
 https://arxiv.org/abs/1803.09517
 This implementation uses the suffix array and inverse suffix array.
*/
class LexParse
{
  public:
    // Construct the lex-parse of the input text.
    static void compress(std::string &text, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa, LZWriter &writer)
    {
            uint64_t i = 0;
            while (i < text.size())
            {
                SINDEX j = isa[i];
                MSFactor f;
                uint64_t lce = j == 0 ? 0 : stool::StringFunctions::LCE(text, sa[j], sa[j - 1]);
                if (lce == 0)
                {
                    f = MSFactor(i, text[i]);
                }
                else
                {
                    f = MSFactor(i, sa[j - 1], lce);
                }
                // std::cout << f.toString();
                writer.write(f.toLZFactor());
                i += f.getLength();
            }
        }
    static void compress(std::string &text, LZWriter &writer)
    {
            std::vector<uint64_t> sa = libdivsufsort::construct_suffix_array(text);
            std::vector<uint64_t> isa = stool::constructISA(text, sa);

            // stool::constructSA(text, sa, isa);
            LexParse::compress(text, sa, isa, writer);
            // MSFactor::toLZFactors(d.factors, output);
        }

    // Construct the lex-r-parse of the input text.
    static void compressR(std::string &text, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa, LZWriter &writer)
    {
            uint64_t i = 0;
            uint64_t p = 0;
            uint64_t n = sa.size();
            while (i < text.size())
            {
                if (p % 10000 == 0)
                    std::cout << "\r"
                              << "Compressing the input text : [" << i << "/" << text.size() << "]" << std::flush;
                p++;

                SINDEX j = isa[i];
                MSFactor f;
                uint64_t lce = j == n - 1 ? 0 : stool::StringFunctions::LCE(text, sa[j], sa[j + 1]);
                if (lce == 0)
                {
                    f = MSFactor(i, text[i]);
                }
                else
                {
                    f = MSFactor(i, sa[j + 1], lce);
                }
                writer.write(f.toLZFactor());
                i += f.getLength();
            }

            std::cout << std::endl;
        }
    static void compressR(std::string &text, LZWriter &writer)
    {

            std::vector<uint64_t> sa = libdivsufsort::construct_suffix_array(text);
            std::vector<uint64_t> isa = stool::constructISA(text, sa);

            LexParse::compressR(text, sa, isa, writer);
            // MSFactor::toLZFactors(d.factors, output);
        }
};
  }
}