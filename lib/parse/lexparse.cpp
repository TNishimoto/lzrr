#include "parse/lexparse.hpp"

namespace stool
{
    namespace lzrr
    {
        void LexParse::compress(std::string &text, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa, LZWriter &writer)
        {
            uint64_t i = 0;
            while (i < text.size())
            {
                SINDEX j = isa[i];
                MSFactor f;
                uint64_t lce = j == 0 ? 0 : stool::lzrr::StringFunctions::LCE(text, sa[j], sa[j - 1]);
                if (lce == 0)
                {
                    f = MSFactor(i, text[i]);
                }
                else
                {
                    f = MSFactor(i, sa[j - 1], lce);
                }
                // std::cout << f.toString();
                writer.write(f.toLZFactor());
                i += f.getLength();
            }
        }
        void LexParse::compress(std::string &text, LZWriter &writer)
        {
            std::vector<uint64_t> sa = stool::construct_suffix_array(text);
            std::vector<uint64_t> isa;
            stool::lzrr::constructISA(text, sa, isa);

            // stool::constructSA(text, sa, isa);
            LexParse::compress(text, sa, isa, writer);
            // MSFactor::toLZFactors(d.factors, output);
        }

        void LexParse::compressR(std::string &text, std::vector<uint64_t> &sa, std::vector<uint64_t> &isa, LZWriter &writer)
        {
            uint64_t i = 0;
            uint64_t p = 0;
            uint64_t n = sa.size();
            while (i < text.size())
            {
                if (p % 10000 == 0)
                    std::cout << "\r"
                              << "Compressing the input text : [" << i << "/" << text.size() << "]" << std::flush;
                p++;

                SINDEX j = isa[i];
                MSFactor f;
                uint64_t lce = j == n - 1 ? 0 : stool::lzrr::StringFunctions::LCE(text, sa[j], sa[j + 1]);
                if (lce == 0)
                {
                    f = MSFactor(i, text[i]);
                }
                else
                {
                    f = MSFactor(i, sa[j + 1], lce);
                }
                writer.write(f.toLZFactor());
                i += f.getLength();
            }

            std::cout << std::endl;
        }
        void LexParse::compressR(std::string &text, LZWriter &writer)
        {

            std::vector<uint64_t> sa = stool::construct_suffix_array(text);
            std::vector<uint64_t> isa;
            stool::lzrr::constructISA(text, sa, isa);

            LexParse::compressR(text, sa, isa, writer);
            // MSFactor::toLZFactors(d.factors, output);
        }
    }
}