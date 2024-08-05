#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

using VertexType = int;
using WeightType = uint32_t;
using Neighbor = std::pair<VertexType, WeightType>;
using key_type = VertexType;
using count_type = WeightType;

void LoadDataSparseDense(std::string path);
void LoadDataCaida(std::string path);
void LoadDataNotredame(std::string path);
void LoadDataWiki(std::string path);
void LoadDataStackOverflow(std::string path);
void LoadDataSina(std::string path);

#ifndef CANKAO
extern std::set<VertexType> vertex;
extern VertexType maxVertex;

extern std::vector<VertexType> asrc, adst;
extern int cnt;
#endif