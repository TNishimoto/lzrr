#pragma once
#include <vector>
#include <iostream>
#include <list>
#include <memory>
#include <random>
#include <exception>
#include <algorithm>
#include <fstream>

using namespace std;
string createRandomString(int64_t len, int64_t alphabetSize, int64_t seed);
bool addBit(vector<bool> &bits);
string toString(vector<bool> &bits);

void createStringPermutation(uint64_t len, vector<string> &output);

string createDeBruijnSequence(uint64_t n);

string createFibonacciWord(int length);

namespace stool
{
template <class X>
bool equalCheck(vector<X> &item1, vector<X> &item2)
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
} // namespace stool