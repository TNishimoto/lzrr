#pragma once
#include <vector>
#include <iostream>
#include <list>
#include <memory>
#include <stack>
#include "sa_lcp.hpp"
#include <set>
#include "lz_factor.hpp"

namespace stool{
/*
LZ77 factorization algorithm.
This implementation uses the suffix array and inverse suffix array.
*/
class LZ77{
    public:
    static void constructLPFUDArray(vector<uint64_t> &sa, vector<uint64_t> &lpfu, vector<uint64_t> &lpfd);
    // Computing the LZ77 factorization of the input text.    
    static void compress(string &text, vector<uint64_t> &lpf, LZWriter &writer);
    // Computing the LZ77 factorization of the input text.
    static void compress(string &text, vector<uint64_t> &lpfu, vector<uint64_t> &lpfd, LZWriter &writer);
    // Computing the LZ77 factorization of the input text.
    static void compress(string &text, LZWriter &writer);
};
}