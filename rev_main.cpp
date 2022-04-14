#include <cassert>
#include <chrono>
#include "stool/include/cmdline.h"
#include "common/io.h"
#include "common/lz_factor.hpp"
#include "stool/include/string_functions.hpp"
int main(int argc, char *argv[])
{
    //argc parse
    cmdline::parser p;

    p.add<std::string>("input_file", 'i', "input file name", true);
    p.add<std::string>("output_file", 'o', "output file name (the default output name is 'input_file.txt')", false);

    p.parse_check(argc, argv);
    std::string input_file = p.get<std::string>("input_file");
    std::string output_file = p.get<std::string>("output_file");
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

    std::string text = "";
    std::cout << "Loading: " << input_file << std::endl;
    stool::lzrr::IO::load(input_file, text);
    stool::StringFunctions::reverse(text);

    stool::lzrr::IO::write(output_file, text);
    std::cout << "Wrote: " << output_file << std::endl;

}
