#include <iostream>
#include <string>
#include <vector>

#include "GeneralGraph.hpp"
#include "SortledtonInterface.hpp"
#include "VersionedBlockedPropertyEdgeIterator.h"
#include "EdgeDoesNotExistsPrecondition.h"

SortledtonInterface::SortledtonInterface(): 
    tm(64), weightSize(sizeof(WeightType)) {
    sortledton = new VersioningBlockedSkipListAdjacencyList(512, weightSize, tm);
    SnapshotTransaction tx = tm.getSnapshotTransaction(sortledton, true);
    tx.insert_vertex(0);
    tx.execute();
    tm.transactionCompleted(tx);
};

SortledtonInterface::SortledtonInterface(std::string path, bool toLoadVertex, bool toLoadEdge):
    tm(64), weightSize(sizeof(WeightType)){
    sortledton = new VersioningBlockedSkipListAdjacencyList(512, weightSize, tm);
    LoadData(path);
    try {
            if (toLoadVertex) {
                for (auto it = vertex.begin(); it != vertex.end(); it++) {
                    InsertVertex(*it);
                }
                if (toLoadEdge) {
                    for (int i = 0; i < cnt; i++) {
                        InsertEdge(asrc[i], adst[i], 0);
                    } 
            }
        }

        
    } catch(std::runtime_error str) {
        std::cerr << "Sortledton Initialization failed: " << str.what() << std::endl;
    }
}

SortledtonInterface::~SortledtonInterface() {
}
void SortledtonInterface::InsertVertex(VertexType src) {
    SnapshotTransaction tx = tm.getSnapshotTransaction(sortledton, false);
    if (!tx.has_vertex(src)) {
        tx.insert_vertex(src);
    }
    tx.execute();
    tm.transactionCompleted(tx);
}



void SortledtonInterface::InsertEdge(VertexType src, VertexType dst, WeightType weight) {
    auto tx2 = tm.getSnapshotTransaction(sortledton, false);
    bool shoudReturn = false;
    if (tx2.has_edge({src, dst})) {
        shoudReturn = true;
    }
    tm.transactionCompleted(tx2);
    if (shoudReturn) {
        return;
    }

    edge_t tempEdge{src, dst};
    tx2.insert_edge(tempEdge, (char *)&weight, 8);

    tx2.execute();
    tm.transactionCompleted(tx2);
}


std::vector<Neighbor> SortledtonInterface::GetNeighbors(VertexType v) {

    try {

    
    SnapshotTransaction tx = tm.getSnapshotTransaction(sortledton, false);
    VersionedBlockedEdgeIterator iter = tx.neighbourhood_blocked_p(tx.physical_id(v));
    std::vector<Neighbor> temp;

    while (iter.has_next_block()) {
        while (iter.has_next_edge()) {
            VertexType dst = iter.next();
            WeightType weight;
            temp.push_back({tx.logical_id(dst), weight});
            tx.execute();
        }
        iter.next_block();
    }
    for (auto &edge: temp) {
        tx.get_weight_p({tx.physical_id(v), tx.physical_id(edge.first)}, (char *)&edge.second);
    }
    tx.execute();
    tm.transactionCompleted(tx);

    return temp;
    } catch (IllegalOperation str) {
        std::cerr << str.what << std::endl;
    }
}

WeightType SortledtonInterface::GetWeightOfEdge(VertexType src, VertexType dst) {
    bool nullEdge = false;
    SnapshotTransaction tx = tm.getSnapshotTransaction(sortledton, false);
    edge_t tempEdge{src, dst};
    WeightType a;
    if (tx.has_edge(tempEdge))
        tx.get_weight(tempEdge, (char *)&a);
    else
        nullEdge = true;
    tm.transactionCompleted(tx);
    return nullEdge ? 0 : a;
}

void SortledtonInterface::ModifyEdge(VertexType src, VertexType dst, WeightType weight) {
    bool shoudDelete = false;
    SnapshotTransaction tx = tm.getSnapshotTransaction(sortledton, false);
    if (tx.has_edge({src, dst})) {
        shoudDelete = true;
    }
    if (shoudDelete) {
        tx.delete_edge({src, dst});
        tx.execute();
    }
    tm.transactionCompleted(tx);
    
    tx = tm.getSnapshotTransaction(sortledton, true);
    tx.insert_edge({src, dst}, (char *)&weight, weightSize);
    tx.execute();
    tm.transactionCompleted(tx);
}


void SortledtonInterface::DeleteVertex(VertexType v) {
    std::cerr << "Sortledton DeleteVertex: This method was not implemented by Sortledton Author" << std::endl;
    exit(1);
    SnapshotTransaction tx = tm.getSnapshotTransaction(sortledton, true);
    tx.delete_vertex(v);
    tx.execute();
    tm.transactionCompleted(tx);
}

void SortledtonInterface::DeleteEdge(VertexType src, VertexType dst) {
    SnapshotTransaction tx = tm.getSnapshotTransaction(sortledton, true);
    tx.delete_edge({src, dst});
    tx.execute();
    tm.transactionCompleted(tx);
}
