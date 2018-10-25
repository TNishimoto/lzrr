#include "lcpcomp.hpp"

namespace stool{
MSFactor LCPCompData::getFactor()
{
    uint64_t maxLCP = UINT64_MAX;
    SINDEX index = UINT64_MAX;

    for(SINDEX i=0;i<this->size();i++){
        if(this->lcpArr[i] != UINT64_MAX){
            if(maxLCP == UINT64_MAX || maxLCP < this->lcpArr[i]){
                maxLCP = this->lcpArr[i];
                index = i;
            }
        }
    }
    if(maxLCP == -1){
        return MSFactor::empty();
    }else if(maxLCP == 0){
        return MSFactor(this->sa[index], this->text[this->sa[index]] );
    }else{
        return MSFactor(this->sa[index], this->sa[index-1], this->lcpArr[index]);
    }
}
void LCPCompData::initialize()
{
    stool::constructSA(this->text, this->sa, this->isa);
    stool::constructLCP(this->text, this->lcpArr, this->sa, this->isa);
}
void LCPCompData::compress(LZWriter &writer)
{
    while(true){
        MSFactor f = this->getFactor();
        //std::cout << f.toString();

        if(f.isEmpty()) break;
        writer.write2(f);
        //this->factors.push_back(f);
        this->update(f);
    }
}
void LCPCompData::update(MSFactor f){
    if(f.isChar()){
        SINDEX i = this->isa[f.index];
        this->lcpArr[i] = UINT64_MAX;
    }else{
        uint64_t len = f.length;

        for(uint64_t x=0;x<len;x++){
            SINDEX i = this->isa[f.index + x];
            this->lcpArr[i] = UINT64_MAX;
        }

        for(uint64_t x=0;x<len;x++){
            if(f.index < x + 1) break;
            SINDEX i = this->isa[f.index - x - 1];
            if(this->lcpArr[i] > x + 1 && this->lcpArr[i] != UINT64_MAX){
                this->lcpArr[i] = x + 1;
            }
        } 

    }
}
void LCPCompData::compress(string &text, LZWriter &writer){
        
        LCPCompData d = LCPCompData();
        d.text = text;
        d.initialize();
        d.compress(writer);
        //MSFactor::toLZFactors(d.factors, output);
    }
}