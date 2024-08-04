#pragma once
#ifndef SPRUCEINTERFACE
#define SPRUCEINTERFACE


#include "GeneralGraph.hpp"
#include "index_algorithms.h"

class SpruceInterface: public GeneralGraph {
public:
    void InsertEdge(VertexType src, VertexType dst, WeightType weight) override;
    void InsertVertex(VertexType src) override;
    std::vector<Neighbor> GetNeighbors(VertexType v) override;
    WeightType GetWeightOfEdge(VertexType src, VertexType dst) override;
    void ModifyEdge(VertexType src, VertexType dst, WeightType weight) override;
    void DeleteVertex(VertexType v) override;
    void DeleteEdge(VertexType src, VertexType dst) override;
    
    SpruceInterface() = default;
    SpruceInterface(std::string path, bool toLoadVertex, bool toLoadEdge);


private:
    SpruceUniverse db;
};

#endif