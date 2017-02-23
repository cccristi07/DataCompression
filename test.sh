#!/bin/bash
echo "Script testare temaPS"


#extensii pentru fisierele folosite
huffman_extension='.hff'
shannonfano_extension='.sf'



#folderele unde punem fisierele arhivate
hcomp='huffman_comp/'
sfcomp='shannonfano_comp/'

#folderele unde punem fisierele dezarhivate
hdcomp='huffman_decomp/'
sfdcomp='shannonfano_decomp/'

# creem folderele numai daca acestea exista
mkdir -p ${hcomp}
mkdir -p ${hdcomp}
mkdir -p ${sfcomp}
mkdir -p ${sfdcomp}


cd tests/

echo "Arhivam cu metoda Huffman"
for file in *
do
	
	arch_file_h="../"${hcomp}${file}${huffman_extension}
	
	echo ${file}
	#arhivam cu huffman
	(/usr/bin/time -f "(comprimarea huff ${file}) a durat %e secunde\n" ../comp -c ${file} ${arch_file_h} huff) 2> time.txt
	cat time.txt
	rm time.txt	
done



echo "Arhivam cu metoda Shannon-Fano"
for file in *
do
	
	
	arch_file_sf="../"${sfcomp}${file}${shannonfano_extension}
	echo ${file}
	#arhivam cu shannon fano
	(/usr/bin/time -f "(comprimarea sf ${file}) a durat %e secunde\n" ../comp -c ${file} ${arch_file_sf} sf) 2> time.txt
	cat time.txt
	rm time.txt
done


cd ../${hcomp}

echo "Dezarhivare fisiere comprimate cu metoda Huffman"

for file in *
do
	decomp_file=${file%.hff}
	echo ${file}
	(/usr/bin/time -f "(decomprimarea huff ${file}) a durat %e secunde\n" ../comp -x ${file} "../"${hdcomp}${decomp_file} huff) 2> time.txt
	cat time.txt
	rm time.txt
	
done

cd ../${sfcomp}


echo "Dezarhivare fisiere comprimate cu metoda Shannon-Fano"
for file in *
do
	decomp_file=${file%.sf}
	echo ${file}
	(/usr/bin/time -f "(decomp shannon fano ${file}) a durat %e secunde\n" ../comp -x ${file} "../"${sfdcomp}${decomp_file} sf) 2> time.txt
	cat time.txt 
	rm time.txt
done




