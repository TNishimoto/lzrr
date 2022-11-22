#include <cassert>
#include <chrono>
#include "src/common/cmdline.h"
#include "src/common/io.h"
#include "src/common/lz_factor.hpp"
//#include "randstr.hpp"
//#include "other/lzr.hpp"
using namespace std;
using namespace stool;

int main(int argc, char *argv[])
{
    //argc parse
    cmdline::parser p;

    p.add<string>("input_file", 'i', "input file name", true);
    p.add<string>("output_file", 'o', "output file name (the default output name is 'input_file.txt')", false);

    p.parse_check(argc, argv);
    string input_file = p.get<string>("input_file");
    string output_file = p.get<string>("output_file");
    if (output_file.size() == 0)
    {
        output_file = input_file + ".rev";
    }

    std::ifstream ifs(input_file);
    bool inputFileExist = ifs.is_open();
    if (!inputFileExist)
    {
        std::cout << input_file << " cannot open." << std::endl;
        return -1;
    }

    string text = "";
    std::cout << "Loading: " << input_file << std::endl;
    stool::IO::load(input_file, text);
    stool::StringFunctions::reverse(text);

    stool::IO::write(output_file, text);
    std::cout << "Wrote: " << output_file << std::endl;

}
