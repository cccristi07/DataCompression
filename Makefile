CC = g++
FLG = -std=c++11 -O3 
.PHONY: build all clean cleandir shannonfano huffman stats


all: build

build: shannonfano stats huffman
	$(CC) $(FLG) $(OPT)  huffman.o shannonfano.o stats.o TemaPs.cpp -o comp

shannonfano: shannonfano.cpp
	$(CC) $(FLG) $(OPT)  -c $^


huffman: huffman.cpp
	$(CC) $(FLG) $(OPT)  -c huffman.cpp


stats: stats.cpp
	$(CC) $(FLG) $(OPT)  -c stats.cpp
	

clean:
	rm *.o *.hff *.sf

cleandir:
	rm -rf huffman_comp huffman_decomp shannonfano_comp shannonfano_decomp
