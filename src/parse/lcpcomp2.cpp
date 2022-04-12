#include "lcpcomp2.hpp"
//#include "lcpcomp.hpp"

namespace stool{
MSFactor LCPCompData2::getFactor()
{
    uint64_t maxLCP = UINT64_MAX;
    SINDEX index = UINT64_MAX;

    index = *this->rankSet.begin();
    maxLCP = this->lcpArr[index];
    //std::cout << maxLCP << "/" << index << std::endl;
    /*
    for(SINDEX i=0;i<this->size();i++){
        if(this->lcpArr[i] != UINT64_MAX){
            if(maxLCP == UINT64_MAX || maxLCP < this->lcpArr[i]){
                maxLCP = this->lcpArr[i];
                index = i;
            }
        }
    }
    */
    if(maxLCP == 0){
        return MSFactor(this->sa[index], this->text[this->sa[index]] );
    }else{
        return MSFactor(this->sa[index], this->sa[index-1], this->lcpArr[index]);
    }
}
void LCPCompData2::initialize()
{
    stool::constructSA(this->text, this->sa, this->isa);
    stool::constructLCP(this->text, this->lcpArr, this->sa, this->isa);
    PointCompare::lcpArr = &this->lcpArr;
    PointCompare::sa = &this->sa;

    for(uint64_t i=1;i<this->text.size();i++){
        this->rankSet.insert(i);    

    }
}
void LCPCompData2::compress(LZWriter &writer)
{
    uint64_t x = 0;
    uint64_t count = 0;

    while(true){
        MSFactor f = this->getFactor();
        x += f.getLength();
        count++;
        
        //std::cout << f.toString();

        if(f.isEmpty()) break;
        writer.write2(f);
        //this->factors.push_back(f);
        this->update(f);
    }
}
void LCPCompData2::update(MSFactor f){
    if(f.isChar()){
        SINDEX i = this->isa[f.index];
        this->rankSet.erase(i);
        this->lcpArr[i] = UINT64_MAX;
    }else{
        uint64_t len = f.length;

        for(uint64_t x=0;x<len;x++){
            SINDEX i = this->isa[f.index + x];
            this->rankSet.erase(i);
            this->lcpArr[i] = UINT64_MAX;
        }

        for(uint64_t x=0;x<len;x++){
            if(f.index < x + 1) break;
            SINDEX i = this->isa[f.index - x - 1];
            if(this->lcpArr[i] > x + 1 && this->lcpArr[i] != UINT64_MAX){
                this->rankSet.erase(i);
                this->lcpArr[i] = x + 1;
                this->rankSet.insert(i);

            }
        } 

    }
}
}
std::vector<uint64_t>* stool::PointCompare::lcpArr = nullptr;
std::vector<uint64_t>* stool::PointCompare::sa = nullptr;
