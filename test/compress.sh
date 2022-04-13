#!/bin/sh
cd `dirname $0`
mkdir ../build/output
mkdir ../build/logs

folderpath="../build/pizza"
outputpath="../build/output"
logpath="../build/logs"


filearr=( "fib41" "rs.13" "tm29" "dblp.xml.00001.1" "dblp.xml.00001.2" 
"dblp.xml.0001.1" "dblp.xml.0001.2" "sources.001.2" "dna.001.1" "proteins.001.1" "english.001.2" "einstein.de.txt" "einstein.en.txt" "world_leaders" 
"influenza" "kernel" "cere" "coreutils" "Escherichia_Coli" "para")

for file in ${filearr[@]}; do
	echo "lzrr ${file}"
    nohup /usr/bin/time -f "#lzrr ${file}, %e sec, %M KB" ../build/compress.out -i ${folderpath}/${file} -o ${outputpath}/${file}.lzrr -m lzrr >> ${logpath}/lzrr_output.log
done

for file in ${filearr[@]}; do
	echo "lzrr(rev) ${file}"
    nohup /usr/bin/time -f "#lzrr(rev) ${file}, %e sec, %M KB" ../build/compress.out -i ${folderpath}/${file}.rev -o ${outputpath}/${file}.rev.lzrr -m lzrr >> ${logpath}/lzrr_rev_output.log
done


for file in ${filearr[@]}; do
	echo "lz77 ${file}"
    nohup /usr/bin/time -f "#lz77 ${file}, %e sec, %M KB" ../build/compress.out -i ${folderpath}/${file} -o ${outputpath}/${file}.lz77 -m lz >> ${logpath}/lz77_output.log
done

for file in ${filearr[@]}; do
	echo "lz77(rev) ${file}"
    nohup /usr/bin/time -f "#lz77(rev) ${file}, %e sec, %M KB" ../build/compress.out -i ${folderpath}/${file}.rev -o ${outputpath}/${file}.rev.lz77 -m lz >> ${logpath}/lz77_rev_output.log
done


for file in ${filearr[@]}; do
	echo "lex ${file}"
    nohup /usr/bin/time -f "#lex ${file}, %e sec, %M KB" ../build/compress.out -i ${folderpath}/${file} -o ${outputpath}/${file}.lex -m lex >> ${logpath}/lex_output.log
done

for file in ${filearr[@]}; do
	echo "lex(rev) ${file}"
    nohup /usr/bin/time -f "#lex(rev) ${file}, %e sec, %M KB" ../build/compress.out -i ${folderpath}/${file}.rev -o ${outputpath}/${file}.rev.lex -m lex >> ${logpath}/lex_rev_output.log
done


echo "Finished."
