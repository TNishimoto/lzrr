#include "lnf_array.hpp"

namespace stool{
        LNFInfo::LNFInfo(SINDEX _begin, SINDEX _end, uint64_t upperLCP, uint64_t lowerLCP)
    {
        this->begin = _begin;
        this->end = _end;

        if (this->begin != UINT64_MAX && this->end != UINT64_MAX)
        {
            if (upperLCP > lowerLCP)
            {
                this->isBeg = true;
                this->lcp = upperLCP;
            }
            else
            {
                this->isBeg = false;
                this->lcp = lowerLCP;
            }
        }
        else if (this->begin != UINT64_MAX)
        {
            this->isBeg = true;
            this->lcp = upperLCP;
        }
        else if (this->end != UINT64_MAX)
        {
            this->isBeg = false;
            this->lcp = lowerLCP;
        }
        else
        {
            this->lcp = 0;
        }
    }
    TINDEX LNFInfo::reference(vector<uint64_t> &sa)
    {
        if (this->isBeg)
        {
            if (this->begin < sa.size())
            {
                return sa[this->begin];
            }
            else
            {
                return UINT64_MAX;
            }
        }
        else
        {
            if (this->end < sa.size())
            {
                return sa[this->end];
            }
            else
            {
                return UINT64_MAX;
            }
        }
    }
uint64_t LNFInfo::getArea()
    {
        int64_t _beg = this->begin == UINT64_MAX ? 0 : this->begin;
        int64_t tate = (this->end - _beg - 2);
        if (tate <= 0)
        {
            return 0;
        }
        else
        {
            int64_t pSize = tate * (this->lcp + 1);
            return pSize;
        }
    }
namespace LNF
{


void computeLNFArray(string &text, vector<uint64_t> &sa, vector<uint64_t> &lcpArr, uint64_t threshold, vector<TINDEX> &output)
{
    uint64_t n = sa.size();

    vector<uint64_t> lowerLCPvec;
    vector<bool> checker;

    output.resize(n, 0);
    lowerLCPvec.resize(n, 0);
    checker.resize(n, false);

    uint64_t counterMax = 2 * n;
    uint64_t counter = 0;

    std::stack<std::pair<SINDEX, uint64_t>> stack;
    uint64_t lcp = 0;
    for (uint64_t i = 0; i < n; i++)
    {
        lcp = lcpArr[i];
        while (stack.size() > 0)
        {
            auto top = stack.top();
            uint64_t topIndex = top.first;
            lcp = top.second < lcp ? top.second : lcp;
            if (sa[topIndex] < sa[i] || lcp == 0)
            {
                output[topIndex] = i;
                lowerLCPvec[topIndex] = lcp;
                stack.pop();
            }
            else
            {
                stack.pop();
                stack.push(std::pair<SINDEX, uint64_t>(topIndex, lcp));
                break;
            }
        }
        stack.push(std::pair<SINDEX, uint64_t>(i, UINT64_MAX));

        if (counter % 1000000 == 0)
            std::cout << "\r"
                      << "Constructing LNF array : [" << counter << "/" << counterMax << "]" << std::flush;
        counter++;
    }
    while (stack.size() > 0)
    {
        auto top = stack.top();
        uint64_t topIndex = top.first;
        output[topIndex] = n;
        stack.pop();
    }

    for (int64_t i = n - 1; i >= 0; i--)
    {
        lcp = i == n - 1 ? 0 : lcpArr[i + 1];
        while (stack.size() > 0)
        {
            auto top = stack.top();
            uint64_t topIndex = top.first;
            lcp = top.second < lcp ? top.second : lcp;
            if (sa[topIndex] < sa[i] || lcp == 0)
            {
                LNFInfo inf(i, output[topIndex], lcp, lowerLCPvec[topIndex]);
                if (inf.getArea() > threshold)
                {
                    output[topIndex] = inf.reference(sa);
                }
                else
                {
                    output[topIndex] = UINT64_MAX;
                }
                checker[topIndex] = true;
                stack.pop();
            }
            else
            {
                stack.pop();
                stack.push(std::pair<SINDEX, uint64_t>(topIndex, lcp));
                break;
            }
        }
        stack.push(std::pair<SINDEX, uint64_t>(i, UINT64_MAX));

        if (counter % 1000000 == 0)
            std::cout << "\r" << "Constructing LNF array : ["  << counter << "/" << counterMax << "]" << std::flush;
        counter++;
    }
    while (stack.size() > 0)
        stack.pop();

    for (uint64_t i = 0; i < n; i++)
    {
        if (!checker[i])
            output[i] = UINT64_MAX;
    }
    std::cout << std::endl;
}
void computeLNFArray(string &text, vector<uint64_t> &sa, vector<uint64_t> &lcpArr, vector<LNFInfo> &output)
{
    //vector<uint64_t> upperLNFIndexvec, lowerLNFIndexvec, upperLNFLCPvec, lowerLNFLCPvec, ;
    uint64_t n = sa.size();

    vector<uint64_t> upperLCPvec, lowerLCPvec;

    output.resize(n);
    upperLCPvec.resize(n, 0);
    lowerLCPvec.resize(n, 0);

    uint64_t counterMax = 3 * n;
    uint64_t counter = 0;

    std::stack<std::pair<SINDEX, uint64_t>> stack;
    uint64_t lcp = 0;
    for (uint64_t i = 0; i < n; i++)
    {
        lcp = lcpArr[i];
        while (stack.size() > 0)
        {
            auto top = stack.top();
            uint64_t topIndex = top.first;
            lcp = top.second < lcp ? top.second : lcp;
            if (sa[topIndex] < sa[i] || lcp == 0)
            {
                output[topIndex].end = i;
                lowerLCPvec[topIndex] = lcp;
                stack.pop();
            }
            else
            {
                stack.pop();
                stack.push(std::pair<SINDEX, uint64_t>(topIndex, lcp));
                break;
            }
        }
        stack.push(std::pair<SINDEX, uint64_t>(i, UINT64_MAX));

        if (counter % 1000000 == 0)
            std::cout << "\r"
                      << "[" << counter << "/" << counterMax << "]" << std::flush;
        counter++;
    }
    while (stack.size() > 0)
    {
        auto top = stack.top();
        uint64_t topIndex = top.first;
        output[topIndex].end = n;
        stack.pop();
    }

    for (int64_t i = n - 1; i >= 0; i--)
    {
        lcp = i == n - 1 ? 0 : lcpArr[i + 1];
        while (stack.size() > 0)
        {
            auto top = stack.top();
            uint64_t topIndex = top.first;
            lcp = top.second < lcp ? top.second : lcp;
            if (sa[topIndex] < sa[i] || lcp == 0)
            {
                output[topIndex].begin = i;
                upperLCPvec[topIndex] = lcp;
                stack.pop();
            }
            else
            {
                stack.pop();
                stack.push(std::pair<SINDEX, uint64_t>(topIndex, lcp));
                break;
            }
        }
        stack.push(std::pair<SINDEX, uint64_t>(i, UINT64_MAX));

        if (counter % 1000000 == 0)
            std::cout << "\r"
                      << "[" << counter << "/" << counterMax << "]" << std::flush;
        counter++;
    }
    while (stack.size() > 0)
        stack.pop();

    for (uint64_t i = 0; i < n; i++)
    {
        if (output[i].begin != UINT64_MAX && output[i].end != UINT64_MAX)
        {
            if (upperLCPvec[i] > lowerLCPvec[i])
            {
                output[i].isBeg = true;
                output[i].lcp = upperLCPvec[i];
            }
            else
            {
                output[i].isBeg = false;
                output[i].lcp = lowerLCPvec[i];
            }
        }
        else if (output[i].begin != UINT64_MAX)
        {
            output[i].isBeg = true;
            output[i].lcp = upperLCPvec[i];
        }
        else if (output[i].end != UINT64_MAX)
        {
            output[i].isBeg = false;
            output[i].lcp = lowerLCPvec[i];
        }
        else
        {
            output[i].lcp = 0;
        }

        if (counter % 1000000 == 0)
            std::cout << "\r"
                      << "[" << counter << "/" << counterMax << "]" << std::flush;
        counter++;
    }
    std::cout << std::endl;
}
void computeLNFArrayForDebug(string &text, vector<uint64_t> &sa, vector<uint64_t> &lcpArr, uint64_t threshold, vector<TINDEX> &output)
{
    vector<LNFInfo> vec;
    computeLNFArray(text, sa, lcpArr, vec);
    output.resize(sa.size(), UINT64_MAX);
    for (uint64_t i = 0; i < sa.size(); i++)
    {
        if (vec[i].getArea() > threshold)
        {
            //std::cout << vec[i].begin << "/" << vec[i].end << "/" << vec[i].lcp << std::endl;
            output[i] = vec[i].reference(sa);
        }
    }
}
} // namespace LNF
}