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
    static void compress(std::string &text, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa, LZWriter &writer);
    static void compress(std::string &text, LZWriter &writer);

    // Construct the lex-r-parse of the input text.
    static void compressR(std::string &text, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa, LZWriter &writer);
    static void compressR(std::string &text, LZWriter &writer);
};
  }
}