# LZRR
LZRR(LZ77 with right reference) is a lossless compression algoirthm whose output file is represented by a bidirectional(macro) scheme. 
The bidirectional scheme represents a text by factors whose each factor is a character or a substring represented by a pointer to another occurrence of the substring.  
LZRR factorizes a given text T left-to-right choosing the longest substring which does not create a circular reference as new factor. 
The number of the factors is smaller than or equal to the number of factors in LZ77 factorization of the reverse text of T. 

This is implementations of LZRR, lex-parse~(https://arxiv.org/abs/1803.09517), and LZ77. 
The implementations of lex-parse and LZ77 factorizes the input text using 
the suffix array, inverse suffix array, and LCP array of the input text. 

All output files in this liblary are represented by bidirectional schemes. 
They use b * 16 bytes, where b is the number of factors.


# download
The source codes in 'module' directory are maintained in different repositories. 
So, to download all the necessary source codes, do the following:

> git clone https://github.com/TNishimoto/lzrr.git  
> cd lzrr  
> git submodule init  
> git submodule update  


# compile

> mkdir build  
> cd build  
> cmake -DCMAKE_BUILD_TYPE=Release ..  
> make  


# executions

## compression
    usage: ./compress.out --input_file=string [options] ... 
    options:
      -i, --input_file     input file name (string)
      -o, --output_file    output file name (the default output name is 'input_file.ext') (string [=])
      -m, --mode           compression algorithm ('lz' : LZ77, 'lex' : Lexicographic parse, 'lcp' : lcpcomp, 'lzrr' : LZRR) (string [=lzrr])
      -c, --file_check     check output file (bool [=0])
      -?, --help           print this message
## decompression
    usage: ./decompress.out --input_file=string [options] ...  
    options:  
      -i, --input_file     input file name (string)
      -o, --output_file    output file name (the default output name is 'input_file.txt') (string [=])
      -e, --extension      If e = 1 then this program outputs the log file. Otherwise, it decompresses the file.. (bool [=0])
      -?, --help           print this message 

## examples

    $ ./compress.out -i ../examples/text1 -m lzrr
    =============RESULT===============
    File : ./examples/text1
    Output : ./examples/text1.lzrr
    Mode : lzrr
    The length of the input text : 37
    The number of factors : 8
    Excecution time : 2ms[18.5chars/ms]
    ==================================

    $ ./decompress.out -i ../examples/text1.lzrr -e 1
    Text     : aaaaabbbababbababbabbababababababbbbb
    Factors  : aaaa|abbb|ababbab|abbab|bababababab|a|bbbb|b|
    Pointers : [1, 4],[31, 4],[13, 7],[18, 5],[22, 11],a,[33, 4],b
    /* the factor representing a substring is encoded to [the starting position of another occurrence of the substring, the length of the substring] */
    =============RESULT===============
    File : ./examples/text1.lzrr
    Output : ./examples/text1.lzrr.log
    The number of factors : 8
    The length of the output text : 37
    Excecution time : 0ms[infchars/ms]
    ==================================

    $ ./compress.out -i ../examples/text1 -m lz
    =============RESULT===============
    File : ./examples/text1
    Output : ./examples/text1.lz
    Mode : lz
    The length of the input text : 37
    The number of factors : 10
    Excecution time : 1ms[37chars/ms]
    ==================================

    $ ./decompress.out -i ../examples/text1.lz -e 1
    Text     : aaaaabbbababbababbabbababababababbbbb
    Factors  : a|aaaa|b|bb|ab|abb|ababbab|babab|abababab|bbbb|
    Pointers : a,[0, 4],b,[5, 2],[4, 2],[4, 3],[8, 7],[7, 5],[23, 8],[32, 4]
    =============RESULT===============
    File : ./examples/text1.lz
    Output : ./examples/text1.lz.log
    The number of factors : 10
    The length of the output text : 37
    Excecution time : 2ms[18.5chars/ms]
    ==================================

    $ ./compress.out -i ../examples/text1 -m lex
    =============RESULT===============
    File : ./examples/text1
    Output : ./examples/text1.lex
    Mode : lex
    The length of the input text : 37
    The number of factors : 10
    Excecution time : 2ms[18.5chars/ms]
    ==================================

    $ ./decompress.out -i ../examples/text1.lex -e 1
    Text     : aaaaabbbababbababbabbababababababbbbb
    Factors  : a|aaaa|bbb|abab|bababbab|b|a|bababababab|bbb|b|
    Pointers : a,[0, 4],[34, 3],[27, 4],[7, 8],[36, 1],[3, 1],[20, 11],[5, 3],b
    =============RESULT===============
    File : ./examples/text1.lex
    Output : ./examples/text1.lex.log
    The number of factors : 10
    The length of the output text : 37
    Excecution time : 2ms[18.5chars/ms]
    ==================================

    $ ./decompress.out -i ../examples/text1.lzrr
    =============RESULT===============
    File : ./examples/text1.lzrr
    Output : ./examples/text1.lzrr.txt
    The number of factors : 8
    The length of the output text : 37
    Excecution time : 0ms[infchars/ms]
    ==================================

## experiments

    OS: CentOS 6.9 
    CPU: quad-core Intel(R) Xeon(R) E5-2680 v2 (2.80 GHz) CPU 
    Memory: 256 GB 
    Benchmark dataset: Pizza \& Chili corpus (http://pizzachili.dcc.uchile.cl/repcorpus/)

- The number of factors (T^R is the reverse text of T)

| Text             | Text size    | LZRR(T)    | LZRR(T^R)  | LZ77(T)    | LZ77(T^R)  | LEX(T)     | LEX(T^R)   |
|------------------|--------------|------------|------------|------------|------------|------------|------------|
| fib41            | 267,914,296  | 5          | 41         | 41         | 22         | **4**          | 41         |
| rs.13            | 216,747,218  | 51         | 51         | 52         | 52         | **40**         | 40         |
| tm29             | 268,435,456  | **31**         | **31**         | 56         | 56         | 43         | 43         |
| dblp.xml.00001.1 | 104,857,600  | 55,183     | **55,166**     | 59,573     | 59,385     | 59,821     | 58,537     |
| dblp.xml.00001.2 | 104,857,600  | 56,570     | **55,156**     | 59,556     | 60,988     | 61,580     | 60,220     |
| dblp.xml.0001.1  | 104,857,600  | 73,998     | **73,602**     | 78,167     | 78,551     | 83,963     | 82,879     |
| dblp.xml.0001.2  | 104,857,600  | 87,537     | **73,602**     | 78,158     | 93,819     | 100,605    | 99,467     |
| sources.001.2    | 104,857,600  | 377,260    | **287,421**    | 294,994    | 400,953    | 466,643    | 466,074    |
| dna.001.1        | 104,857,600  | 297,707    | **295,343**    | 308,355    | 313,060    | 307,329    | 307,456    |
| proteins.001.1   | 104,857,600  | 340,629    | **337,706**    | 355,268    | 360,461    | 364,093    | 364,024    |
| english.001.2    | 104,857,600  | 377,913    | **324,238**    | 335,815    | 402,326    | 489,034    | 487,586    |
| einstein.de.txt  | 92,758,441   | **31,810**     | 32,226     | 34,572     | 34,287     | 37,721     | 37,719     |
| einstein.en.txt  | 467,626,544  | **83,353**     | 83,804     | 89,467     | 89,437     | 97,442     | 96,487     |
| world_leaders    | 46,968,181   | **165,628**    | 166,258    | 175,740    | 175,670    | 179,696    | 179,503    |
| influenza        | 154,808,555  | 720,228    | **714,289**    | 769,286    | 779,213    | 768,623    | 764,634    |
| kernel           | 257,961,616  | **741,589**    | 741,625    | 793,915    | 794,938    | 794,058    | 795,730    |
| cere             | 461,286,644  | **1,597,662**  | 1,601,629  | 1,700,630  | 1,695,631  | 1,649,448  | 1,650,242  |
| coreutils        | 205,281,778  | **1,359,787**  | 1,363,176  | 1,446,468  | 1,441,384  | 1,439,918  | 1,440,042  |
| Escherichia_Coli | 112,689,515  | 1,961,985  | **1,961,282**  | 2,078,512  | 2,078,869  | 2,014,012  | 2,014,430  |
| para             | 429,265,758  | 2,204,983  | **2,200,787**  | 2,332,657  | 2,338,919  | 2,238,362  | 2,238,716  |



## API Documentation (in preparation)

[Doxygen](https://TNishimoto.github.io/lzrr/index.html)

## Changelog

- 25/11/19 Changed the library used for constructing the suffix array from libdivsufsort to libsais.

## license

This program is released under the MIT licence.
