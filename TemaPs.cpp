#include <iostream>
#include "huffman.h"
#include "shannonfano.h"
#include "stats.h"


using namespace std;

void print_help(const string &exec) {

    cerr << "Usage:" << endl <<
         exec << " arg1 inputfile outputfile method" << endl <<
         "arg1 :" << endl <<
         "    -x = extract" << endl <<
         "    -c = compress" << endl <<
         "method:" << endl <<
         "    huff = Huffman compresion" << endl <<
         "    sf   = Shannon Fano compression" << endl <<
         "    (default = Huffman compression)" << endl << endl <<

         exec << " -help  : prints help" << endl;

}

int main(int argc, char **argv) {


    HTree huff;
    SFTree sf;


    if (argc != 5 && argc != 4 || (string(argv[1]) == "-help")) {

        print_help(argv[0]);
        exit(-4);
    } else if (argc == 5) {
        if (string(argv[4]) == "huff") {

            if (string(argv[1]) == "-c") {
                cout << "Compressing...(Huffman)" << endl;
                huff.Hcompresser(argv[2], argv[3]);
                print_stats(string(argv[3]), string(argv[2]));
            } else if (string(argv[1]) == "-x") {
                cout << "Extracting...(Huffman)" << endl;
                huff.Hdecompresser(argv[2], argv[3]);
            } else {
                cout << "Incorrect arguments...exiting" << endl;
                exit(-4);
            }
        } else if (string(argv[4]) == "sf") {
            if (string(argv[1]) == "-c") {
                cout << "Compressing...(Shannon Fano)" << endl;
                sf.SFcompresser(argv[2], argv[3]);
                print_stats(string(argv[3]), string(argv[2]));
            } else if (string(argv[1]) == "-x") {
                cout << "Extracting...(Shannon Fano)" << endl;
                sf.SFdecompresser(argv[2], argv[3]);
            } else {
                cout << "Incorrect arguments...exiting" << endl;
                exit(-4);
            }

        } else {
            print_help(argv[0]);
            exit(-4);
        }

    } else if (argc == 4) {
        if (string(argv[1]) == "-c") {
            cout << "Compressing...(Huffman)" << endl;
            huff.Hcompresser(argv[2], argv[3]);
            print_stats(string(argv[3]), string(argv[2]));
        } else if (string(argv[1]) == "-x") {
            cout << "Extracting...(Huffman)" << endl;
            huff.Hdecompresser(argv[2], argv[3]);
        } else {
            cout << "Incorrect arguments...exiting" << endl;
            exit(-4);
        }
    }

    return 0;
}

