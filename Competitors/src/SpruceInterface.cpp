#include "SpruceInterface.hpp"
#include <vector>

SpruceInterface::SpruceInterface(std::string path, bool toLoadVertex, bool toLoadEdge)
{
    LoadData(path);
    if (toLoadEdge) {
        for (int i = 0; i < cnt; i++) {
            SpruceInterface::InsertEdge(asrc[i], adst[i], 0);
        }
    }
}
void SpruceInterface::InsertEdge(VertexType src, VertexType dst, WeightType weight) 
{
    try
    {
        SpruceUniverse::WeightedEdge edge{src, dst, static_cast<double>(weight)};
        SpruceUniverse::InsertEdge(db, edge);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Spruce InsertEdge failed:" << e.what() << std::endl;
    }
}

void SpruceInterface::InsertVertex(VertexType src)
{
    std::cerr << "Spruce does not support vertex insertion!" << std::endl;
    exit(0);
}

std::vector<Neighbor> SpruceInterface::GetNeighbors(VertexType v)
{
    try
    {
        std::vector<SpruceUniverse::WeightedOutEdge> temp;
        SpruceUniverse::get_neighbours(db, v, temp);
        std::vector<Neighbor> ans;
        for (const auto &edge: temp)
        {
            ans.push_back({edge.des, static_cast<WeightType>(edge.weight)});
        }    
        return ans;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Spruce GetNeighbors failed:" << e.what() << std::endl;
    }
    
}

WeightType SpruceInterface::GetWeightOfEdge(VertexType src, VertexType dst)
{
    std::vector<Neighbor> neighbors = SpruceInterface::GetNeighbors(src);
    for (const auto& edge: neighbors)
    {
        if (dst == edge.first)
            return edge.second;
    }
    return 0;
}

void SpruceInterface::ModifyEdge(VertexType src, VertexType dst, WeightType weight) 
{
    try
    {
        SpruceUniverse::WeightedEdge edge{src, dst, static_cast<double>(weight)};
        SpruceUniverse::UpdateEdge(db, edge);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Spruce ModifyEdge failed:" << e.what() << std::endl;
    }
    
}

void SpruceInterface::DeleteEdge(VertexType src, VertexType dst)
{
    try
    {
        SpruceUniverse::DeleteEdge(db, src, dst);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Spruce DeleteEdge failed:" << e.what() << std::endl;
    }
    
}

void SpruceInterface::DeleteVertex(VertexType v)
{
    std::vector<Neighbor> neighbors = SpruceInterface::GetNeighbors(v);
    for (const auto& edge: neighbors)
    {
        VertexType dst = edge.first;
        SpruceInterface::DeleteEdge(v, dst);
    }
}