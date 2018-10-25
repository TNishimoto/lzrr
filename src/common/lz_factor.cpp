#include "lz_factor.hpp"
#include <algorithm>
#include <cassert>
#include "dependency_array.hpp"

namespace stool{
LZFactor::LZFactor(uint64_t _reference, uint64_t _length)
{
    this->reference = _reference;
    this->length = _length;
}
LZFactor::LZFactor(char c)
{
    this->reference = (uint8_t)c;
    this->length = UINT64_MAX;
}
bool LZFactor::isChar()
{
    return this->length == UINT64_MAX;
}
char LZFactor::getChar()
{
    return (char)this->reference;
}


string LZFactor::toString()
{
    if (!this->isChar())
    {
        return "[" + to_string(reference) + ", " + to_string(length) + "]";
    }
    else
    {
        string s = "";
        s += (char)this->reference;
        return s;
    }
}
void MSFactor::toLZFactors(vector<MSFactor> &factors, vector<LZFactor> &output)
{
    std::sort(factors.begin(), factors.end());
    output.resize(factors.size());

    for (uint64_t i = 0; i < factors.size(); i++)
    {
        if (factors[i].isChar())
        {
            output[i] = LZFactor(factors[i].getChar());
        }
        else
        {
            output[i] = LZFactor(factors[i].reference, factors[i].length);
        }
    }
}
void MSFactor::toMSFactors(vector<LZFactor> &factors, vector<MSFactor> &output)
{
    output.resize(factors.size());
    uint64_t pos = 0;
    for (uint64_t i = 0; i < factors.size(); i++)
    {
        if (factors[i].isChar())
        {
            output[i] = MSFactor(pos, factors[i].getChar());
            pos += 1;
        }
        else
        {
            output[i] = MSFactor(pos, factors[i].reference, factors[i].length);
            pos += factors[i].length;
        }
    }
}

void MSFactor::decompose(vector<MSFactor> &factors, string &output)
{
    vector<LZFactor> lzf;
    MSFactor::toLZFactors(factors, lzf);
    LZFactor::decompress(lzf, output);
}

void LZFactor::decompress(vector<LZFactor> &factors, string &output)
{
    uint64_t len = 0;
    bool isRightReference = false;
    for (uint64_t i = 0; i < factors.size(); i++)
    {
        if (factors[i].isChar())
        {
            len += 1;
        }
        else
        {
            uint64_t pos = len;
            if (pos < factors[i].reference)
            {
                isRightReference = true;
            }
            len += factors[i].length;
        }
    }
    if (!isRightReference)
    {
        output.resize(len);
        uint64_t x = 0;
        for (uint64_t i = 0; i < factors.size(); i++)
        {
            if (factors[i].isChar())
            {
                output[x] = factors[i].getChar();
                x++;
            }
            else
            {
                for (uint64_t j = 0; j < factors[i].length; j++)
                {
                    output[x] = output[factors[i].reference + j];
                    x++;
                }
            }
        }
    }
    else
    {
        DependencyArrayManager dp = DependencyArrayManager();

        dp.initialize(len, true);
        uint64_t pos = 0;
        for (uint64_t i = 0; i < factors.size(); i++)
        {
            LZFactor f = factors[i];
            MSFactor p;
            if (f.isChar())
            {
                p = MSFactor(pos, f.getChar());
            }
            else
            {
                p = MSFactor(pos, f.reference, f.length);
            }
            dp.update(p);

            pos += f.getLength();

            if (i % 100000 == 0)
                std::cout << "\r Constructing Dependency Array : [" << pos << "/" << len << "]" << std::flush;
        }
        std::cout << std::endl;

        output.resize(len);
        for (uint64_t i = 0; i < len; i++)
        {
            output[i] = dp.getChar(i);
            if (i % 1000000== 0)
                std::cout << "\r Decompressing the compressed text : [" << i << "/" << len << "]" << std::flush;
            //std::cout << dp.dependIndexes[i] << ",";
        }
        std::cout << std::endl;
        //dp.decompress(output);
    }
}

}