#pragma once

#include "lz_factor.hpp"
#include <cassert>
#include <unordered_map>

using namespace std;

namespace stool{
/*
 LexParse Algorithm
 https://arxiv.org/abs/1803.09517
 This implementation uses the suffix array and inverse suffix array.
*/
class LexParse
{
  public:
    // Construct the lex-parse of the input text.
    static void compress(string &text, vector<uint64_t> &sa, vector<uint64_t> &isa, LZWriter &writer);
    static void compress(string &text, LZWriter &writer);

    // Construct the lex-r-parse of the input text.
    static void compressR(string &text, vector<uint64_t> &sa, vector<uint64_t> &isa, LZWriter &writer);
    static void compressR(string &text, LZWriter &writer);
};
}