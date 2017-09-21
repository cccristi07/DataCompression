#ifndef __HUFFMAN__H__
#define __HUFFMAN__H__


#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <algorithm>
#include <queue>

using namespace std;

struct HNode
{
    HNode *left;
    HNode *right;
    unsigned int frecv;
    unsigned char chr;

    HNode() {
        frecv = 0;
        chr = 0;
        left = nullptr;
        right = nullptr;
    }

    explicit HNode(HNode *p) {
        this->left = p->left;
        this->right = p->right;
        this->frecv = p->frecv;
        this->chr = p->chr;
    }

    ~HNode() {
        delete left;
        delete right;
    }

    bool operator==(const HNode *Node) const {
        return this->frecv == Node->frecv;
    }

    bool operator!=(const HNode *Node) const {
        return this->frecv != Node->frecv;
    }

    bool operator<(const HNode *Node) const {
        return this->frecv < Node->frecv;
    }

    bool operator>(const HNode *Node) const {
        return this->frecv > Node->frecv;
    }

    bool operator<=(const HNode *Node) const {
        return this->frecv <= Node->frecv;
    }

    bool operator>=(const HNode *Node) const {
        return this->frecv >= Node->frecv;
    }

    void prints(HNode *node) {
        if (node) {
            cout << (int) node->chr << "    " << node->frecv << endl;
            prints(node->left);
            prints(node->right);
        }
    }


    friend ostream &operator<<(ostream &ost, const HNode &nod) {
        if (&nod != nullptr)
            ost << (nod.chr < 128 && nod.chr > 31 ? nod.chr : (unsigned short) nod.chr) << " ---- " << nod.frecv
                << endl;
        return ost;
    }
};


class HTree
{

    HNode *root;

    HTree();

    explicit HTree(HNode *node);

    ~HTree();

    static void encode_char(
            HNode *Node,
            unsigned char c,
            string rets,
            string &buff
    );

    static int decode_char(
            HNode *Node,
            string hstr,
            unsigned char &chr
    );


    static void write_char(unsigned char bit, ofstream &ost);

    static unsigned char get_bits(ifstream &hf);

public:

    static void Hdecompresser(string infile, string outfile);

    static void Hcompresser(string infile, string outfile);

};


#endif
