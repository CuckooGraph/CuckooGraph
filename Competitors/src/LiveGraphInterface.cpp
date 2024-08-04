#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <string_view>

#include "LiveGraphInterface.hpp"
#include "graph.hpp"
#include "transaction.hpp"
#include "edge_iterator.hpp"

LiveGraphInterface::LiveGraphInterface(std::string path, bool insertVertex, bool insertEdge) {
    LoadData(path);
    try {

        if (insertVertex) {
            auto tx = db.begin_transaction();
            for (auto it = vertex.begin(); it != vertex.end(); it++) {
                uint64_t vertexId = tx.new_vertex(false);
                realId2LogicId[*it] = vertexId;
                logicId2RealId[vertexId] = *it;
            }
            if (insertEdge) {
                for (int i = 0; i < cnt; i++) {
                    uint64_t logicalSrc = realId2LogicId[asrc[i]];
                    uint64_t logicalDst = realId2LogicId[adst[i]];
                    tx.put_edge(logicalSrc, 0, logicalDst, "");
                } 
            }
            tx.commit();
        }
        
    } catch(std::runtime_error str) {
        std::cerr << "LiveGraph Initialization failed: " << str.what() << std::endl;
    }
}


void LiveGraphInterface::InsertEdge(VertexType src, VertexType dst, WeightType weight) {
    if (realId2LogicId.count(src) == 0)
        LiveGraphInterface::InsertVertex(src);
    
    if (realId2LogicId.count(dst) == 0)
        LiveGraphInterface::InsertVertex(dst);

    try {
        livegraph::Transaction tx = db.begin_transaction();
        uint64_t logicalSrc = realId2LogicId[src];
        uint64_t logicalDst = realId2LogicId[dst];
        std::string weightStr = std::to_string(weight);
        tx.put_edge(logicalSrc, 0, logicalDst, weightStr);
        tx.commit();
        
    } catch(std::runtime_error str) {
        std::cerr << "LiveGraph InsertEdge failed: " << str.what() << std::endl;
    }

}
void LiveGraphInterface::InsertVertex(VertexType src) {
    if (realId2LogicId.count(src) > 0)
        return;
    try {
        
        livegraph::Transaction tx = db.begin_transaction();
        
        uint64_t logicId = tx.new_vertex();
        realId2LogicId[src] = logicId;
        logicId2RealId[logicId] = src;
        tx.commit();
    } catch(std::runtime_error str) {

    }

}

std::vector<Neighbor> LiveGraphInterface::GetNeighbors(VertexType v) {
    try {
        std::vector<Neighbor> temp;
        livegraph::Transaction tx = db.begin_read_only_transaction();
        livegraph::EdgeIterator iter = tx.get_edges(realId2LogicId[v], 0);
        while (iter.valid()) {
            VertexType realDstId = logicId2RealId[iter.dst_id()];
            WeightType weight;
            if (iter.edge_data() == "") {
                weight = 0;
            } else {
                weight = static_cast<uint64_t>(std::stoull(std::string(iter.edge_data())));          
            }
            temp.push_back({realDstId, weight});

            iter.next();
        }
        // tx.commit();
        return temp;
    } catch(std::runtime_error str) {
        std::cerr << "LiveGraph GetNeighbors failed: " << str.what() << std::endl;
    }
}

WeightType LiveGraphInterface::GetWeightOfEdge(VertexType src, VertexType dst) {
    try {
        bool nullEdge = false;
        livegraph::Transaction tx = db.begin_read_only_transaction();
        
        uint64_t logicalSrc = realId2LogicId[src];
        uint64_t logicalDst = realId2LogicId[dst];
        std::string_view str = tx.get_edge(logicalSrc, 0, logicalDst);
        
        if (str == "")
            return 0;
        WeightType weight = static_cast<uint64_t>(std::stoull(std::string(str)));
        return weight;
    } catch(std::runtime_error str) {
        std::cerr << "LiveGraph GetWeightOfEdge failed: " << str.what() << std::endl;
    }    
}
void LiveGraphInterface::ModifyEdge(VertexType src, VertexType dst, WeightType weight) {
    InsertEdge(src, dst, weight);
}


void LiveGraphInterface::DeleteVertex(VertexType v) {
    try {
        livegraph::Transaction tx = db.begin_transaction();
        uint64_t logic = realId2LogicId[v];
        tx.del_vertex(logic);
        tx.commit();
    } catch(std::runtime_error str) {
        std::cerr << "LiveGraph DeleteVertex failed: " << str.what() << std::endl;
    }
}

void LiveGraphInterface::DeleteEdge(VertexType src, VertexType dst) {
    try {
        livegraph::Transaction tx = db.begin_transaction();
        uint64_t logicSrc = realId2LogicId[src];
        uint64_t logicDst = realId2LogicId[dst];
        tx.del_edge(logicSrc, 0, logicDst);
        tx.commit();
    } catch(std::runtime_error str) {
        std::cerr << "LiveGraph DeleteEdge failed: " << str.what() << std::endl;
    }
}