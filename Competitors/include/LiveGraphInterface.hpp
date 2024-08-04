#pragma once
#ifndef LIVEGRAPHINTERFACE
#define LIVEGRAPHINTERFACE

#include <iostream>
#include <string>
#include <unordered_map>
#include "GeneralGraph.hpp"
#include "graph.hpp"
#include "transaction.hpp"



class LiveGraphInterface: public GeneralGraph {
public:
    void InsertEdge(VertexType src, VertexType dst, WeightType weight) override;
    void InsertVertex(VertexType src) override;
    std::vector<Neighbor> GetNeighbors(VertexType v) override;
    WeightType GetWeightOfEdge(VertexType src, VertexType dst) override;
    void ModifyEdge(VertexType src, VertexType dst, WeightType weight) override;
    void DeleteVertex(VertexType v) override;
    void DeleteEdge(VertexType src, VertexType dst) override;




    LiveGraphInterface(std::string path, bool insertVertex, bool insertEdge );
    LiveGraphInterface(void) = default;
    ~LiveGraphInterface() override = default;
private:
    livegraph::Graph db;
    std::unordered_map<VertexType, uint64_t>  realId2LogicId;
    std::unordered_map<uint64_t, VertexType>  logicId2RealId;
};

 
#endif