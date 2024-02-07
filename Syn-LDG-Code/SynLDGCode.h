#include<math.h>
#include<string>
#include<iostream>
#include<memory.h>
#include<queue>
#include <fstream>
using namespace std;

void make_dataset_new() {
	ofstream fout;
	double node = 8000;
	double rrr = 0.9;
	double num = rrr * node;
	int r = 10;
	fout.open("CuckooGraphDatasetNewNew.txt", ios::out);
	for (int i = 1; i <= node; ++i) {
		for (int j = 1; j <= node; ++j) {
			if (j != i && ((rand() % (10))))
				fout << j << ',' << i << endl;
		}
	}
	fout.close();
}
