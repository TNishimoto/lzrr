#include "lz77.hpp"

namespace stool{
void LZ77::constructLPFUDArray(vector<uint64_t> &sa, vector<uint64_t> &lpfu, vector<uint64_t> &lpfd)
{
    lpfd.resize(sa.size(), UINT64_MAX);
    lpfu.resize(sa.size(), UINT64_MAX);

    std::stack<uint64_t> stack;
    for (uint64_t i = 0; i < sa.size(); i++)
    {
        while (stack.size() > 0)
        {
            uint64_t top = stack.top();
            if (top > sa[i])
            {
                lpfd[top] = sa[i];
                stack.pop();
            }
            else
            {
                break;
            }
        }
        stack.push(sa[i]);
    }
    while (stack.size() > 0)
        stack.pop();

    for (int64_t i = sa.size() - 1; i >= 0; i--)
    {
        while (stack.size() > 0)
        {
            uint64_t top = stack.top();
            if (top > sa[i])
            {
                lpfu[top] = sa[i];
                stack.pop();
            }
            else
            {
                break;
            }
        }
        stack.push(sa[i]);
    }
}

void LZ77::compress(string &text, vector<uint64_t> &lpf, LZWriter &writer)
{
    uint64_t i = 0;
    while (i < text.size())
    {
        if (lpf[i] == UINT64_MAX)
        {
            writer.write(LZFactor(text[i]));
            i++;
        }
        else
        {
            uint64_t max = text.size() - i;
            uint64_t j = lpf[i];
            uint64_t lce = stool::StringFunctions::LCE(text, j, i);
            writer.write(LZFactor(j, lce));
            i += lce;
        }
    }
}

void LZ77::compress(string &text, vector<uint64_t> &lpfu, vector<uint64_t> &lpfd, LZWriter &writer)
{
    uint64_t i = 0;
    while (i < text.size())
    {
        uint64_t lceu = lpfu[i] == UINT64_MAX ? 0 : stool::StringFunctions::LCE(text, i, lpfu[i]);
        uint64_t lced = lpfd[i] == UINT64_MAX ? 0 : stool::StringFunctions::LCE(text, i, lpfd[i]);
        uint64_t j = lceu > lced ? lpfu[i] : lpfd[i];
        uint64_t lce = lceu > lced ? lceu : lced;
        if (lce == 0)
        {
            writer.write(LZFactor(text[i]));
            i++;
        }
        else
        {
            writer.write(LZFactor(j, lce));
            i += lce;
        }
    }
}
void LZ77::compress(string &text, LZWriter &writer)
{
    vector<uint64_t> sa, lpfu, lpfd;
    stool::constructSA(text, sa);
    std::cout << "constructing LPFUD...";
    constructLPFUDArray(sa, lpfu, lpfd);
    std::cout << "[END]" << std::endl;

    std::cout << "Computing factors...";
    compress(text, lpfu, lpfd, writer);
    std::cout << "[END]" << std::endl;
}
}