#pragma once
#include "io.h"
#include "constants.hpp"
using namespace std;

namespace stool
{
void constructSA(string &text, vector<uint64_t> &sa);
void constructISA(string &text, vector<uint64_t> &sa, vector<uint64_t> &isa);

void constructSA(string &text, vector<uint64_t> &sa, vector<uint64_t> &isa);
void constructLCP(string &text, vector<uint64_t> &lcp, vector<uint64_t> &sa, vector<uint64_t> &isa);

namespace StringFunctions
{
// Return the longest common extension of text[i..] and text[j..].
uint64_t LCE(string &text, uint64_t i, uint64_t j);
// Reverse the input text.
void reverse(string &text);
} // namespace StringFunctions
} // namespace stool