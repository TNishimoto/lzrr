#include "debug/debug_lzrr.hpp"

void transformVector(std::vector<uint64_t> &item, std::vector<uint64_t> &sa, std::vector<uint64_t> &output)
{
    output.resize(sa.size());
    for (uint64_t i = 0; i < sa.size(); i++)
    {
        output[sa[i]] = item[i];
    }
}

void transformVector2(std::vector<uint64_t> &item, std::vector<uint64_t> &sa, std::vector<uint64_t> &output)
{
    output.resize(sa.size());
    for (uint64_t i = 0; i < sa.size(); i++)
    {
        if (item[i] >= sa.size())
        {
            output[sa[i]] = item[i];
        }
        else
        {
            output[sa[i]] = sa[item[i]];
        }
    }
}