# CuckooGraph
## Introduction
 Graphs play an increasingly important role in various big data applications. 
 However, the performance of graph storage is challenged due to the dynamic update, 
 large scale, and high query complexity of graphs. Existing graph storage cannot address the above challenges simultaneously. 
 This paper proposes a novel storage scheme for large-scale dynamic graphs called CuckooGraph. 
 It can be flexibly adapted to unknown graphs of arbitrary size, while
 significantly reducing the time for insertion and various query
 tasks. The key techniques of CuckooGraph include Transformation and Denylist. 
 Transformation makes good use of limited memory space by designing related data structures into a flexible
 and spatially transformable form to smoothly expand or tighten the memory based on the number of incoming items. 
 Denylist addresses the problem that items may fail to be inserted into the
 original cuckoo hash table and generate errors, ensuring that CuckooGraph is absolutely error-free and time efficient.
 We conduct extensive experiments. The results show that CuckooGraph significantly reduces query time by four orders of magnitude on 1-hop
 successor and precursor queries compared to the state-of-the-art.
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

## About parameter settings of the graph stream summarization schemes in our experiments
* `TCM`: width=19200 depth=19200 hashnum=9 usetable=false
* `GSS`: width=19200 range=16 p_num=16 size=2 f_num=16 usehashtable=false
* `Auxo`: width=100 range=4 p_num=16 f_num=16
* `Horae`: startTime=0 timesliceLength=61200 gl=1 width=6500 depth=6500 fingerprintLength=7 cache_align=false kick=false row_addrs=4 column_addrs=4


