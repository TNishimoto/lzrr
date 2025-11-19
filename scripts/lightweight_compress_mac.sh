#!/bin/sh
inputfolderpath="/Users/nishimoto/Documents/test_data/pizza"
buildfolderpath="/Users/nishimoto/Documents/github/lzrr/build"
logfolderpath="/Users/nishimoto/Documents/github/lzrr/build"

outputpath1="/Users/nishimoto/Documents/test_data/output/lzrr"
outputpath2="/Users/nishimoto/Documents/test_data/output/lzrr"
outputpath3="/Users/nishimoto/Documents/test_data/output/lzrr"


filearr=("cere" "coreutils" "einstein.de.txt" "einstein.en.txt" "Escherichia_Coli" "influenza" "kernel" "para" "world_leaders")

for file in ${filearr[@]}; do
	echo "lzrr ${file}"
    /usr/bin/time -l  ${buildfolderpath}/compress.out -i ${inputfolderpath}/${file} -o ${outputpath1}/${file}.lzrr -m lzrr  >> ${logfolderpath}/lzrr_output2.log
done

#for file in ${filearr[@]}; do
#	echo "lz77 ${file}"
#    nohup /usr/bin/time -f "#lz77 ${file}, %e sec, %M KB" ${buildfolderpath}/compress.out -i ${inputfolderpath}/${file} -o ${outputpath2}/${file}.lz77 -m lz >> ${logfolderpath}/lzrr_output.log
#done

#for file in ${filearr[@]}; do
#	echo "lex ${file}"
#    nohup /usr/bin/time -f "#lex ${file}, %e sec, %M KB" ${buildfolderpath}/compress.out -i ${inputfolderpath}/${file} -o ${outputpath3}/${file}.lex -m lex >> ${logfolderpath}/lzrr_output.log
#done



 echo "Finished."

 