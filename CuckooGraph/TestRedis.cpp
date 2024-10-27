#include "redisDriver.h"
#include "cankao.hpp"
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <chrono>

// using VertexType = int;
// using key_type = int;
// std::set<VertexType> vertex;
// VertexType maxVertex;
// std::vector<VertexType> asrc, adst;
// int cnt;

// void LoadDataCaida(std::string path)
// {
//     using namespace std;
//     ifstream input("/share/datasets/CAIDA2018/dataset/130000.dat", ios::in | ios::binary);
//     char buf[200] = {0};
//     cnt = 0;
//     int MAXNUM = 5 * 1e7;
//     int length = MAXNUM;
//     for (cnt = 0; cnt < length; ++cnt)
//     {
//         if (!input.read(buf, 21))
//         {
//             break;
//         }
//         asrc.push_back(*(int *)buf);
//         adst.push_back(*(int *)(buf + 4));
//     }

//     unordered_map<key_type, key_type> mp;
//     int node_num = 0;
//     for (int it = 0; it < cnt; ++it)
//     {
//         if (!mp.count(asrc[it]))
//         {
//             ++node_num;
//             mp.insert({asrc[it], node_num});
//         }
//         if (!mp.count(adst[it]))
//         {
//             ++node_num;
//             mp.insert({adst[it], node_num});
//         }
//     }
//     maxVertex = 0;
//     cout << "caida node num:  " << node_num << endl;
//     for (int it = 0; it < cnt; ++it)
//     {
//         asrc[it] = mp[asrc[it]];
//         adst[it] = mp[adst[it]];
//         maxVertex = std::max(asrc[it], maxVertex);
//         maxVertex = std::max(adst[it], maxVertex);
//         vertex.insert(asrc[it]);
//         vertex.insert(adst[it]);
//     }

//     std::cout << "Data set loading over\n";
//     std::cout << "Vertex numbers = " << vertex.size() << std::endl;
//     std::cout << "load data cnt: " << cnt << endl;
//     std::cout << "maxVertex: " << maxVertex << endl;
// }

// void LoadDataStackOverflow(std::string path) {
//     using namespace std;
//     std::ifstream infile(path);
//     if (!infile.is_open()) {
//         std::cout << "Cannot open the file：" << path << std::endl;
//         exit(1);
//     }
//     std::cout << "Loading Dataset:" << path << std::endl;
//     maxVertex = 0;
//     int intbig = 2147483647;
//     int i = 0;

//     char useless[50];
//     infile.getline(useless, intbig);


//     while (1) {
//         char s[50];
//         if (!infile.getline(s, intbig)) break;
// #ifdef DEBUG
//         if (i > 100000) {
//             break;
//         }
// #endif
//         std::string src = "";
//         std::string dst = "";
//         int d = 0;

//         for (int k = 0; k < 50;) {
//             if (s[k] >= '0' && s[k] <= '9') {
//                 int r = k;
//                 while (s[r] >= '0' && s[r] <= '9') {
//                     std::string cc; cc = s[r];
//                     if (d == 0) { src.append(cc); ++r; }
//                     else if (d == 1) { dst.append(cc); ++r; }
//                 }
//                 k = r;
//                 if (d == 0)++d;
//                 else if (d == 1)k = 50;
//             }
//             else ++k;
//         }
//         asrc.push_back(stoi(src));
//         adst.push_back(stoi(dst));
//         maxVertex = std::max(asrc[i], maxVertex);
//         maxVertex = std::max(adst[i], maxVertex);
//         vertex.insert(asrc[i]);
//         vertex.insert(adst[i]);
//         ++i;
//     }
//     cnt = i;
//     std::cout << "load data cnt: " << cnt << endl;
//     infile.close();
//     std::cout << "Data set loading over\n";
//     std::cout << "Vertex numbers = " << vertex.size() << std::endl;
//     std::cout << "asrc[5]: " << asrc[5] << std::endl;std::cout << "adst[5]: " << adst[5] << std::endl;//////
// }

// void LoadDataCaida(std::string path)
// {
//     using namespace std;
//     ifstream input(path, ios::in | ios::binary);
//     if (!input.is_open())
//     {
//         std::cerr << "Could not open file: " << path << std::endl;
//         exit(1);
//     }
//     char buf[200] = { 0 };
//     cnt = 0;
//     int MAXNUM = 5 * 1e7;
//     int length = MAXNUM;
//     for (cnt = 0; cnt < length; ++cnt)
//     {
//         if (!input.read(buf, 21))
//         {
//             break;
//         }
//         asrc.push_back(*(int*)buf);
//         adst.push_back(*(int*)(buf + 4));
//     }

//     unordered_map<key_type, key_type> mp;
//     int node_num = 0;
//     for (int it = 0; it < cnt; ++it)
//     {
//         if (!mp.count(asrc[it]))
//         {
//             ++node_num;
//             mp.insert({ asrc[it], node_num });
//         }
//         if (!mp.count(adst[it]))
//         {
//             ++node_num;
//             mp.insert({ adst[it], node_num });
//         }
//     }
//     maxVertex = 0;
//     cout << "caida node num:  " << node_num << endl;
//     for (int it = 0; it < cnt; ++it)
//     {
//         asrc[it] = mp[asrc[it]];
//         adst[it] = mp[adst[it]];
//         maxVertex = std::max(asrc[it], maxVertex);
//         maxVertex = std::max(adst[it], maxVertex);
//         vertex.insert(asrc[it]);
//         vertex.insert(adst[it]);
//     }

//     std::cout << "Data set loading over\n";
//     std::cout << "Vertex numbers = " << vertex.size() << std::endl;
//     std::cout << "load data cnt: " << cnt << endl;
//     std::cout << "maxVertex: " << maxVertex << endl;
// }

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "usage: ./TestCaidaRedis /path/to/CAIDA" << std::endl;
        exit(1);
    }
    std::string path = argv[1];
    RedisDriver redis;
    LoadDataCaida(path);
    const int kBatch = 10'000;


    auto t1 = std::chrono::steady_clock::now();
    const int batchNum = cnt / kBatch;
    std::cout << std::endl;
    for (int i = 0; i < cnt; ++i) {
        bool isSuccess = redis.Insert(asrc[i], adst[i]);
        if (!isSuccess)
        {
            std::cerr << "Warning: Insertion failed " << cnt << std::endl;
        }
        if (i % kBatch == 0)
        {
            int curBatch = i / kBatch;
            printf("\rInsertion: %.2f%%", float(i) / cnt * 100);
        }
    }
   printf("\rInsertion: 100.00%%\n");
    
    auto t2 = std::chrono::steady_clock::now();
    auto t3 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "Insert throughput: " << cnt / (1.0 * t3) << " MIPs" << std::endl;



    t1 = std::chrono::steady_clock::now();
    for (int i = 0; i < cnt; ++i) {
        bool edgeExist;
        bool isSuccess = redis.Query(asrc[i], adst[i], edgeExist);
        if (!isSuccess)
        {
            std::cerr << "Warning: Query failed " << cnt << std::endl;
        }
        if (!edgeExist)
        {
            std::cerr << "Warning: edge did not exist " << cnt << std::endl;
        }
        if (i % kBatch == 0)
        {
            int curBatch = i / kBatch;
            printf("\rQuery: %.2f%%", float(i) / cnt * 100);
        }
    }
    printf("\rQuery: 100.00%%\n");
    t2 = std::chrono::steady_clock::now();
    t3 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "Query throughput: " << cnt / (1.0 * t3) << " MIPs" << std::endl;


    t1 = std::chrono::steady_clock::now();
    for (int i = 0; i < cnt; ++i) {
        bool isSuccess = redis.Del(asrc[i], adst[i]);
        if (!isSuccess)
        {
            std::cerr << "Warning: Deletion failed " << cnt << std::endl;
        }
                if (i % kBatch == 0)
        {
            int curBatch = i / kBatch;
            printf("\rDeletion: %.2f%%", float(i) / cnt * 100);
        }
    }
    printf("\rDeletion: 100.00%%\n");
    t2 = std::chrono::steady_clock::now();
    t3 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "Deletion throughput: " << cnt / (1.0 * t3) << " MIPs" << std::endl;

}