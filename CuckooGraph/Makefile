CPFLAGS = -rdynamic -mavx2 -mbmi -mavx512bw -mavx512dq --std=c++17 -O3

cuckoo: main.cpp  Cuckoo.h Util.h BobHash.h WindBell.h GSS.h GraphUndirected.h hashTable.h querysupportstruct.h Auxo.h tcm.h hashFunction.h hash.h CuckooGraphInterface.h
	g++ -mavx2 main.cpp -o cuckoo  $(CPFLAGS) -g
	g++ main.cpp -I . -fPIC -c -o libcuckoograph.a -rdynamic -mavx2 -mbmi -mavx512bw -mavx512dq -lstdc++ --std=c++17 -g
	gcc CuckooGraphType.c -I ./ -L ./ -lcuckoograph -fPIC -shared -o cg.so -lstdc++ -g
	g++ TestRedis.cpp redisDriver.cpp cankao.cpp -L ./ -lhiredis -o TestRedis --std=c++17 -g


clean: 
	rm cuckoo

