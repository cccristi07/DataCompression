#ifndef __SHANNONFANO__H__
#define __SHANNONFANO__H__



#include <iostream>
#include <fstream>
#include <string>
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

	Node(vector<charTuple*> hist)
	{
		this->char_table = hist;
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

	void showNode()
	{
	
		for (int i = 0; i < char_table.size(); i++)
		{
			cout << char_table.at(i);
		}
		showNode(left);
		showNode(right);
	}
	void showNode(Node *ptr)
	{
		cout << endl << endl;
		if (ptr)
		{
			for (int i = 0; i < ptr->char_table.size(); i++)
			{
				cout << ptr->char_table.at(i);
			}
			showNode(ptr->left);
			showNode(ptr->right);
		}
	}
	

	

};


class SFTree
{
	Node* root;

	static void char2EncodedStr(Node* node,
		unsigned char c,
		string buff,
		string &retstr);
	
	static int str2DecodedChar(Node* node,
		unsigned char &c,
		string enc);

	static void write_bits_to_file(ofstream& out, unsigned char c);
	static unsigned char read_bits_from_file(ifstream& in);

	void buildtree(Node* subtree);
	SFTree();
	SFTree(Node* root);
	SFTree(vector<charTuple*> v);
	void setLeft(Node*);
	void setRight(Node*);
	Node* getLeft();
	Node* getRight();
	Node* getRoot();
	void buildtree();
	void printtree();
	~SFTree();
public:
	static void SFcompresser(string infile, string outfile);
	static void SFdecompresser(string infile, string outfile);
};

#endif

