#pragma once

#ifndef GENERALGRAPH
#define GENERALGRAPH

#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <stack>
#include<map>
using namespace std;

using VertexType = int;
using WeightType = uint32_t;
using Neighbor = std::pair<VertexType, WeightType>;
using key_type = VertexType;
using count_type = WeightType;


class GeneralGraph {
public:
    static VertexType maxVertex;
    
    virtual void InsertEdge(VertexType src, VertexType dst, WeightType weight) = 0;
    virtual void InsertVertex(VertexType src) = 0;
    virtual std::vector<Neighbor> GetNeighbors(VertexType v) = 0;
    virtual WeightType GetWeightOfEdge(VertexType src, VertexType dst) = 0;
    virtual void ModifyEdge(VertexType src, VertexType dst, WeightType weight) = 0;
    virtual void DeleteVertex(VertexType v) = 0;
    virtual void DeleteEdge(VertexType src, VertexType dst) = 0;
    virtual ~GeneralGraph() = default;

    static void LoadData(std::string path);



// protected:
    // Used to deduplicate the vertex, for initialization. 
    // Some work would cast a excetpion if we insert a vertex that has already existed.
    static std::set<VertexType> vertex;

    // asrc, adst and cnt are borrowed from old testing Code, to load dataset.
    static std::vector<VertexType> asrc, adst;
    static int cnt;
    static bool hasLoadedData;
    

};

#endif