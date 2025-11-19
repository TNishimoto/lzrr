#include <cassert>
#include <chrono>
#include "stool/include/third_party/cmdline.h"
#include "stool/include/debug/memory.hpp"
#include "../include/all.hpp"


using namespace std;



void compress(string &text, string mode, uint64_t threshold, uint64_t lzrrMode, stool::lzrr::LZWriter &writer)
{

    if (mode == "lz")
    {
        stool::lzrr::LZ77::compress(text, writer);
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


        stool::lzrr::LZRR::compress(text, threshold, usingLCPArray, usingDependArray, writer);
    }
    else if (mode == "lex")
    {
        stool::lzrr::LexParse::compress(text, writer);
    }else if(mode == "lexr"){
        stool::lzrr::LexParse::compressR(text, writer);
    }
    else if (mode == "lcp")
    {
        stool::lzrr::LCPCompData2::compress(text, writer);
    }
    else
    {
        std::cout << "invalid mode!" << std::endl;
        throw - 1;
    }
    writer.complete();
}
void checkFile(std::string &text, std::string filename)
{
    std::string dtext;
    //std::cout << "decompose " << std::endl;

    std::vector<stool::lzrr::LZFactor> factors;
    stool::lzrr::IO::load(filename, factors);

    stool::lzrr::MSDecompressor::decompress_factors(factors, dtext);
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
    p.add<std::string>("input_file", 'i', "input file name", true);
    p.add<std::string>("output_file", 'o', "output file name (the default output name is 'input_file.ext')", false, "");
    p.add<std::string>("mode", 'm', "compression algorithm ('lz' : LZ77, 'lex' : Lexicographic parse, 'lcp' : lcpcomp, 'lzrr' : LZRR)", false, "lzrr");
    //p.add<bool>("reverse", 'r', "use the reverse text of input text", false, false);
    p.add<bool>("file_check", 'c', "check output file", false, false);
    //p.add<uint64_t>("threshold", 't', "threshold (used in LZRR)", false, UINT64_MAX);
    //p.add<uint64_t>("lzlrmode", 'z', "lzlrmode(LZRR)", false, 0);

    p.parse_check(argc, argv);
    std::string inputFile = p.get<std::string>("input_file");
    std::string outputFile = p.get<std::string>("output_file");
    std::string mode = p.get<std::string>("mode");
    bool fileCheck = p.get<bool>("file_check");
    //bool isReverse = p.get<bool>("reverse");
    //uint64_t threshold = p.get<uint64_t>("threshold");
    //uint64_t lzrr = p.get<uint64_t>("lzlrmode");

    //uint64_t lzrr = threshold == UINT64_MAX ? 0 : 1;
    if (outputFile.size() == 0)
    {
        outputFile = inputFile + "." + mode;

    }

    std::ifstream ifs(inputFile);
    bool inputFileExist = ifs.is_open();
    if(!inputFileExist){
        std::cout << inputFile << " cannot open." << std::endl;
        return -1;
    }


    stool::lzrr::LZWriter writer;
    writer.open(outputFile);

    std::string text = "";
    std::cout << "Loading : " << inputFile << std::endl;
    stool::lzrr::IO::load(inputFile, text);

    auto start = std::chrono::system_clock::now();
    compress(text, mode, UINT64_MAX, 0, writer);
    auto end = std::chrono::system_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    if (fileCheck)
    {
        checkFile(text, outputFile);
        std::cout << "text check : " << "OK!" << std::endl;

    }
    
    std::cout << "\033[36m" << std::endl;
    std::cout << "# =============RESULT===============" << std::endl;
    std::cout << "# File : " << inputFile << std::endl;
    std::cout << "# Output : " << outputFile << std::endl;
    std::cout << "# Mode : " << mode << std::endl;
    //if(threshold != UINT64_MAX) std::cout << "Threshold : " << threshold << std::endl;
    std::cout << "# The length of the input text : " << text.size() << std::endl;
    double charperms = (double)text.size() / elapsed;
    std::cout << "# The number of factors : " << writer.counter << std::endl;
    std::cout << "# Excecution time : " << elapsed << "ms";
    std::cout << " [" << charperms << "chars/ms]" << std::endl;
    stool::Memory::print_memory_usage();
    std::cout << "# ==================================" << std::endl;
    std::cout << "\033[39m" << std::endl;
}