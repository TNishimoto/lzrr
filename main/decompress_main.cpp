#include <cassert>
#include <chrono>
#include "stool/include/third_party/cmdline.h"
#include "../include/lzrr.hpp"


//#include "randstr.hpp"
//#include "other/lzr.hpp"
using namespace std;
using namespace stool;

void decompressWithDelimiter(std::vector<stool::lzrr::LZFactor> &factors, std::string &output)
{
    std::string text;
    stool::lzrr::MSDecompressor::decompress_factors(factors, text);
    output = "";
    uint64_t w = 0;
    for (uint64_t i = 0; i < factors.size(); i++)
    {
        stool::lzrr::LZFactor f = factors[i];
        uint64_t len = f.getLength();
        output += text.substr(w, len);
        output.push_back('|');
        w += len;
    }
}

int main(int argc, char *argv[])
{
    //argc parse
    cmdline::parser p;

    p.add<std::string>("input_file", 'i', "input file name", true);
    p.add<std::string>("output_file", 'o', "output file name (the default output name is 'input_file.txt')", false);
    p.add<bool>("extension", 'e', "If e = 1 then this program outputs the log file. Otherwise, it decompresses the file.", false, false);

    p.parse_check(argc, argv);
    std::string filename = p.get<std::string>("input_file");
    std::string output_file = p.get<std::string>("output_file");
    bool e = p.get<bool>("extension");
    if(output_file.size() == 0){
        if(e){
            output_file = filename + ".log";
        }else{
            output_file = filename + ".txt";
        }
    }

    std::ifstream ifs(filename);
    bool inputFileExist = ifs.is_open();
    if(!inputFileExist){
        std::cout << filename << " cannot open." << std::endl;
        return -1;
    }

    //decompress
    auto start = std::chrono::system_clock::now();
    std::vector<stool::lzrr::LZFactor> factors;
    stool::lzrr::IO::load(filename, factors);
    std::string text="";
    stool::lzrr::MSDecompressor::decompress_factors(factors, text);
    //std::cout << text << std::endl;
     stool::lzrr::IO::write(output_file, text);
    auto end = std::chrono::system_clock::now();

    if(e){
        std::string delimiterText = "";
        std::string factorText = "";
        decompressWithDelimiter(factors, delimiterText);
        stool::lzrr::LZFactor::toFactorString(factors, factorText);
        if(delimiterText.size() < 1000){
            std::cout << "Text     : " << text << std::endl;
            std::cout << "Factors  : " << delimiterText << std::endl;
            std::cout << "Pointers : " << factorText << std::endl;

        }


        std::string log = text + "\n" + delimiterText + "\n" + factorText;
        stool::lzrr::IO::write(output_file, log);
    }


    //console output
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "\033[36m";
    std::cout << "=============RESULT==============="  << std::endl;
    std::cout << "File : " << filename << std::endl;
    std::cout << "Output : " << output_file << std::endl;
    std::cout << "The number of factors : " << factors.size() << std::endl;
    std::cout << "The length of the output text : " << text.size() << std::endl;    
    double charperms = (double)text.size() / elapsed;
    std::cout << "Excecution time : " << elapsed << "ms";
	std::cout << "[" << charperms << "chars/ms]" << std::endl;
    std::cout << "=================================="  << std::endl;
    std::cout << "\033[39m" << std::endl;
    
}
