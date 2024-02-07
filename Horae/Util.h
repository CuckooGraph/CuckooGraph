#ifndef UTIL_H
#define UTIL_H


#include <algorithm>
#include <string>
#include <random>
#include <unordered_map>
#include <climits>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <fstream>
#include <ctime>



#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

#define CONSTANT_NUMBER 2654435761u
#define mmhash hash32
static const  uint64_t big_prime[] = { 2654435761u ,3221225473,290182597,2147483647 };


#define TUPLES_LEN 13
using namespace std;

struct TUPLES {
public:
	uint8_t array[TUPLES_LEN];
};


typedef int key_type;

bool operator == (const TUPLES& a, const TUPLES& b) {
	return memcmp(a.array, b.array, sizeof(TUPLES)) == 0;
}

struct TIMESTAMP {
	uint8_t array[8];
};

static key_type precursor[63497059] = { 0 };
static key_type successor[63497059] = { 0 };
static key_type precursor1[63497059] = { 0 };
static key_type successor1[63497059] = { 0 };
queue<key_type> need;

typedef uint32_t count_type;

typedef struct Node_degree
{
	key_type                node;
	key_type              val;
} Node_degree;

bool Compare(const Node_degree& a, const Node_degree& b) {
	if (a.val == b.val)return a.node < b.node;
	return a.val < b.val;
}

class Edge {
public:
	key_type src;
	key_type dst;
};

class EdgeLess {
public:
	bool operator()(const Edge& e1, const Edge& e2)const {
		if (e1.src == e2.src)
			return e1.dst < e2.dst;
		return e1.src < e2.src;
	}
};

int asrc[63497059];
int adst[63497059];
#endif