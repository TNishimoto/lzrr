
#!/bin/bash
cd `dirname $0`
mkdir ../build
mkdir ../build/pizza
mkdir ../build/rev_pizza

list=("artificial fib41" "artificial rs.13" "artificial tm29" 
"pseudo-real dblp.xml.00001.1" "pseudo-real dblp.xml.00001.2" "pseudo-real dblp.xml.0001.1" "pseudo-real dblp.xml.0001.2" 
"pseudo-real dna.001.1" "pseudo-real english.001.2" "pseudo-real proteins.001.1" "pseudo-real sources.001.2" 
"real Escherichia_Coli" "real cere" "real coreutils" "real einstein.de.txt" "real einstein.en.txt" "real influenza" "real kernel" "real para" "real world_leaders")

for item in "${list[@]}" ; do
  set -- $item
  echo $1 $2
  wget http://pizzachili.dcc.uchile.cl/repcorpus/$1/$2.gz -O ../build/pizza/$2.gz
  gunzip ../build/pizza/$2.gz
  ../build/rev.out -i ../build/pizza/$2 -o ../build/rev_pizza/$2.rev
done
