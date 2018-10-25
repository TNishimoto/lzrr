#include "debug_lzrr.hpp"

void transformVector(vector<uint64_t> &item, vector<uint64_t> &sa, vector<uint64_t> &output)
{
    output.resize(sa.size());
    for (uint64_t i = 0; i < sa.size(); i++)
    {
        output[sa[i]] = item[i];
    }
}

void transformVector2(vector<uint64_t> &item, vector<uint64_t> &sa, vector<uint64_t> &output)
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