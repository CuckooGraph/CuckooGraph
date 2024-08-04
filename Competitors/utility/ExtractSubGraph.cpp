#include "cankao.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_map>


using namespace std;

unordered_map<string, string> defaultPath = {
    {"caida", "/share/datasets/CAIDA2018/dataset/130000.dat"},
};

unordered_map<string, std::function<void(string)>> LoadFunction = {
    {"caida", LoadDataCaida},
    {"stackoverflow", LoadDataStackOverflow},
    {"notredame", LoadDataNotredame},
    {"wiki", LoadDataWiki},
    {"sparse", LoadDataSparseDense},
    {"dense", LoadDataSparseDense},
    {"sina", LoadDataSina},
};

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 5) {
        cout << "Usage: ./extract dataset_name destination_dir num_of_vertex_in_subgraph [dataset_source_dir]" << endl;
        cout << "Supported dataset: caida、stackoverflow、notredame、wiki、sparse、dense、sina\n Supported default path: caida" << endl; 
        exit(1);
    }
    string oldDatasetPath;
    string datasetName = argv[1];
    string newdatasetPath = argv[2];
    int numOfVertexes = stoi(argv[3]);

    if (argc == 5)
        oldDatasetPath = argv[4];
    else 
    {
        if (defaultPath[datasetName] == "") {
            if (defaultPath[datasetName] != "") {
                cout << "Did not have default path for " << datasetName << endl;
            } else {
                cout << "Did not support this dataset: " << datasetName << endl;
            }
            exit(1);
        } else {
            oldDatasetPath = defaultPath[datasetName];
        }
    }

    if (filesystem::exists(newdatasetPath)) {
        cerr << "File exisits! Please delete the file first!" << endl;
        exit(1);
    }
    ofstream of(newdatasetPath, std::ios::out | std::ios::trunc);
    if (!of.is_open()) {
        cerr << "Cannot create " << newdatasetPath;
        exit(1);
    }

    auto func = LoadFunction[datasetName];
    if (func != nullptr) {
        func(oldDatasetPath);
    } else {
        cout << "Did not support this dataset: " << datasetName << endl;
        exit(1);
    }

    vector<pair<int, int>> degree(maxVertex + 1);
    int start = 0;
    auto generator = [&start](){
        return make_pair(0, start++);
    };

    generate(degree.begin(), degree.end(), generator);


    for (int i = 0; i < cnt; i++)
    {
        degree[asrc[i]].first++;
        degree[adst[i]].first++;
    }

    sort(degree.begin(), degree.end(), greater<pair<int, int>>());
    
    for (auto iter = degree.begin(); iter != degree.end(); iter++)
    {
        if (iter->first == 0)
        {
            degree.erase(iter, degree.end());
            break;
        }
    }
    numOfVertexes = min(numOfVertexes, (int)degree.size());
    cout << "Actually generate " << numOfVertexes << " Vertexes!" << endl;
    

    set<int> chosenVertexes;



    for (int i = 0; i < numOfVertexes; i++) {
        chosenVertexes.insert(degree[i].second);
    }
    cout << "Vertex choosing over" << endl;

    unordered_map<int, int> real2Logical;

    int logicalId = 1;
    int temp = 0;

    for (int i = 0; i < cnt; i++) {
        if (chosenVertexes.count(asrc[i]) && chosenVertexes.count(adst[i])) {
            int asrcLogical, adstLogical;
            if (real2Logical[asrc[i]] == 0) {
                real2Logical[asrc[i]] = logicalId++;
            }

            if (real2Logical[adst[i]] == 0) {
                real2Logical[adst[i]] = logicalId++;
            }

            of  << real2Logical[asrc[i]] << " " << real2Logical[adst[i]] << " 0" << endl;

            temp++;
        }
            
    }
    of.close();

    cout << "Over" << endl;
    cout << "Generated " << temp << " edges. Cnt = " << cnt << endl;
    return 0;
}

