CPFLAGS = -g -rdynamic -mavx2 -mbmi -mavx512bw -mavx512dq --std=c++17 -O3

cuckoo: main.cpp Cuckoo.h Util.h BobHash.h WindBell.h GSS.h GraphUndirected.h hashTable.h querysupportstruct.h Auxo.h tcm.h hashFunction.h hash.h 
	g++ -mavx2 main.cpp -o cuckoo $(CPFLAGS)

clean: 
	rm cuckoo

