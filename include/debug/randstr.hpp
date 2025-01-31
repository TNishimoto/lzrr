#pragma once
#include <vector>
#include <iostream>
#include <list>
#include <memory>
#include <random>
#include <exception>
#include <algorithm>
#include <fstream>

std::string createRandomString(int64_t len, int64_t alphabetSize, int64_t seed)
{
    std::vector<char> alphabets;
    alphabets.push_back('a');
    alphabets.push_back('b');
    alphabets.push_back('c');
    alphabets.push_back('d');
    alphabets.push_back('e');
    alphabets.push_back('f');
    alphabets.push_back('g');
    alphabets.push_back('h');
    alphabets.push_back('i');
    alphabets.push_back('j');
    alphabets.push_back('k');
    alphabets.push_back('l');
    alphabets.push_back('m');
    alphabets.push_back('n');

    std::mt19937 mt(seed);
    std::uniform_int_distribution<> rand100(0, alphabetSize - 1); // [0, 99]

    std::string r = "";
    for (int64_t i = 0; i < len; i++)
    {
        r.push_back(alphabets[rand100(mt)]);
    }
    return r;
}
bool addBit(std::vector<bool> &bits)
{
    uint64_t i=0;
    while(i < bits.size()){
        if(bits[i]){
            bits[i]=false;
            i++;
        }else{
            bits[i]=true;
            break;
        }
    }
    return i == bits.size();
}
std::string toString(std::vector<bool> &bits){
    std::string s="";
    s.resize(bits.size());
    for(uint64_t i=0;i<bits.size();i++){
        s[i] = bits[bits.size()-1-i] ? 'b' : 'a';
    }
    return s;
}


void createStringPermutation(uint64_t len, std::vector<std::string> &output){
    std::vector<bool> bits;
    bits.resize(len, false);
    //uint p=0;
    while(true){

        output.push_back(toString(bits));
        bool b = addBit(bits);
        if(b) break;
    }
}

std::string createDeBruijnSequence(uint64_t n){
    std::string s = "";
    uint64_t i=1;
    while(true){
        std::vector<std::string> seq;
        createStringPermutation(i, seq);
        for(uint64_t x=0;x<seq.size();x++){
            for(uint64_t y=0;y<seq[x].size();y++){
                s.push_back(seq[x][y]);
                if(s.size() >= n) return s;

            }
        }
        i++;
    }
    return s;
}

std::string createFibonacciWord(int length){
	std::string p0 = "a";
	std::string p1 = "b";
	std::string p2 = p1 + p0;
	int t = 1;
	while ((int)p2.size() < length)
	{
		p2 = p1 + p0;
		t++;
		p0 = p1;
		p1 = p2;
		//std::cout << p2.size() << std::endl;
	}
	return p2;
}

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