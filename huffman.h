#ifndef __HUFFMAN__H__
#define __HUFFMAN__H__


#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <algorithm>
#include <queue>

using namespace std;


class HTree {

    struct HNode {
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

    HNode *root;


    explicit HTree(HNode *node);


    void encode_char(
            HNode *Node,
            unsigned char c,
            string rets,
            string &buff
    );

    int decode_char(
            HNode *Node,
            string hstr,
            unsigned char &chr
    );


    void write_char(unsigned char bit, ofstream &ost);

    unsigned char get_bits(ifstream &hf);

public:

    HTree();

    ~HTree();

    void Hdecompresser(string infile, string outfile);

    void Hcompresser(string infile, string outfile);

};


#endif
