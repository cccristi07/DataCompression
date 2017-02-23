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
		HNode()
		{

			frecv = 0;
			chr = 0;
			left = nullptr;
			right = nullptr;
		}
		
		HNode(HNode* p)
		{
			this->left = p->left;
			this->right = p->right;
			this->frecv = p->frecv;
			this->chr = p->chr;
			
		}
		
		

		~HNode()
		{
			delete left;
			delete right;
		}
		
		bool operator==(const HNode *Node) const
		{
			return this->frecv == Node->frecv;
		}
		bool operator!=(const HNode *Node) const
		{
			return this->frecv != Node->frecv;
		}
		bool operator<(const HNode *Node) const
		{
			return this->frecv < Node->frecv;
		}
		bool operator>(const HNode *Node) const
		{
			return this->frecv > Node->frecv;
		}
		bool operator<=(const HNode *Node) const
		{
			return this->frecv <= Node->frecv;
		}
		bool operator>=(const HNode *Node) const
		{
			return this->frecv >= Node->frecv;
		}
		
		void print()
		{
			cout << (int) chr << "   " << frecv << endl;
			prints(left);
			prints(right);
			
		}
		
		void prints(HNode *node)
		{
			if(node)
			{
				cout << (int) node->chr << "    " << node->frecv<<endl;
				prints(node->left);
				prints(node->right);
			}
		}

	

		friend ostream& operator<<(ostream& ost, const HNode& nod)
		{
			if (&nod != NULL)
			{
				if (nod.chr < 128 && nod.chr > 31)
				ost << (unsigned char)nod.chr << " ---- " << nod.frecv << endl;

			else
				ost << (unsigned short)nod.chr << " ---- " << nod.frecv << endl;

			}
			return ost;
		}
	};


class HTree
{
	
	
	HNode *root;

	HTree();
	HTree(HNode *node);
	~HTree();

	// cautam caracterul c in arbore
	// daca  il gasim returnam true
	// daca nu il gasim returnam false
	static void char2EncodedStr(HNode* Node,
		unsigned char c,
		string rets,
		string &buff);

	static int str2DecodedChar(HNode* Node,
		string hstr,
		unsigned char &chr);


	static void write_huff_char(unsigned char bit, ofstream& ost);
	static unsigned char get_huff_bits(ifstream& hf);
	
	void printTree();

public:

	static void Hdecompresser(string infile, string outfile);
	static void Hcompresser(string infile, string outfile);

};


#endif
