# CuckooGraph
## Introduction
 Graph streams play an increasingly important role in various big data 
 applications, but their storage and query are facing challenges
 because graph streams have the characteristics of large-scale, 
 dynamic updates, and high query complexity. Existing graph storage
 solutions are either too time- and space-inefficient or fail to be
 error-free. This paper proposes a novel error-free storage scheme
 for graph streams called CuckooGraph. It can flexibly adapt to 
 unknown graph streams of any size, while significantly reducing the
 time for insertion and various query tasks. The key techniques of
 CuckooGraph include Transformation and Blacklist. Transformation 
 makes good use of limited memory space by designing related
 data structures into a flexible and spatially transformable form to
 smoothly expand or tighten the memory based on the number of
 incoming items. Blacklist addresses the problem that items may
 fail to be inserted into the original cuckoo hash table and generate
 errors, ensuring that CuckooGraph is absolutely error-free and 
 improves efficiency. We conduct extensive experiments on large-scale
 real-world datasets to evaluate the performance of CuckooGraph.
 The results show that, compared with the state-of-the-art, CuckooGraph 
 significantly reduces the query time by about four orders of
 magnitude on the 1-hop successor and precursor queries.
## About this repository
* `CuckooGraph` contains the codes of CuckooGraph and the other algorithms.

## About settings of the L-CHT length in our experiments
* In all the related experiments, the results of CuckooGraph are shown for both pre-expansion (noTransformation, NT)
& post-expansion (Transformation, T) cases of L-CHT in CuckooGraph,where T refers to setting the L-CHT to expand from length 4, 
and NT is set to make the L-CHT loading rate high but does not happen to expand.
(Accordingto our observations, S-CHT must expand, so we focus on observing the performance of CuckooGraph when L-CHT expands.)
The settings of L-CHT length in CuckooGraph(NT) in each experiments are as follows.
### Experiments on Edge Queries
* `CAIDA`: 28136
* `NotreDame`: 14108
* `StackOverflow`: 245816
* `WikiTalk`: 73984
### Experiments on 1-Hop Successor Queries
* `CAIDA`: 28136
* `NotreDame`: 14108
* `StackOverflow`: 245816
* `WikiTalk`: 73984
### Experiments on 1-Hop Precursor Queries
* `CAIDA`: 28540
* `NotreDame`: 32600
* `StackOverflow`: 253920
* `WikiTalk`: 313648
### Experiments on Finding Triangles
* `CAIDA`: 28136
* `NotreDame`: 14108
* `StackOverflow`: 245816
* `WikiTalk`: 73984
### Experiments on Single Source Shorted Path
* `CAIDA`: 28136
* `NotreDame`: 16000
* `StackOverflow`: 245816
* `WikiTalk`: 78000

## About the setting of kicked out threshold
In our codes, we define `kicktimes` as the threshold which the total number of kicked out can't exceed. It is defined in `Cuckoo.h`. `kicktimes` is set to 500 in all the experiments of our paper.

