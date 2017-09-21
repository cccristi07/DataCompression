#ifndef __SHANNONFANO__H__
#define __SHANNONFANO__H__



#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>
#include <queue>

using namespace std;


struct charTuple
{
	unsigned char ch;
	unsigned int freq;
	charTuple()
	{
		ch = 0;
		freq = 0;
	}
	charTuple(unsigned char ch, unsigned int freq)
	{
		this->ch = ch;
		this->freq = freq;
	}

	bool operator <(charTuple* t)
	{
		return this->freq < t->freq;
	}

	bool operator >(charTuple* t)
	{
		return this->freq > t->freq;
	}

	bool operator <=(charTuple* t)
	{
		return this->freq <= t->freq;
	}

	bool operator ==(charTuple* t)
	{
		return this->freq == t->freq;
	}

	bool operator !=(charTuple* t)
	{
		return this->freq != t->freq;
	}

	friend ostream& operator <<(ostream& ost, charTuple * t)
	{
		ost << t->ch << " ---- " << t->freq << endl;
		return ost;
	}
};


struct Node
{
	vector<charTuple*> char_table;
	Node* left;
	Node* right;

	Node()
	{
		left = nullptr;
		right = nullptr;
	}

	explicit Node(vector<charTuple *> hist) {
		this->char_table = std::move(hist);
		left = nullptr;
		right = nullptr;
	}

	~Node()
	{
		char_table.clear();
		delete left;
		delete right;
	}

	bool isLeaf()
	{
		return !left && !right;
	}

	void showNode(Node *ptr)
	{
		cout << endl << endl;
		if (ptr)
		{
			for (auto &i : ptr->char_table) {
				cout << i;
			}
			showNode(ptr->left);
			showNode(ptr->right);
		}
	}
};


class SFTree
{
	Node* root;

	static void encode_char(
			Node *node,
			unsigned char c,
			string buff,
			string &retstr
	);

	static int decode_char(
			Node *node,
			unsigned char &c,
			string enc
	);

	static void write_bit(ofstream &out, unsigned char c);

	static unsigned char get_bits(ifstream &in);

	void buildtree(Node* subtree);
	SFTree();

	explicit SFTree(Node *root);

	explicit SFTree(vector<charTuple *> v);

	Node* getRoot();
	void buildtree();

	~SFTree();
public:
	static void SFcompresser(string infile, string outfile);
	static void SFdecompresser(string infile, string outfile);
};

#endif

