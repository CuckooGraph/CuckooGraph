# CuckooGraph

## Introduction

 Graphs play an increasingly important role in various big data applications. However, existing graph data structures cannot simultaneously address the performance bottlenecks caused by the dynamic updates, large scale, and high query complexity of current graphs. This paper proposes a novel data structure for large-scale dynamic graphs called CuckooGraph. It does not require any prior knowledge of the upcoming graphs, and can adaptively resize to the most memory-efficient form while requiring few memory accesses for very fast graph data processing. The key techniques of CuckooGraph include TRANSFORMATION and DENYLIST. TRANSFORMATION fully utilizes the limited memory by designing related data structures that allow flexible space transformations to smoothly expand/tighten the required space depending on the number of incoming items. DENYLIST efficiently handles item insertion failures and further improves processing speed. Our experimental results show that compared with the most competitive solution Spruce, CuckooGraph achieves about 33× higher insertion throughput while requiring only about 68% of the memory space.

## About this repository

* `CuckooGraph` contains the codes of CuckooGraph and WBI.

* `Competitors` contains the codes of the other competitors.

* `CuckooGraph_weighted` contains the codes of CuckooGraph and WBI(weighted version).

* `Syn-Datasets-Code` contains the codes we used to synthesize the DenseGraph dataset and the SparseGraph dataset.

*  `CuckooGraphNeo4j` contains the codes of the neo4j implementation of CuckooGraph
  
  ## How to run
  
  ### Raw CuckooGraph
  
  ### `CuckooGraph` and `CuckooGraph_weighted`
1. make

2. ./cuckoo
   
   ### CuckooGraph on Neo4j
   
   - cd /path/to/CuckooGraphNeo4j
     
   - mvn clean install -DskipTests -Dlicense.skip=true -Doverwrite
     
   - cd /path/to/CuckooGraphNeo4j/packaging/standalone/target/
  
   - tar -xzvf neo4j-community-4.2.0-SNAPSHOT-unix.tar.gz
  
   - Rename the folder obtained after extraction to neo4j.
  
   - cp -r /path/to/CuckooGraph/CuckooGraphNeo4j/TestingAPI/neo4jAPI /path/to/CuckooGraphNeo4j/packaging/standalone/target/
  
   - Modify the reading of dataset in Testing.java.
  
   - cd /path/to/CuckooGraphNeo4j/packaging/standalone/target/neo4jAPI/
  
   - javac -cp .:/path/to/CuckooGraphNeo4j/packaging/standalone/target/neo4j/lib/* Testing.java
  
   - cd /path/to/CuckooGraphNeo4j/packaging/standalone/target/ 
  
   - java -cp .:/path/to/CuckooGraphNeo4j/packaging/standalone/target/neo4j/lib/* neo4jAPI.Testing
     
   ### CuckooGraph on Redis
   
   #### Prerequisites
   
   - redis-5.0.7
   
   - libhiredis-dev
   
   #### Run Redis
   
   - cd /path/to/CuckooGraph
   
   - Process1:  redis-server --loadmodule ./cg.so
   
   - Process2:  redis-cli
   
   redis command examples of CuckooGraph are as follows:
   
   - Query: `CUCKGRPHT.QUERY key srcVertex dstVertex`
   
   - Inset:`CUCKGRPHT.QUERY key srcVertex dstVertex`
   
   - Delete:`CUCKGRPHT.DEL key srcVertex dstVertex`
   
   - GetNeighbors:`CUCKGRPHT.GETNEIGHBORS key vertex`
   
   Also we have implemented the wrapper of redis in C++, you can reference *TestRedis.cpp* and *RedisDriver.cpp* for more information.
   
   ### `Competitors`
   
   #### Prerequisites
- tbb

- pthread

- numa

- event

- boost

- openmp

#### Compilation

Our codes for competitors are in folder **Competitors/**. 

Due to name collision, you should compile the test platform for LiveGraph, Sortletdon and Spruce respectively. The utilities will be compiled as well.

`cd Competitors/`

`mkdir build && cd build`

`cmake .. -Dsystem=livegraph`(or spruce, sortledton. Be aware that all letters are lowercase. If there exists any spelling mistake, the compilation will be aborted.)

`make -j`

`cd bin`

#### Usage

##### Extract subgraph

For example, ./extract stackoverflow ./stackoverflow-1k.txt 1000 /path/to/sx-stackoverflow.txt. We support caida, stackoverflow, notredame, wiki, sina, sparse and dense. The latter two are generated manually. You could customize these two datasets as you wish and we provide a possible way to produce sparse graph. We extracted the subgraph and made its formation the same as stackoverflow, with the third column filled with zero.

##### Start your test

All the executable tests will be named after test-\*. For example, if you want to test lcc on livegraph and repeat 3 times, use: ./test-livegraph lcc 3 ./stackoverflow-1k.txt. We provide the source code of SSSP, BFS, LCC, etc. You can change **src/TestAlgorithm.cpp** and **src/test.cpp** to add more algorithm. If you have many datasets or algorithms, you may find **overalTest.sh** useful.

