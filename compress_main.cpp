#include <cassert>
#include <chrono>
#include "cmdline.h"
#include "io.h"
#include "lz_factor.hpp"
#include "randstr.hpp"
#include "lzrr.hpp"
#include "lz77.hpp"

//#include "other_lz.hpp"
//#include "lzr.hpp"
#include "lcpcomp.hpp"
#include "lcpcomp2.hpp"

#include "lexparse.hpp"

using namespace std;

void compress(string &text, string mode, uint64_t threshold, uint64_t lzrrMode, LZWriter &writer)
{

    if (mode == "lz")
    {
        LZ77::compress(text, writer);
    }
    else if (mode == "lzrr")
    {
        bool usingLCPArray = true;
        if(lzrrMode == 0 || lzrrMode == 1){
            usingLCPArray = true;
        }else{
            usingLCPArray = false;
        }
        bool usingDependArray = true;
        if(lzrrMode == 0 || lzrrMode == 2){
            usingDependArray = true;
        }else{
            usingDependArray = false;
        }


        LZRR::compress(text, threshold, usingLCPArray, usingDependArray, writer);
    }
    else if (mode == "lex")
    {
        LexParse::compress(text, writer);
    }else if(mode == "lexr"){
        LexParse::compressR(text, writer);
    }
    else if (mode == "lcp")
    {
        LCPCompData::compress(text, writer);
    }
    else
    {
        std::cout << "invalid mode!" << std::endl;
        throw - 1;
    }
    writer.complete();
}
void checkFile(string &text, string filename)
{
    string dtext;
    //std::cout << "decompose " << std::endl;

    vector<LZFactor> factors;
    stool::IO::load(filename, factors);

    LZFactor::decompress(factors, dtext);
    if (text.size() != dtext.size())
    {
        std::cout << "differrent size!" << std::endl;
        throw - 1;
    }
    for (uint64_t x = 0; x < text.size(); x++)
    {
        if (text[x] != dtext[x])
        {

            std::cout << "error decompress" << std::endl;
            throw - 1;
        }
    }
}

int main(int argc, char *argv[])
{
    #ifdef DEBUG
    std::cout << "\033[41m";
    std::cout << "DEBUG MODE!" << std::endl;
    std::cout << "\e[m" << std::endl;
    //std::cout << "\033[30m" << std::endl;

    #endif
    cmdline::parser p;
    p.add<string>("input_file", 'i', "input file name", true);
    p.add<string>("output_file", 'o', "output file name (the default output name is 'input_file.ext')", false, "");
    p.add<string>("mode", 'm', "compression algorithm ('lz' : LZ77, 'lex' : Lexicographic parse, 'lcp' : lcpcomp, 'lzrr' : LZRR)", false, "lzrr");
    p.add<bool>("reverse", 'r', "use the reverse text of input text", false, false);
    p.add<bool>("file_check", 'c', "check output file", false, false);
    p.add<uint64_t>("threshold", 't', "threshold (used in LZRR)", false, UINT64_MAX);
    //p.add<uint64_t>("lzlrmode", 'z', "lzlrmode(LZRR)", false, 0);

    p.parse_check(argc, argv);
    string inputFile = p.get<string>("input_file");
    string outputFile = p.get<string>("output_file");
    string mode = p.get<string>("mode");
    bool fileCheck = p.get<bool>("file_check");
    bool isReverse = p.get<bool>("reverse");
    uint64_t threshold = p.get<uint64_t>("threshold");
    //uint64_t lzrr = p.get<uint64_t>("lzlrmode");

    uint64_t lzrr = threshold == UINT64_MAX ? 0 : 1;
    if (outputFile.size() == 0)
    {
        if(mode == "lzrr" && threshold != UINT64_MAX){
            outputFile = inputFile + (isReverse ? "_rev" : "") + "_t" + to_string(threshold) + "." + mode;
        }else{
            outputFile = inputFile + (isReverse ? "_rev" : "") + "." + mode;
        }
    }

    std::ifstream ifs(inputFile);
    bool inputFileExist = ifs.is_open();
    if(!inputFileExist){
        std::cout << inputFile << " cannot open." << std::endl;
        return -1;
    }


    LZWriter writer;
    writer.open(outputFile);
	//ofstream out(outputFile, ios::out | ios::binary);

    string text = "";
    std::cout << "Loading : " << inputFile << std::endl;
    stool::IO::load(inputFile, text);
    if(isReverse)stool::StringFunctions::reverse(text);

    //vector<LZFactor> factors;

    auto start = std::chrono::system_clock::now();
    compress(text, mode, threshold,lzrr, writer);
    auto end = std::chrono::system_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    //std::cout << "write : " << outputFile << std::endl;
    //stool::IO::write(outputFile, factors);
    
    if (fileCheck)
    {
        checkFile(text, outputFile);
        std::cout << "text check : " << "OK!" << std::endl;

    }
    
    std::cout << "\033[36m";
    std::cout << "=============RESULT===============" << std::endl;
    std::cout << "File : " << inputFile << std::endl;
    std::cout << "Output : " << outputFile << std::endl;
    std::cout << "Mode : " << mode << std::endl;
    if(threshold != UINT64_MAX) std::cout << "Threshold : " << threshold << std::endl;
    /*
    if(mode == "lzrr"){
        string p1 = (lzrr == 0 || lzrr == 1) ? "yes" : "no";
        string p2 = (lzrr == 0 || lzrr == 2) ? "yes" : "no";

        std::cout << "using LCP Array : " << p1 << std::endl;
        std::cout << "using Depend Array : " << p2 << std::endl;

    }
    */
    std::cout << "The length of the input text : " << text.size() << std::endl;
    double charperms = (double)text.size() / elapsed;
    std::cout << "The number of factors : " << writer.counter << std::endl;
    std::cout << "Excecution time : " << elapsed << "ms";
    std::cout << "[" << charperms << "chars/ms]" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "\033[39m" << std::endl;
}