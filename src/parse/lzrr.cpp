
#include "lzrr.hpp"
#include "debug/print.hpp"
#include "lp.hpp"

namespace stool
{
void LZRR::initialize(uint64_t _threshold, bool usingLCPArray, bool usingDependArray)
{
    constructSA(*this->text, this->sa, this->isa);

    if (usingLCPArray)
    {
        constructLCP(*this->text, this->lcpArr, this->sa, this->isa);
    }

    this->threshold = _threshold;
    if (this->threshold != UINT64_MAX)
    {
        if (this->lcpArr.size() == 0)
        {
            constructLCP(*this->text, this->lcpArr, this->sa, this->isa);
        }
        LNF::computeLNFArray(*this->text, this->sa, this->lcpArr, this->threshold, this->longestRightReferenceArr);
    }
    if (!usingLCPArray && this->lcpArr.size() > 0)
    {
        this->lcpArr.resize(0);
        this->lcpArr.shrink_to_fit();
    }

    std::cout << "Initializing the dependency array manager...";
    this->depper.initialize(this->size(), usingDependArray);
    std::cout << "[END]" << std::endl;
}
MSFactor LZRR::getLZRRFactor(SINDEX i)
{
    TINDEX x = this->sa[i];
    std::pair<SINDEX, uint64_t> z = LZRR::getLongestOccurrence(i, this->lcpArr.size() == 0 ? NULL : &this->lcpArr, this->depper, this->sa, this->isa, *this->text);
    return z.second == 0 ? MSFactor(x, (*this->text)[x]) : MSFactor(x, this->sa[z.first], z.second);
}

void LZRR::compress(LZWriter &writer)
{
    uint64_t nowPosition = 0;
    uint64_t p = 0;
    while (nowPosition < this->size())
    {
        if (p % 10000 == 0)
            std::cout << "\r"
                      << "compressing text.. : [" << nowPosition << "/" << this->size() << "]" << std::flush;

        //auto startLZ = std::chrono::system_clock::now();
        SINDEX i = this->isa[nowPosition];
        MSFactor f;
        if (this->threshold == UINT64_MAX)
        {
            f = this->getLZRRFactor(i);
        }
        else
        {
            if (this->longestRightReferenceArr[i] == UINT64_MAX)
            {
                f = this->getLZRRFactor(i);
            }
            else
            {
                TINDEX refIndex = this->longestRightReferenceArr[i];
                uint64_t lce = stool::StringFunctions::LCE(*this->text, nowPosition, refIndex);
                if (lce == 0)
                {
                    f = MSFactor(nowPosition, (*this->text)[nowPosition]);
                }
                else
                {
                    f = MSFactor(nowPosition, refIndex, lce);
                }
            }
        }

        //this->factors.push_back(f.toLZFactor());
        this->depper.update(f);
        nowPosition += f.getLength();
        p++;
        writer.write(f.toLZFactor());
    }
    std::cout << std::endl;
}
/*
std::pair<SINDEX, uint64_t> LZRR::getLongestOccurrence(SINDEX i, vector<uint64_t> *lcpArr, DependencyArrayManager &depper, vector<uint64_t> &sa, vector<uint64_t> &isa, string &text)
{
    uint64_t n = sa.size();
    uint64_t lfLength = 0;
    SINDEX index = UINT64_MAX;

    int64_t lowerIndex = i + 1;
    int64_t upperIndex = i - 1;
    uint64_t lowerLCP = lowerIndex == n ? 0 : lcpArr == NULL ? stool::StringFunctions::LCE(text, sa[i], sa[lowerIndex]) : (*lcpArr)[lowerIndex];
    uint64_t upperLCP = upperIndex < 0 ? 0 : lcpArr == NULL ? stool::StringFunctions::LCE(text, sa[i], sa[upperIndex]) : (*lcpArr)[i];

    uint64_t counter = 0;
    uint64_t counterX = 0;
    uint64_t sumLCP = 0;

    while (true)
    {
        counter++;
        uint64_t maxLCP = lowerLCP <= upperLCP ? upperLCP : lowerLCP;
        if (lfLength >= maxLCP)
        {
            break;
        }
        MSFactor f;
        uint64_t y, nowLF = 0;

        uint64_t factorLength = lowerLCP <= upperLCP ? upperLCP : lowerLCP;
        y = lowerLCP <= upperLCP ? upperIndex : lowerIndex;


        f = MSFactor(sa[i], sa[y], factorLength);

        if (f.reference > f.index)
        {
            nowLF = f.length;
        }
        else
        {
            uint64_t fakeLen = depper.getFakeLCPWR(f);

            if (fakeLen > lfLength)
            {
                nowLF = depper.getLCPWR(f);
            }
        }

        if (lfLength < nowLF)
        {
            lfLength = nowLF;
            index = y;
        }
        if (lowerLCP <= upperLCP)
        {
            upperIndex--;
            upperLCP = upperIndex < 0 ? 0 : lcpArr == NULL ? stool::StringFunctions::LCE(text, sa[i], sa[upperIndex]) : (*lcpArr)[upperIndex + 1] < upperLCP ? (*lcpArr)[upperIndex + 1] : upperLCP;
        }
        else
        {
            lowerIndex++;
            lowerLCP = lowerIndex == n ? 0 : lcpArr == NULL ? stool::StringFunctions::LCE(text, sa[i], sa[lowerIndex]) : (*lcpArr)[lowerIndex] < lowerLCP ? (*lcpArr)[lowerIndex] : lowerLCP;
        }
    }
    return std::pair<SINDEX, uint64_t>(index, lfLength);
}
*/



std::pair<SINDEX, uint64_t> LZRR::getLongestOccurrence(SINDEX i, vector<uint64_t> *lcpArr, DependencyArrayManager &depper, vector<uint64_t> &sa, vector<uint64_t> &isa, string &text)
{
    LPIterater lpi = LPIterater(i, &sa, &isa, lcpArr);
    uint64_t n = sa.size();
    uint64_t longestFactorLength = 0;
    SINDEX index = UINT64_MAX;

    while (true)
    {
        uint64_t jFactorLength = 0;
        std::pair<SINDEX, uint64_t> candidate = lpi.take();
        uint64_t lcp = candidate.second;
        uint64_t j = candidate.first;
        if (longestFactorLength >= lcp)
        {
            break;
        }

        MSFactor f = MSFactor(sa[i], sa[j], lcp);
        if (f.reference > f.index)
        {
            jFactorLength = f.length;
        }
        else
        {
            uint64_t fakeLen = depper.getFakeLCPWR(f);

            if (fakeLen > longestFactorLength)
            {
                jFactorLength = depper.getLCPWR(f);
            }
        }
        if (longestFactorLength < jFactorLength)
        {
            longestFactorLength = jFactorLength;
            index = j;
        }
    }
    return std::pair<SINDEX, uint64_t>(index, longestFactorLength);
}


} // namespace stool