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
#include "BobHash.h"




#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

#define CONSTANT_NUMBER 2654435761u
#define mmhash hash32


static const  uint64_t big_prime[] = {2654435761u ,3221225473,290182597,2147483647 };





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

static key_type precursor[63497059]={0};
static key_type successor[63497059]={0};
static key_type precursor1[63497059]={0};
static key_type successor1[63497059]={0};
queue<key_type> need;







 



typedef uint32_t count_type; 


inline uint64_t hash64(key_type item, uint32_t seed = 0) {
    return Hash::BOBHash64((uint8_t*)&item, sizeof(key_type), seed);
}

inline uint32_t hash32(key_type item, uint32_t seed = 0) {
    return Hash::BOBHash64((uint8_t*)&item, sizeof(key_type), seed);
}

inline uint32_t mhash(key_type fp, uint32_t seed = 0) {return (((fp) * (CONSTANT_NUMBER+seed)) >> 15);}

inline uint32_t ourhash(key_type fp, uint32_t seed = 0) {
    return (((fp) * (big_prime[seed%4])) >> 15);
}


typedef struct Node_degree
{
    key_type                node;             
    key_type              val;
} Node_degree;

bool Compare(const Node_degree& a, const Node_degree& b) {
    if(a.val == b.val)return a.node < b.node;
    return a.val < b.val;
}

using Neighbor = std::pair<key_type, count_type>;

typedef key_type VertexType;
typedef count_type WeightType;

#endif