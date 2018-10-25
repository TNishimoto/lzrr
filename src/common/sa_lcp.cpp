#include "sa_lcp.hpp"
#include <unordered_set>
#include <cassert>
#include <divsufsort.h>
#include <divsufsort64.h>

namespace stool
{
uint64_t intervalTotalCount = 0;
uint64_t rlcpCounter = 0;

void constructSA(string &text, vector<uint64_t> &sa)
{
    
    uint64_t n = text.size();
    sa.resize(n);

    int64_t *SA = (int64_t *)malloc(n * sizeof(int64_t));
    divsufsort64((const unsigned char *)&text[0], SA, n);

    for (uint64_t i = 0; i < text.size(); ++i)
    {
        sa[i] = SA[i];
    }
    free(SA);
    
}
void constructISA(string &text, vector<uint64_t> &sa, vector<uint64_t> &isa)
{
    uint64_t n = text.size();
    isa.resize(n);

    for (uint64_t i = 0; i < text.size(); ++i)
    {
        isa[sa[i]] = i;
    }
}
void constructSA(string &text, vector<uint64_t> &sa, vector<uint64_t> &isa)
{
    
    std::cout << "constructing Suffix Array...";
    uint64_t n = text.size();
    isa.resize(n);
    sa.resize(n);

    int64_t *SA = (int64_t *)malloc(n * sizeof(int64_t));
    divsufsort64((const unsigned char *)&text[0], SA, n);

    for (uint64_t i = 0; i < text.size(); ++i)
    {
        sa[i] = SA[i];
        isa[SA[i]] = i;
    }
    free(SA);
    std::cout << "[END]" << std::endl;
    

}
void constructLCP(string &text, vector<uint64_t> &lcp, vector<uint64_t> &sa, vector<uint64_t> &isa)
{
    lcp.resize(text.size(), 0);
    uint64_t n = text.size();
    uint64_t k = 0;
    for (uint64_t i = 0; i < n; i++)
    {

        if (i % 10000000 == 0)
            std::cout << "\r"
                      << "constructing LCP Array : [" << i << "/" << n << "]" << std::flush;

        uint64_t x = isa[i];
        if (x == 0)
        {
            lcp[x] = 0;
        }
        else
        {
            while (text[sa[x] + k] == text[sa[x - 1] + k])
            {
                k++;
            }
        }
        lcp[x] = k;
        if (k > 0)
            k--;
    }
    std::cout << std::endl;
}
namespace StringFunctions
{
uint64_t LCE(string &text, uint64_t i, uint64_t j)
{
    if (i > j)
        return LCE(text, j, i);
    uint64_t max = text.size() - j;
    uint64_t x = 0;
    for (x = 0; x < max; x++)
    {
        if (text[i + x] != text[j + x])
        {
            break;
        }
    }
    return x;
}
void reverse(string &text)
{
    string tmp = text;
    for (uint64_t i = 0; i < text.size(); i++)
    {
        text[i] = tmp[text.size() - 1 - i];
    }
}
} // namespace string
} // namespace stool
