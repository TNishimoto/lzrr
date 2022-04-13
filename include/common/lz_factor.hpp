#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <list>
#include <memory>
#include <stack>
#include "sa_lcp.hpp"
//#include "print.hpp"
#include <set>

namespace stool{

/*
This class represents the factor of LZ77.
*/
class LZFactor
{
  public:
    uint64_t reference;
    uint64_t length;

    LZFactor(uint64_t _reference, uint64_t _length);
    LZFactor(char c);
    LZFactor(){

    }
uint64_t getLength(){
    if(this->isChar()){
            return 1;
        }else{
            return this->length;
        }
}
    bool isChar();
    char getChar();


    std::string toString();

    static void toFactorString(std::vector<LZFactor> &input, std::string& output){
        output.clear();
        for(uint64_t i=0;i<input.size();i++){
            std::string s = input[i].toString();
            output += s;
            if(i != input.size() - 1) output += ",";
        }
    }

    static void decompress(std::vector<LZFactor> &factors, std::string &output);
    //static void decompress(std::vector<LZFactor> &factors, string &output, bool allowRightReference);
};
/*
This class represents the factor of Macro scheme.
*/
class MSFactor{
    public:
    uint64_t index;
    uint64_t reference;
    uint64_t length;
    
    MSFactor(uint64_t _index, uint64_t _reference, uint64_t _length){
        this->index = _index;
        this->reference = _reference;
        this->length = _length;
    }
    MSFactor(uint64_t _index, char c){
        this->index = _index;
        this->reference = (uint8_t)c;
        this->length = UINT64_MAX;
    }
    MSFactor(){

    }
    static MSFactor empty(){
        return MSFactor(UINT64_MAX, UINT64_MAX, 0);
    }
    uint64_t getLength(){
        if(this->isChar()){
            return 1;
        }else{
            return this->length;
        }
    }
    bool isEmpty() const {
        return this->length == 0;
    }
    bool isChar() const {
        return this->length == UINT64_MAX;
    }
    char getChar() const{
        return (char)this->reference;
    }
    std::string toString(){
        if(!this->isChar()){
            return "[" + std::to_string(index) + ", " + std::to_string(reference) + ", " + std::to_string(length) + "]"; 
        }else{
            std::string s = "";
            s += (char)this->reference;
            return s;
        }
    }
    bool operator==(const MSFactor& rh) const {
        return this->index == rh.index && this->reference == rh.reference && this->length == rh.length; 
    }
    bool operator<(const MSFactor& rh) const {
        if(this->index != rh.index){
            return this->index < rh.index;
        }else{
            if(this->reference != rh.reference){
                return this->reference < rh.reference;
            }else{
                return this->length < rh.length;
            }
        }
    }

    static void decompose(std::vector<MSFactor> &factors, std::string &output);
    LZFactor toLZFactor(){
        LZFactor f;
        f.reference = this->reference;
        f.length = this->length;
        return f;
    }
    static void toLZFactors(std::vector<MSFactor> &factors, std::vector<LZFactor> &output);

    static void toMSFactors(std::vector<LZFactor> &factors, std::vector<MSFactor> &output);

};

class LZWriter{
    public:

    std::vector<LZFactor> tmp;
    std::vector<MSFactor> tmp2;
    uint64_t counter = 0;
    std::ofstream *out;

    LZWriter(){
        this->out = NULL;
    }
    void open(std::string file){
        this->out = new std::ofstream(file, std::ios::out | std::ios::binary);
	    //std::ofstream out(outputFile, ios::out | ios::binary);
    }
    void write(LZFactor factor){
        tmp.push_back(factor);
        if(out != NULL && tmp.size() > 8092){
            out->write((const char *)(&tmp[0]), sizeof(LZFactor) * tmp.size());
            tmp.clear();
        }
        counter++;
    }
    void write2(MSFactor f){
        tmp2.push_back(f);
        counter++;
    }
    void complete(){
        if(this->tmp2.size() > 0){
            MSFactor::toLZFactors(this->tmp2, this->tmp);
            this->tmp2.clear();
            this->tmp2.shrink_to_fit();
        }

        if(out != NULL && tmp.size() > 0){
            out->write((const char *)(&tmp[0]), sizeof(LZFactor) * tmp.size());
            tmp.clear();
            out->close();
        }

    }
};


class MSFactorReferenceComp {
public:
    bool operator() (const MSFactor& lh, const MSFactor& rh) {
        if(lh.isChar() && rh.isChar()){
            return lh.getChar() < rh.getChar();
        }else if(lh.isChar()){
            return false;
        }else if(rh.isChar()){
            return true;
        }else{
            if(lh.reference != rh.reference){
                return lh.reference < rh.reference;
            }else if(lh.length != rh.length){
                return lh.length < rh.length;
            }else{
                return lh.index < rh.index;
            }
        }
    } 
};
}