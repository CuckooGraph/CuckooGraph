#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include "GeneralGraph.hpp"

std::set<VertexType> GeneralGraph::vertex;
std::vector<VertexType> GeneralGraph::asrc, GeneralGraph::adst;
int GeneralGraph::cnt = 0;
bool GeneralGraph::hasLoadedData = false;
VertexType GeneralGraph::maxVertex = 0;





void GeneralGraph::LoadData(std::string path) {
    if (hasLoadedData)  {
        return;
    }
    std::ifstream infile(path);
    if (!infile.is_open()) {
        std::cout << "Cannot open the file：" << path << std::endl;
        exit(1);
    } 
    std::cout << "Loading Dataset:" << path << std::endl;
    maxVertex = 0;
    int intbig = 2147483647;
    int i = 0;
    
    char useless[50];
    infile.getline(useless, intbig);
    
    
    while (1) {
        char s[50];
        if (!infile.getline(s, intbig)) break;
        #ifdef DEBUG
        if (i > 100000) {
            break;
        }
        #endif
        std::string src = "";
        std::string dst = "";
        int d = 0;

        for (int k = 0; k < 50;) {
            if (s[k] >= '0' && s[k] <= '9') {
                int r = k;
                while (s[r] >= '0' && s[r] <= '9') {
                    std::string cc; cc = s[r];
                    if (d == 0) { src.append(cc); ++r; }
                    else if (d == 1) { dst.append(cc); ++r; }
                }
                k = r;
                if (d == 0)++d;
                else if (d == 1)k = 50;
            }
            else ++k;
        }
       asrc.push_back(stoi(src));
       adst.push_back(stoi(dst));
        maxVertex = std::max(asrc[i], maxVertex);
        maxVertex = std::max(adst[i], maxVertex);
        vertex.insert(asrc[i]);
        vertex.insert(adst[i]);
        ++i;
    }
    cnt = i;
    std::cout <<"load data cnt: "<<cnt<<endl;
    infile.close();
    std::cout << "Data set loading over\n";
    std::cout << "Vertex numbers = " << vertex.size() << std::endl;
    hasLoadedData = true;
    std::cout<<"asrc[5]: "<<asrc[5]<<std::endl;std::cout<<"adst[5]: "<<adst[5]<<std::endl;//////
}