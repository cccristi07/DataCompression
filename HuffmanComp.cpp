//
// Created by spark on 22.09.2017.
//

#include "HuffmanComp.hpp"

HuffmanComp::HuffmanComp(_HNode *node) {
    root = node;
}

HuffmanComp::HuffmanComp() {
    root = new _HNode();
}

HuffmanComp::~HuffmanComp() {
    delete root;
}

void HuffmanComp::encode_char(_HNode *Node, unsigned char c, string rets, string &buff) {

    if (Node) {
        if (!Node->get_left() && !Node->get_right() && c == get_char(Node)) {
            // am gasit caracterul
            buff = rets;
            return;
        } else {
            // nu am ajuns la frunze, cautam in continuare
            encode_char(Node->get_left(), c, rets + '0', buff);
            encode_char(Node->get_right(), c, rets + '1', buff);

        }
    }


}

int HuffmanComp::decode_char(_HNode *Node, string hstr, unsigned char &chr) {

    for (char i : hstr) {

        if (i == '0')
            Node = Node->get_left();

        if (i == '1')
            Node = Node->get_right();

        if (Node == nullptr)
            return 0;

    }

    if (!Node->get_left() && !Node->get_right()) {
        chr = get_char(Node);
        return 1;
    }

    chr = 0;
    return 0;
}

void HuffmanComp::write_char(unsigned char bit, ofstream &ost) {

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

unsigned char HuffmanComp::get_bits(ifstream &hf) {

    static int pos = 0;
    static auto ch = static_cast<unsigned char>(hf.get());

    unsigned char bit;

    // din octetul citit vrem sa obtinem bitul de pe pozitia pos
    bit = static_cast<unsigned char>((ch >> (7 - pos)) % 2);

    pos++;
    pos %= 8;

    if (pos == 0) {
        //am citit toti bitii, trecem la urmatorul octet
        if (!hf.eof())
            ch = static_cast<unsigned char>(hf.get());
        else
            bit = 2; // daca am ajuns la sfarsit returnam EOF
    }

    return bit;
}



void HuffmanComp::decompresser(string infile, string outfile) {

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
    auto comp = [](_HNode *n1, _HNode *n2) {
        return get_freq(n1) > get_freq(n2);
    };

    // heap pentru a tine nodurile huffman
    priority_queue<_HNode*, vector<_HNode*>, decltype(comp)> q(comp);



    unsigned int N = 0; // nr caractere cu aparitii nenule
    char car; // caracter de citit
    //unsigned char ucar;
    unsigned int f; // frecventa de citit
    // fisierul nostru are un tip de header de tipul
    //		N - numarul de simboluri care apar in fisierul de decomprimat
    //		N linii de tipul:
    //      (unsigned char)caracter -- (unsigned int)nr aparitii
    // citind aceste informatii vom putea reface arborele huffman si putem decodifica

    char buff[4] = {0};
    f_in.read(buff, 4);
    for (int i = 0; i < 4; i++)
        N += (unsigned char) (buff[i]) * ((unsigned char) 1 << (24 - 8 * i));


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

            int frecv = static_cast<unsigned int>(freq.at(i));
            char chr = (unsigned char) i;
            auto nod = unique_ptr<_HNode>(new _HNode(nullptr, nullptr, HuffData(frecv, chr)));

            q.push(nod);
        }
    }


    auto heap_size = static_cast<int>(q.size());
    for (int i = 0; i < heap_size - 1; i++) {
        // TODO: use scoped pointers to build the tree
        auto z = make_unique<_HNode>();
        z->set_left(q.top());
        q.pop();
        z->set_right(q.top());
        q.pop();
        z->set_data(HuffData(get_freq(z->get_left()) + get_freq(z->get_right()), '\0'));

        q.push(z);
    }


    auto root = make_unique(q.top());

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

void HuffmanComp::compresser(string infile, string outfile) {

}
