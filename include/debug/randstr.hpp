#pragma once
#include <vector>
#include <iostream>
#include <list>
#include <memory>
#include <random>
#include <exception>
#include <algorithm>
#include <fstream>

std::string createRandomString(int64_t len, int64_t alphabetSize, int64_t seed);
bool addBit(std::vector<bool> &bits);
std::string toString(std::vector<bool> &bits);

void createStringPermutation(uint64_t len, std::vector<std::string> &output);

std::string createDeBruijnSequence(uint64_t n);

std::string createFibonacciWord(int length);

namespace stool
{

    namespace lzrr
    {
        template <class X>
        bool equalCheck(std::vector<X> &item1, std::vector<X> &item2)
        {
            if (item1.size() != item2.size())
            {
                return false;
            }
            else
            {
                for (uint64_t i = 0; i < item1.size(); i++)
                {
                    if (item1[i] != item2[i])
                        return false;
                }
                return true;
            }
        }
    }
} // namespace stool