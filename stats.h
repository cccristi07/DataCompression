#ifndef __STATS__H__
#define __STATS__H__


#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <cstdlib>

using namespace std;


enum sizes {
    B = 0,
    KB = 1,
    MB = 2,
    GB = 3
};

void print_stats(string compressed_file, string original_file);

#endif
