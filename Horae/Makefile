CPFLAGS = -g -rdynamic -mavx2 -mbmi -mavx512bw -mavx512dq --std=c++17 -O3  -lpthread

cuckoo: main.cpp Util.h 
	g++ -mavx2 main.cpp -o cuckoo $(CPFLAGS)

clean: 
	rm cuckoo


