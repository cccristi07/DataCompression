#include "shannonfano.h"



SFTree::SFTree()
{
	root = new Node;
}

SFTree::SFTree(Node* root)
{

	this->root = root;
}

SFTree::SFTree(vector<charTuple*> v)
{
	root = new Node(v);
}

void SFTree::setLeft(Node *left)
{
	root->left = left;
}

void SFTree::setRight(Node* right)
{
	root->right = right;
}

Node * SFTree::getLeft()
{
	return root->left;
}

Node * SFTree::getRight()
{
	return root->right;
}

Node * SFTree::getRoot()
{
	return root;
}

void SFTree::buildtree()
{
	vector<charTuple*> chrs = root->char_table;
	

	if (chrs.size() == 1)
	{
		root->left = nullptr;
		root->right = nullptr;
	}
	else
	{
		int N = chrs.size();
		int indexl = 0;
		int indexr = N - 1;
		unsigned int suml = chrs.at(0)->freq;
		unsigned int sumr = chrs.at(N - 1)->freq;

		while (indexl + 1 < indexr)
		{
			if (suml >= sumr)
			{
				indexr--;
				sumr += chrs.at(indexr)->freq;
			}
			else
			{
				indexl++;
				suml += chrs.at(indexl)->freq;

			}
		}

			
			vector<charTuple*>::iterator fst, lst;

			fst = chrs.begin();
			lst = chrs.end();


			vector<charTuple*> left_table(fst, fst + indexl + 1);

			vector<charTuple*> right_table(fst + indexl + 1, lst);

			

			


			Node *l = new Node(left_table);
			Node *r = new Node(right_table);
			root->left = l;
			root->right = r;

			buildtree(root->left);
			buildtree(root->right);

		}
}

void SFTree::printtree()
{
	this->root->showNode();
}



void SFTree::char2EncodedStr(Node * node,
	unsigned char c, 
	string buff, 
	string& retstr)
{
	if (node)
	{
		if (node->isLeaf() && node->char_table[0]->ch == c) // daca s-a ajuns la frunze
		{
			// returnam stringul creat
			retstr = buff;
			return; 
		}
		else
		{
			char2EncodedStr(node->left, c, buff + '0', retstr);
			char2EncodedStr(node->right, c, buff + '1', retstr);
		}


	}
	else
	{
		return;
	}
}

int SFTree::str2DecodedChar(Node* node, 
	unsigned char& c, 
	string enc)
{
	Node * crawl = node;
	c = 0;

	if (crawl)
	{
		for (int i = 0; i < enc.size(); i++)
		{
			switch (enc[i])
			{

			case '0':
				crawl = crawl->left;
				break;

			case '1':
				crawl = crawl->right;
				break;

			default:
				break;
			}
		}

		if (crawl->isLeaf())
		{
			c = crawl->char_table[0]->ch;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}


}

void SFTree::write_bits_to_file(ofstream & out, unsigned char c)
{
	static int pos = 0; // pozitia la care scriem  [0 1 2 3 4 5 6 7]
	static unsigned char byte = 0; // bitul de scris

	switch (c)
	{
	case 0:
		// punem la pozitia pos in octetul byte, bitul 0
		byte = byte & static_cast<unsigned char>(~(1 << (7 - pos)));
		break;
	case 1:
		byte = byte | static_cast<unsigned char>(1 << (7 - pos));
		break;
	case 2:
		out.put(byte);
		return;

	}

	pos++;
	pos %= 8;

	if (pos == 0) // s-a terminat un octet, il scriem
	{
		out.put(byte);
		byte = 0;
	}


}

unsigned char SFTree::read_bits_from_file(ifstream & in)
{
	static int pos = 0; // returnam bitii de pe pozitia pos [0 1 2 3 4 5 6 7]
	static unsigned char byte = in.get();
	unsigned char bit; // bitul de returnat

	bit = (byte >> (7 - pos)) & 1;// obtinem bitul de la pozitia dorita

	pos++;
	pos %= 8;
	if (pos == 0)
	{
		if (in.eof())
		{
			bit = 2;
		}
		else
		{
			byte = in.get();
		}
	}

	return bit;
}

void SFTree::buildtree(Node * subtree)
{

	
	vector<charTuple*> chrs = subtree->char_table;
	
	if (chrs.size() == 1)
	{
		subtree->left = nullptr;
		subtree->right = nullptr;
	}
	else if (chrs.size() == 0)
	{
		return;
	}
	else
	{
		int N = chrs.size();
		int indexl = 0;
		int indexr = N - 1;
		unsigned int suml = chrs.at(0)->freq;
		unsigned int sumr = chrs.at(N - 1)->freq;

		while (indexl + 1 < indexr)
		{
			if (suml >= sumr)
			{
				indexr--;
				sumr += chrs.at(indexr)->freq;
			}
			else
			{
				indexl++;
				suml += chrs.at(indexl)->freq;
			}
		}

		

		vector<charTuple*>::iterator fst, lst;

		fst = chrs.begin();
		lst = chrs.end();


		vector<charTuple*> left_table(fst, fst + indexl + 1);
		vector<charTuple*> right_table(fst + indexl + 1, lst);

		

		Node *l = new Node(left_table);
		Node *r = new Node(right_table);

		subtree->left = l;
		subtree->right = r;

		buildtree(subtree->left);
		buildtree(subtree->right);

	}
}


SFTree::~SFTree()
{
	delete this->root;
}

void SFTree::SFcompresser(string infile, string outfile)
{
	// fisierul de intrare
	ifstream in(infile, ios::binary);
	ofstream out(outfile, ios::binary);
	if (!in.is_open())
	{
		cerr << "Eroare la deschiderea fisierului" << endl;
		exit(-2);
	}

	if (!out.is_open())
	{
		cerr << "Eroare la scrierea fisierului" << endl;
		exit(-3);
	}
	//histograma
	vector<unsigned int> freq(256, 0);

	// lambda pentru comparatie
	auto comp = [](charTuple* t1, charTuple* t2) -> bool
	{
		if (t1->freq == t2->freq)
		{
			return t1->ch < t1->ch;
		}

		return t1->freq > t2->freq;
	};

	

	// tot ce e in fisier punem intr-un buffer
	std::vector<char> buffer(
		(std::istreambuf_iterator<char>(in)),
		(std::istreambuf_iterator<char>()));
	
	in.clear();
	in.seekg(0);

	// pentru fiecare caracter incrementam histograma
	for (int i = 0; i < buffer.size(); i++)
	{
		freq.at(static_cast<unsigned char>(buffer.at(i)))++;
	}

	// scriem metainformatiile in fisier
	unsigned int N = 0;
	for (int i = 0; i < freq.size(); i++)
	{
		if (freq.at(i))
		{
			N++; // numar de simboluri folosite
		}
	}

	// scriem numarul de simboluri ca pe bucati de 8 biti

	out.put(static_cast<unsigned char>((N >> 24) % 256));
	out.put(static_cast<unsigned char>((N >> 16) % 256));
	out.put(static_cast<unsigned char>((N >> 8) % 256));
	out.put(static_cast<unsigned char>(N % 256));

	for (int i = 0; i < 256; i++)
	{
		// scriem in fisier caracterul si numarul lui de aparitii
		if (freq.at(i))
		{
			out.put(static_cast<unsigned char>(i));
			out.put(static_cast<unsigned char>((freq.at(i) >> 24) % 256));
			out.put(static_cast<unsigned char>((freq.at(i) >> 16) % 256));
			out.put(static_cast<unsigned char>((freq.at(i) >> 8) % 256));
			out.put(static_cast<unsigned char>(freq.at(i) % 256));
		}
	}

	// tabela cu caracterele soratate descrescator
	std::vector<charTuple*> tabela;

	for (int i = 0; i < 256; i++)
	{
		if (freq.at(i) != 0)
		{
			// daca caracterul apare in fisier il punem in tabela
			charTuple* tmp = new charTuple(
				static_cast<unsigned char>(i),
				freq.at(i));

			tabela.push_back(tmp);
		}
	}

	

	std::sort(tabela.begin(), tabela.end(),comp);

	SFTree *tree = new SFTree(tabela);
	tree->buildtree();

	string enc_str[256];

	for (int i = 0; i < 256; i++)
	{
		char2EncodedStr(tree->getRoot(),
			static_cast<unsigned char>(i),
			"",
			enc_str[i]);
	}
	

	unsigned char decoded;
	for (int i = 0; i < buffer.size(); i++)
	{
		// pentru fiecare caracter, care se afla in buffer
		// aflam decodificarea si o punem in fisier
		unsigned char pos = static_cast<unsigned char>(buffer.at(i));
		for (int j = 0; j < enc_str[pos].size(); j++)
		{
			// scriem fiecare bit din enc_str[buffer[i]]
			unsigned char ch_write = enc_str[pos].at(j) - '0';
			if (ch_write == 0)
			{	
				write_bits_to_file(out, 0);
			}
			if (ch_write == 1)
			{
				write_bits_to_file(out, 1);
			}
			
		}
	}

	// scriem eof
	write_bits_to_file(out, 2);
	delete tree;
	in.close();
	out.close();
	cout << "Done :)" << endl;

}


void SFTree::SFdecompresser(string infile, string outfile)
{
	ifstream in(infile, ios::binary);
	ofstream out(outfile, ios::binary);

	if (!in.is_open())
	{
		cerr << "Nu s-a putut deschide fisierul " << endl;
		exit(-2);
	}

	if (!out.is_open())
	{
		cerr << "Nu s-a putut deschide fisierul" << endl;
		exit(-3);
	}

	auto comp = [](charTuple* t1, charTuple* t2) -> bool
	{
		if (t1->freq == t2->freq)
		{
			return t1->ch < t1->ch;
		}

		return t1->freq > t2->freq;
	};


	unsigned int N = 0; // nr caractere cu aparitii nenule
	char car; // caracter de citit
			  //unsigned char ucar;
	unsigned int f; // frecventa de citit
					// fisierul nostru are un tip de header de tipul
					//		N - numarul de simboluri care apar in fisierul de decomprimat	
					//		N linii de tipul:
					//      (unsigned char)caracter -- (unsigned int)nr aparitii
					// citind aceste informatii vom putea reface arborele huffman si putem decodifica
	
	char buff[4] = { 0 };
	in.read(buff, 4);
	
	for (int i = 0; i < 4; i++)
	{
		N += (unsigned char)(buff[i])*((unsigned char)1 << (24 - 8 * i));
	}

	vector<charTuple*> read_table;
	unsigned char ch_write = 0;

	for (int i = 0; i < N; i++)
	{
		in.read(&car, 1);

		in.read(buff, 4);
		f = 0;
		ch_write = car;
		unsigned char unu = 1;
		for (int j = 0; j < 4; j++)
		{
			f += (unsigned char)(buff[j])*((unsigned char)unu << (24 - 8 * j));
			
		}

		charTuple* p = new charTuple(ch_write, f);
		read_table.push_back(p);
	}

	// avem histograma aparitiilor caracterelor in read_table
	std::sort(read_table.begin(), read_table.end(), comp);// o sortam dupa nr de aparitii
	unsigned long long nr_cars = 0;

	for (long long i = 0; i < read_table.size(); i++)
	{
		nr_cars += read_table.at(i)->freq;
	}

	// construim arborele shannon fano
	SFTree *tree = new SFTree(read_table);
	tree->buildtree();
	unsigned char c_build;

	int ok;

	for(long long i = 0; i < nr_cars; i++)
	{
		
		string str = "";
		do
		{
			c_build = read_bits_from_file(in);

			if (c_build == 0)
				str += '0';

			if (c_build == 1)
				str += '1';

			if (c_build == 2)
			{
				ch_write = 0;
				break;
			}
			ok = str2DecodedChar(tree->getRoot(), ch_write, str);

		} while (!ok);

		out.put(static_cast<char>(ch_write));
	}


	delete tree;
	read_table.clear();
	in.close();
	out.close();
	cout << "Done :)" << endl;

}
