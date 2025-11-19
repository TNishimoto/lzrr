

#pragma once
#include <cassert>
#include <vector>

//#include "libdivsufsort/sa.hpp"
#include "libsais/sa.hpp"

namespace stool
{
    namespace lzrr
    {
        class SAConstructor{
            public:
            static std::vector<uint64_t> construct_suffix_array(const std::string &text){
                return libsais::construct_suffix_array(text);
            }
            static std::vector<uint64_t> construct_suffix_array(const std::vector<char> &text){
                return libsais::construct_suffix_array(text);
            }

        };
    }
}
