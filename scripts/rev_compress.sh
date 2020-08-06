#!/bin/sh
folderpath="/mnt/server/nishimoto/text"
outputpath1="/mnt/server/nishimoto/output/lzrr"
outputpath2="/mnt/server/nishimoto/output/lz77"
outputpath3="/mnt/server/nishimoto/output/lex"

filearr=( "fib41" "rs.13" "tm29" "dblp.xml.00001.1" "dblp.xml.00001.2" 
"dblp.xml.0001.1" "dblp.xml.0001.2" "sources.001.2" "dna.001.1" "proteins.001.1" "english.001.2" "einstein.de.txt" "einstein.en.txt" "world_leaders" 
"influenza" "kernel" "cere" "coreutils" "Escherichia_Coli" "para" 
"sources" "pitches" "proteins" "dna" "english" "dblp.xml"
"sdsl-lite_202008_500MB" "samtools_202008_500MB" "boost_202008_500MB")

for file in ${filearr[@]}; do
	echo "lzrr ${file}"
    nohup /usr/bin/time -f "#lzrr ${file}, %e sec, %M KB" ~/lzrr/build/compress.out -i ${folderpath}/${file} -o ${outputpath1}/${file}.rev.lzrr -m lzrr -r 1 >> /mnt/server/nishimoto/result/lzrr_output.log
done

for file in ${filearr[@]}; do
	echo "lz77 ${file}"
    nohup /usr/bin/time -f "#lz77 ${file}, %e sec, %M KB" ~/lzrr/build/compress.out -i ${folderpath}/${file} -o ${outputpath2}/${file}.rev.lz77 -m lz -r 1 >> /mnt/server/nishimoto/result/lzrr_output.log
done

for file in ${filearr[@]}; do
	echo "lex ${file}"
    nohup /usr/bin/time -f "#lex ${file}, %e sec, %M KB" ~/lzrr/build/compress.out -i ${folderpath}/${file} -o ${outputpath3}/${file}.rev.lex -m lex -r 1 >> /mnt/server/nishimoto/result/lzrr_output.log
done



 echo "Finished."

#filearr2=( "fib41" "rs.13" "dblp.xml.00001.1" "dblp.xml.00001.2" "dblp.xml.0001.1" "dblp.xml.0001.2" "sources.001.2" "dna.001.1" "proteins.001.1" "english.001.2" "einstein.de.txt" "einstein.en.txt" "world_leaders" "influenza" "kernel" "cere" "coreutils" "Escherichia_Coli" "para")

#for file in ${filearr2[@]}; do
#	echo "esaxx ms(sdsl) ${file}"
#    nohup /usr/bin/time -f "#esaxx ms(sdsl) ${file}, %e sec, %M KB" ~/esaxx/build/enumMaximalSubstring.out -i ${folderpath}/${file} -o ${outputpath}/${file}.sdsl.msi2 -p 0 -m sdsl -u 1 >> /mnt/server/nishimoto/result/esaxx_ms_sdsl_output.log
# done
# echo "Finished."
 