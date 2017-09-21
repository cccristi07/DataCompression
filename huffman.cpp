#include "huffman.h"




HTree::~HTree()
{
	delete this->root;
}

HTree::HTree()
{
	root = new HNode();
}


HTree::HTree(HNode* node)
{
	root = node;
}


void HTree::char2EncodedStr(
        HNode *Node,
        unsigned char c,
        string rets,
        string &buff
)
{

	if (Node)
		{
			if (!Node->left && !Node->right && c == Node->chr)
			{
				// am gasit caracterul
				buff = rets;
				return;
			}
			else
			{
				// nu am ajuns la frunze, cautam in continuare
				char2EncodedStr(Node->left, c, rets + '0', buff);
				char2EncodedStr(Node->right, c, rets + '1', buff);

			}
		}


}


int HTree::decode_char(HNode *Node,
                       string hstr,
                       unsigned char &chr)
{

    for (char i : hstr) {

        if (i == '0')
				Node = Node->left;

        if (i == '1')
				Node = Node->right;

        if (Node == nullptr)
            return 0;

    }

    if (!Node->left && !Node->right) {
        chr = Node->chr;
        return 1;
    }

    chr = 0;
    return 0;

}

void HTree::write_char(unsigned char bit, ofstream &ost) {
    static int pos = 0; // pozitia la care scriem intr-un octet
	static unsigned char oct = '\0'; // octetul care vrem sa il scriem

    if (bit < 2) {

        if (bit == 1)
            oct = oct | (bit << (7 - pos));
        else
            oct = oct & static_cast<unsigned char>(255 - (1 << (7 - pos)));

        pos++;
        pos = pos % 8;
        if (pos == 0) {

            ost.put(oct);
            oct = '\0';
        }
    } else
        ost.put(oct);

}

unsigned char HTree::get_bits(ifstream &hf)
{
	static int pos = 0;
    static auto ch = static_cast<unsigned char>(hf.get());

	unsigned char bit;

	// din octetul citit vrem sa obtinem bitul de pe pozitia pos
    bit = static_cast<unsigned char>((ch >> (7 - pos)) % 2);

	pos++;
	pos %= 8;

	if (pos == 0)
	{
		//am citit toti bitii, trecem la urmatorul octet
		if (!hf.eof())
            ch = static_cast<unsigned char>(hf.get());
		else
			bit = 2; // daca am ajuns la sfarsit returnam EOF
	}


	return bit;

}


void HTree::Hdecompresser(string infile, string outfile)
{

	ifstream f_in(infile, ios::binary);
    if (!f_in.is_open()) {

		cerr << "Nu s-a putut deschide fisierul " << endl;
		exit(-1);
	}

	ofstream f_out(outfile, ios::binary);

    if (!f_out.is_open()) {

		cerr << "Eroare la deschiderea fisierului" << endl;
		exit(-2);
	}

	// lambda pentru comparatie
	auto comp = [](HNode* n1, HNode* n2)
	{
		return n1->frecv > n2->frecv;
	};

	// heap pentru a tine nodurile huffman 
	priority_queue < HNode*, vector<HNode*>, decltype(comp)> q(comp);

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
	f_in.read(buff, 4);
	for (int i = 0; i < 4; i++)
		N += (unsigned char)(buff[i])*((unsigned char)1 << (24 - 8 * i));

	
	vector<int> freq(256, 0);

	unsigned char ch_write = 0;
    for (int i = 0; i < N; i++) {
	
		f_in.read(&car, 1);
		f_in.read(buff, 4);
		f = 0;

        ch_write = static_cast<unsigned char>(car);
		unsigned char unu = 1;

        for (int j = 0; j < 4; j++) {

            f += (unsigned char) (buff[j]) * (unu << (24 - 8 * j));

		}

		freq[ch_write] = f;

	}

	// avem histograma aparitiei simbolurilor, acum reconstruim arborele

    for (int i = 0; i < 256; i++) {

        if (freq.at(static_cast<unsigned long>(i))) {

			//cout << freq.at(i) << endl;
			// creem nodul si il introducem in heap
            auto *nod = new HNode;
            nod->frecv = static_cast<unsigned int>(freq.at(i));
			nod->chr = (unsigned char)i;
			q.push(nod);
		}
	}


    auto heap_size = static_cast<int>(q.size());
    for (int i = 0; i < heap_size - 1; i++) {

        auto *z = new HNode;
		z->left = q.top();
		q.pop();
		z->right = q.top();
		q.pop();
		z->frecv = z->left->frecv + z->right->frecv;
		q.push(z);
	}

	HNode *root = q.top();

	// in acest punct avem arborele huffman reconstruit
	
	// citim in continuare din fisier octeti si ii decodam
    string huff_str;
	car = 0;
	unsigned char car_w = 0;
	int ok = 0;
	int nr_car = root->frecv;

    for (int i = 0; i < nr_car; i++) {

		huff_str = "";

        do {

            car = get_bits(f_in);

			if (car == 0)
				huff_str += '0';
			if (car == 1)
				huff_str += '1';

            ok = decode_char(root, huff_str, car_w);

		} while (!ok);


		f_out.put(static_cast<char>(car_w));
	}

	f_in.close();
	f_out.close();
	delete root;
	cout << "Done :)" << endl;

}


void HTree::Hcompresser(string infile, string outfile)
{
	ifstream f_in(infile, ios::binary);

	unsigned int non_zero_freq_elem = 0; // nr simboluri cu nr nenul de aparitii

    if (!f_in.is_open()) {

		cerr << "Eroare la deschiderea fisierului" << endl;
		exit(-2);
	}

	ofstream f_out(outfile, ios::binary);

    if (!f_out.is_open()) {

		cerr << "Eroare la deschiderea fisierului de iesire" << endl;
		exit(-3);
	}


    // lambda pentru comparatie
	auto comp = [](HNode* n1, HNode* n2)
	{
		return n1->frecv > n2->frecv;
	};


	// heap pentru a tine nodurile huffman 
	priority_queue < HNode*, vector<HNode*>, decltype(comp)> q(comp);

	std::vector<unsigned int> freq(256, 0);


    while (!f_in.eof()) {

        unsigned char read_chr;
        f_in >> read_chr;
        freq.at(read_chr)++;
    }

    f_in.clear();
    f_in.seekg(0);



	// creem nodurile huffman
    for (int i = 0; i < 256; i++) {

		// daca caracterul i a aparut
        if (freq.at(static_cast<unsigned long>(i)) != 0) {

			// alocam nodul
            auto *nod = new HNode;
			nod->chr = (unsigned char)i;
			nod->frecv = freq.at(i);
			
			
			// il introducem in coada
			q.push(nod);
		}
	}


    auto n = static_cast<int>(q.size()); // dimensiunea cozii
		
		
	// algoritmul pentru a face arborele huffman
    for (int i = 0; i < n - 1; i++) {

        auto *z = new HNode;
		z->left = q.top();
		q.pop();
		z->right = q.top();
		q.pop();
		z->frecv = z->left->frecv + z->right->frecv;
		q.push(z);
		//log << *z;
	}
	// extragem radacina arborelui huffman
	HNode *root = q.top();

	string huff_str[256];

    for (int i = 0; i < 256; i++) {

        char2EncodedStr(root, (unsigned char) i, "", huff_str[i]);

		int ok = huff_str[i].empty();

		if (!ok)
			non_zero_freq_elem++;
	}
	

	
	// scriem numarul de simboluri folosite in grupuri de 1 octet
	f_out.put(static_cast<unsigned char>(((non_zero_freq_elem >> 24) % 256)));
	f_out.put(static_cast<unsigned char>(((non_zero_freq_elem >> 16) % 256)));
	f_out.put(static_cast<unsigned char>(((non_zero_freq_elem >> 8) % 256)));
	f_out.put(static_cast<unsigned char>((non_zero_freq_elem % 256)));

    for (int i = 0; i < 256; i++) {

        // o sa punem caracterul si nr de aparitii
        if (!huff_str[i].empty()) {

            f_out.put(static_cast<unsigned char>(i));
            f_out.put(static_cast<unsigned char>((freq[i] >> 24) % 256));
            f_out.put(static_cast<unsigned char>((freq[i] >> 16) % 256));
            f_out.put(static_cast<unsigned char>((freq[i] >> 8) % 256));
            f_out.put(static_cast<unsigned char>(freq[i] % 256));

        }
    }
	
	f_in.clear();
	f_in.seekg(0);

	char car;
	unsigned char ch;
	unsigned char _ch;


    while (f_in.get(car)) {

        ch = static_cast<unsigned char>(car);

        for (char i : huff_str[ch]) {
            if (i == '0')
				_ch = 0;
            if (i == '1')
				_ch = 1;

            write_char(_ch, f_out);
		}
	}

	_ch = 2;
    write_char(_ch, f_out);

	f_in.close();
	f_out.close();

	delete root;
	cout << "DONE :)" << endl;

}

