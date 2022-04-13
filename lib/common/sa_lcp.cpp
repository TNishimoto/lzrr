#include <unordered_set>
#include <cassert>
#include <iostream>
#include <fstream>
#include "common/sa_lcp.hpp"

namespace stool
{
    uint64_t intervalTotalCount = 0;
    uint64_t rlcpCounter = 0;

    /*
    void constructSA(std::string &text, std::vector<uint64_t> &sa)
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
    */
    void constructISA(std::string &text, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa)
    {
        uint64_t n = text.size();
        isa.resize(n);

        for (uint64_t i = 0; i < text.size(); ++i)
        {
            isa[sa[i]] = i;
        }
    }
    /*
    void constructSA(std::string &text, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa)
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
    */
    void constructLCP(std::string &text, std::vector<uint64_t> &lcp, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa)
    {
        lcp.resize(text.size(), 0);
        uint64_t n = text.size();
        uint64_t k = 0;
        if (text.size() > 1000000)
            std::cout << "Constructing LCP Array" << std::flush;

        for (uint64_t i = 0; i < n; i++)
        {

            assert(i < n);
            uint64_t x = isa[i];
            assert(x < n);

            if (x == 0)
            {
                lcp[x] = 0;
            }
            else
            {
                while (sa[x] + k < n && sa[x - 1] + k < n && text[sa[x] + k] == text[sa[x - 1] + k])
                {
                    assert(sa[x] + k < n);
                    assert(sa[x - 1] + k < n);

                    k++;
                }
            }
            lcp[x] = k;

            assert(x == 0 || (x > 0 && ((n - sa[x - 1]) >= k)));

            if (k > 0)
                k--;
        }

        if (n > 1000000)
            std::cout << "[END]" << std::endl;
    }
    namespace StringFunctions
    {
        uint64_t LCE(std::string &text, uint64_t i, uint64_t j)
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
        void reverse(std::string &text)
        {
            std::string tmp = text;
            for (uint64_t i = 0; i < text.size(); i++)
            {
                text[i] = tmp[text.size() - 1 - i];
            }
        }
    } // namespace StringFunctions
} // namespace stool
