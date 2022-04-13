#pragma once
#include "libdivsufsort/sa.hpp"
#include "constants.hpp"

namespace stool
{

    namespace lzrr
    {
        // void constructSA(std::string &text, std::vector<uint64_t> &sa);
        void constructISA(std::string &text, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa);

        // void constructSA(std::string &text, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa);
        void constructLCP(std::string &text, std::vector<uint64_t> &lcp, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa);

        namespace StringFunctions
        {
            // Return the longest common extension of text[i..] and text[j..].
            uint64_t LCE(std::string &text, uint64_t i, uint64_t j);
            // Reverse the input text.
            void reverse(std::string &text);
        } // namespace StringFunctions
    }
} // namespace stool