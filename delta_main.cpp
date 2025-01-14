#include <cassert>
#include <chrono>
#include <random>
#include <algorithm>
#include <set>
#include "stool/include/light_stool.hpp"

#include "libdivsufsort/sa.hpp"

#include <unordered_map>

// #include "esaxx/src/minimal_substrings/minimal_substring_tree.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // using CHAR = uint8_t;
#ifdef DEBUG
    std::cout << "\033[41m";
    std::cout << "DEBUG MODE!";
    std::cout << "\e[m" << std::endl;
// std::cout << "\033[30m" << std::endl;
#endif
    cmdline::parser p;
    p.add<string>("input_filepath", 'i', "Input text file name", true);
    p.add<bool>("verification", 'v', "verification", false, false);
    p.add<string>("output_filepath", 'o', "Output filepath", false, "");

    p.parse_check(argc, argv);
    string input_filepath = p.get<string>("input_filepath");
    string output_filepath = p.get<string>("output_filepath");

    bool verification = p.get<bool>("verification");

    std::vector<char> text;
    stool::IO::load(input_filepath, text);
    std::cout << "Constructing Suffix Array" << std::flush;
    std::vector<uint64_t> sa = libdivsufsort::construct_suffix_array(text);
    std::cout << "[END]" << std::endl;

    std::vector<uint64_t> lcp_array = stool::construct_LCP_array(text, sa);

    std::cout << "Computing delta..." << std::flush;
    std::vector<uint64_t> distinct_substring_counter_array = stool::SubstringComplexityFunctions::construct_distinct_substring_counter_array(lcp_array);
    uint64_t delta = stool::SubstringComplexityFunctions::compute_delta(distinct_substring_counter_array);
    std::cout << "[END]" << std::endl;

    std::cout << "\033[32m";
    std::cout << "______________________INFO______________________" << std::endl;
    std::cout << "File name \t : " << input_filepath << std::endl;

    std::cout << "Text length \t : " << text.size() << std::endl;
    std::cout << "Delta \t \t : " << delta << std::endl;
    std::cout << "_______________________________________________________" << std::endl;
    std::cout << "\033[39m" << std::endl;

    if (output_filepath.size() != 0)
    {
        std::cout << "Writing: " << output_filepath << std::flush;

        std::ofstream outputfile(output_filepath);
        outputfile << "i"
                   << ", "
                   << "distinct substrings"
                   << ", "
                   << "distinct substrings / i" << std::endl;

        for (uint64_t i = 0; i < distinct_substring_counter_array.size(); i++)
        {
            uint64_t d = i == 0 ? 0 : distinct_substring_counter_array[i] / i;
            outputfile << i << ", " << distinct_substring_counter_array[i] << ", " << d << std::endl;
        }
        std::cout << "[END]" << std::endl;
    }

    if (verification)
    {
        std::cout << "Verification" << std::endl;
        std::vector<uint64_t> distinct_substring_counter_array2 = stool::SubstringComplexityFunctions::construct_distinct_substring_counter_array(text, sa);
        bool b = stool::equal_check(distinct_substring_counter_array, distinct_substring_counter_array2);
        if (b)
        {
            std::cout << "OK!" << std::endl;
        }
        else
        {
            std::cout << "Error!" << std::endl;

            throw -1;
        }
    }

    return 0;
}
