#pragma once
#ifndef SORTLEDTONINTERFACE
#define SORTLEDTONINTERFACE

#include <iostream>
#include <inttypes.h>
#include <vector>
#include <string>
#include "GeneralGraph.hpp" 
#include "VersioningBlockedSkipListAdjacencyList.h"
#include "TransactionManager.h"


class SortledtonInterface: public GeneralGraph {
public:

    void InsertEdge(VertexType src, VertexType dst, WeightType weight) override;
    void InsertVertex(VertexType src) override;
    std::vector<Neighbor> GetNeighbors(VertexType v) override;
    WeightType GetWeightOfEdge(VertexType src, VertexType dst) override;
    void ModifyEdge(VertexType src, VertexType dst, WeightType weight) override;
    void DeleteVertex(VertexType v) override;
    void DeleteEdge(VertexType src, VertexType dst) override;


    SortledtonInterface();
    SortledtonInterface(std::string path, bool toLoadVertex, bool toLoadEdge);
    ~SortledtonInterface() override;
    TransactionManager tm;

private:
    
    VersioningBlockedSkipListAdjacencyList *sortledton;
    int weightSize;
};

#endif