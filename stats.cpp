#include "stats.h"



void print_stats(string compressed_file, string original_file)
{

	FILE *cmp;
	FILE *orig;
	cmp  = fopen(compressed_file.c_str(), "rb");
	orig = fopen(original_file.c_str(), "rb");
	if (orig == nullptr || cmp == nullptr)
	{
		cerr << "Eroare la deschidere" << endl;
		exit(-1);
	}


	fseek(cmp, 0, SEEK_END);
	fseek(orig, 0, SEEK_END);

	auto size_comp = static_cast<unsigned long long int>(ftell(cmp));
	auto size_orig = static_cast<unsigned long long int>(ftell(orig));

	auto comp_rate = size_orig != 0 ? (1.0 - (double) size_comp / (double) size_orig) : -1e20;


	double show_comp = size_comp;
	double show_orig = size_orig;
	short cont_comp = 0; // pentru afisare
	short cont_orig = 0;

	while (show_comp > 1024)
	{
		cont_comp++;
		show_comp /= 1024.0;
	}

	while (show_orig > 1024)
	{
		cont_orig++;
		show_orig /= 1024.0;
	}
	string orig_mag;
	string comp_mag; // pot fi B, KB, MB, GB
    
        
    
	switch (cont_comp)
	{
	case B:
		comp_mag = "B";
		break;

	case KB:
		comp_mag = "KB";
		break;

	case MB:
		comp_mag = "MB";
		break;

	case GB:
		comp_mag = "GB";
		break;

	default:
		break;
	}

	switch (cont_orig)
	{
	case B:
		orig_mag = "B";
		break;

	case KB:
		orig_mag = "KB";
		break;

	case MB:
		orig_mag = "MB";
		break;

	case GB:
		orig_mag = "GB";
		break;

	default:
		break;
	}

	cout << endl;
	cout << "compressed size :" << show_comp << comp_mag << endl <<
		"orig size " << show_orig << orig_mag << endl;


	cout << "Compression rate is " << comp_rate * 100 << " %" << endl;
	fclose(cmp);
	fclose(orig);
}
