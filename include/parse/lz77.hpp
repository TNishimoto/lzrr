#pragma once
#include <vector>
#include <iostream>
#include <list>
#include <memory>
#include <stack>

#include <set>
#include "../common/lz_factor.hpp"

namespace stool
{
    namespace lzrr
    {
        /*
        LZ77 factorization algorithm.
        This implementation uses the suffix array and inverse suffix array.
        */
        class LZ77
        {
        public:
            static void constructLPFUDArray(std::vector<uint64_t> &sa, std::vector<uint64_t> &lpfu, std::vector<uint64_t> &lpfd);
            // Computing the LZ77 factorization of the input text.
            static void compress(std::string &text, std::vector<uint64_t> &lpf, LZWriter &writer);
            // Computing the LZ77 factorization of the input text.
            static void compress(std::string &text, std::vector<uint64_t> &lpfu, std::vector<uint64_t> &lpfd, LZWriter &writer);
            // Computing the LZ77 factorization of the input text.
            static void compress(std::string &text, LZWriter &writer);
        };
    }
}