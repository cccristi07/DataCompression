//
// Created by spark on 22.09.2017.
//

#ifndef DATACOMPRESSION_HUFFMANCOMP_HPP
#define DATACOMPRESSION_HUFFMANCOMP_HPP

#include "Node.hpp"
#include <tuple>
#include <memory>
#include <fstream>
#include <iostream>
#include <queue>

using namespace std;

typedef tuple<unsigned long long, unsigned char> HuffData;
typedef Node<HuffData> _HNode;

class HuffmanComp {

    _HNode *root;

    explicit HuffmanComp(_HNode *node);

    void encode_char(
            _HNode *Node,
            unsigned char c,
            string rets,
            string &buff
    );

    int decode_char(
            _HNode *Node,
            string hstr,
            unsigned char &chr
    );

    static unsigned long long get_freq(_HNode *Node){
        return get<0>(Node->get_data());
    }

    static unsigned char get_char(_HNode *Node){
        return get<1>(Node->get_data());
    }


    void write_char(unsigned char bit, ofstream &ost);

    unsigned char get_bits(ifstream &hf);


public:

    HuffmanComp();

    ~HuffmanComp();

    void decompresser(string infile, string outfile);

    void compresser(string infile, string outfile);


};


#endif //DATACOMPRESSION_HUFFMANCOMP_HPP
