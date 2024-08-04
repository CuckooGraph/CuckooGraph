#include <iostream>
#include <fstream>

using namespace std;

void make_sparse_dataset() {
    ofstream fout;
    // double node = 5'000'000; // 5'000'000 for lcc
    double node = 10'000; // 5'000 for bc
    double num = 6;
    fout.open("Sparse-10k.txt", ios::out);
    for (int i = 1; i <= node; ++i) {
        for (int j = 1; j <= num; ++j) {
            int u = i + j;
            if (u > node)u = u - node;
            fout << u << ' ' << i << " 0" << endl;
        }
    }
    fout.close();
}

int main(void)
{
    make_sparse_dataset();
}