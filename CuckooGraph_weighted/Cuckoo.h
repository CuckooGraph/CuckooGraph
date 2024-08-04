#ifndef CUCKOO_H
#define CUCKOO_H

#include <cstring>
#include <cmath>
#include <vector>
#include <fstream>
#include <set>
#include <map>
#include <cstdlib>

#include <chrono>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <immintrin.h> 
#include <assert.h>
#include"BobHash.h"
#include"Util.h"

#define bup 8
#define kicktimes 500

using namespace std::chrono;
using namespace std;

static int debug = 0;
key_type debug_src = -1308072195;
key_type debug_dst = -664855104;

#ifdef TEST_MEM
static int cuckoo_mem = 0;
#endif

#ifdef TEST_KICK
static unsigned long s_kick = 0;
static unsigned long l_kick = 0;
static unsigned long s_hole = 0;
static unsigned long l_hole = 0;
static unsigned long s_num = 0;
static unsigned long l_num = 0;
#endif

int asum;
key_type mykick;
count_type mykick_w;

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

class Bucket {
public:
    
    key_type bslot[bup];
    count_type weight[bup];
    Bucket() {
        memset(bslot, 0, sizeof(key_type) * bup);
        memset(weight, 0, sizeof(count_type) * bup);

    }

    int del(key_type key) {
#ifdef USING_SIMD    

#ifdef SIMD8 
        const __m256i item = _mm256_set1_epi32(key);
        __m256i* keys_p = (__m256i*)(bslot);
        int matched = 0;
        __m256i comp1 = _mm256_cmpeq_epi32(item, *keys_p);
        matched = _mm256_movemask_ps((__m256)comp1);
        if (matched != 0) {
            int matched_index = __tzcnt_u32((uint32_t)matched);
            if (matched_index < bup) {
                bslot[matched_index] = 0;
                return 1;
            }
        }
        return 0;
#else
        const __m512i item = _mm512_set1_epi32(key);
        __m512i* keys_p = (__m512i*)(bslot);
        int matched = 0;
        matched = _mm512_cmpeq_epi32_mask(item, *keys_p);
        if (matched != 0) {
            int matched_index = __tzcnt_u32((uint32_t)matched);
            if (matched_index < bup) {
                bslot[matched_index] = 0;
                return 1;
            }
        }
        return 0;
#endif

#else
        for (int i = 0; i < bup; ++i)
            if (bslot[i] == key) {
                bslot[i] = 0;
                return 1;
            }
        return 0;
#endif
    }

    int insert(key_type key,count_type weigh) {
#ifdef USING_SIMD

#ifdef SIMD8 
        const __m256i item = _mm256_set1_epi32(key);
        __m256i* keys_p = (__m256i*)(bslot);
        int matched = 0;
        
        __m256i comp1 = _mm256_cmpeq_epi32(item, *keys_p);
        matched = _mm256_movemask_ps((__m256)comp1);
        if (matched != 0) return -1;
        const __m256i zero_item = _mm256_set1_epi32(0);
        
        comp1 = _mm256_cmpeq_epi32(zero_item, *keys_p);
        matched = _mm256_movemask_ps((__m256)comp1);
        if (matched != 0) {
            int matched_index = __tzcnt_u32((uint32_t)matched);
            if (matched_index < bup) {
                bslot[matched_index] = key;
                return 1;
            }
        }
        return 0;
#else
        const __m512i item = _mm512_set1_epi32(key);
        __m512i* keys_p = (__m512i*)(bslot);
        int matched = 0;
        matched = _mm512_cmpeq_epi32_mask(item, *keys_p);
        if (matched != 0) return -1;
        const __m512i zero_item = _mm512_set1_epi32(0);
        matched = _mm512_cmpeq_epi32_mask(zero_item, *keys_p);
        if (matched != 0) {
            int matched_index = __tzcnt_u32((uint32_t)matched);
            
            if ((matched_index < bup) && (matched_index >= 0)) {
                bslot[matched_index] = key;
                return 1;
            }
        }
        return 0;
#endif

#else
  
        for (int i = 0; i < bup; ++i) {
            if (bslot[i] == 0) {
                bslot[i] = key;
                weight[i] = weigh;
                return 1;
            }
        }
        return 0;
#endif
    }

    count_type query(key_type key) {
#ifdef USING_SIMD    

#ifdef SIMD8 
        const __m256i item = _mm256_set1_epi32(key);
        __m256i* keys_p = (__m256i*)(bslot);
        int matched = 0;
        
        __m256i comp1 = _mm256_cmpeq_epi32(item, *keys_p);
        matched = _mm256_movemask_ps((__m256)comp1);
        return matched != 0;
#else
        const __m512i item = _mm512_set1_epi32(key);
        __m512i* keys_p = (__m512i*)(bslot);
        int matched = 0;
        matched = _mm512_cmpeq_epi32_mask(item, *keys_p);

        return matched != 0;
#endif

#else
        
        for (int i = 0; i < bup; ++i)
            if (bslot[i] == key)
                return weight[i];
        return 0;
#endif
    }

    int query_insert(key_type key, count_type weigh = 1) {
#ifdef USING_SIMD    

#ifdef SIMD8 
        const __m256i item = _mm256_set1_epi32(key);
        __m256i* keys_p = (__m256i*)(bslot);
        int matched = 0;
        
        __m256i comp1 = _mm256_cmpeq_epi32(item, *keys_p);
        matched = _mm256_movemask_ps((__m256)comp1);
        return matched != 0;
#else
        const __m512i item = _mm512_set1_epi32(key);
        __m512i* keys_p = (__m512i*)(bslot);
        int matched = 0;
        matched = _mm512_cmpeq_epi32_mask(item, *keys_p);

        return matched != 0;
#endif

#else
        
        for (int i = 0; i < bup; ++i)
            if (bslot[i] == key) {
                weight[i] += weigh;
                return 1;
            }
        return 0;
#endif
    }

    bool tq(key_type key) {
        for (int i = 0; i < bup; ++i)
            if (bslot[i] == key)
                return true;
        return false;
    }
};

class Layer {
public:
    Bucket* layer;
    int length;
    int seed;
    Layer() {}

    Layer(int num, int s) :layer(new(std::align_val_t{ 64 })Bucket[num]), length(num), seed(s)
    {

    }

    ~Layer() {
        delete[] layer;
    }

    int del(key_type key) {
        return layer[(int)(mmhash(key, seed) % (uint32_t)length)].del(key);
    }

    count_type query(key_type key) {
        return layer[(int)(mmhash(key, seed) % (uint32_t)length)].query(key);

    }
    int insert(key_type key, count_type weight) {
        return layer[(int)(mmhash(key, seed) % (uint32_t)length)].insert(key, weight);

    }

    int query_insert(key_type key, count_type weight = 1) {
        return layer[(int)(mmhash(key, seed) % (uint32_t)length)].query_insert(key,weight);
    }
};

class Cuckoo {
public:
    int len1;
    int len2;
    int seed1, seed2;
    int num;
    Layer layer1;
    Layer layer2;
    
    Cuckoo(int l1, int l2, int s1, int s2) :len1(l1), len2(l2), seed1(s1), seed2(s2), num(0), layer1(l1, s1), layer2(l2, s2)
    {

    }

#ifdef TEST_MEM

    int get_mem() {
        int mr = 0;
        mr += sizeof(Cuckoo);
        mr += sizeof(Bucket) * (layer1.length);
        mr += sizeof(Bucket) * (layer2.length);
        return mr;
    }
#endif     

    bool insert(key_type item, count_type weight=1) {
#ifdef TEST_KICK
        s_kick = 0;
        s_num += 1;
#endif
        int ir = layer1.insert(item, weight);
        if (ir) {
            if (ir == 1)
                ++num;
            return true;
        }
        ir = layer2.insert(item, weight);
        if (ir) {
            if (ir == 1)
                ++num;
            return true;
        }
        int times = kicktimes;
        bool flag = true;
        int pos1 = (int)(mmhash(item, seed1) % (uint32_t)len1);
        int pos2 = (int)(mmhash(item, seed2) % (uint32_t)len2);
        key_type kick = item;
        count_type kick_w = weight;
        key_type ins = item;
        count_type ins_w = weight;
        while (times) {
            if (flag) {
                int xx = rand();
                kick = layer1.layer[pos1].bslot[xx % bup];
                kick_w = layer1.layer[pos1].weight[xx % bup];
                layer1.layer[pos1].bslot[xx % bup] = ins;
                layer1.layer[pos1].weight[xx % bup] = ins_w;
                int f = layer2.insert(kick, kick_w);
                if (f) {
                    if (f == 1)
                        ++num;
#ifdef TEST_KICK
                 s_kick = kicktimes +1 - times;
                 s_hole += s_kick;
#endif
                    return true;
                }
                --times;
                flag = false;
                ins = kick;
                ins_w = kick_w;
                pos2 = (int)(mmhash(ins, seed2) % (uint32_t)len2);
            }
            else {
                int xx = rand();
                kick = layer2.layer[pos2].bslot[xx % bup];
                kick_w = layer2.layer[pos2].weight[xx % bup];
                layer2.layer[pos2].bslot[xx % bup] = ins;
                layer2.layer[pos2].weight[xx % bup] = ins_w;
                int f = layer1.insert(kick,kick_w);
                if (f)
                {
                    if (f == 1)
                        ++num;
#ifdef TEST_KICK
                    s_kick = kicktimes + 1 - times;
                    s_hole += s_kick;
#endif
                    return true;
                }
                --times;
                flag = true;
                ins = kick;
                ins_w = kick_w;
                pos1 = (int)(mmhash(ins, seed1) % (uint32_t)len1);
            }
        }

        mykick = ins;
        mykick_w = ins_w;
#ifdef TEST_KICK
        s_kick = kicktimes;
        s_hole += s_kick;
#endif
        return false;
    }

    count_type query(key_type item) {
        int q = layer1.query(item);
        if (q)return q;
        q = layer2.query(item);
        if (q)return q;
        return q;
    }

    int query_insert(key_type item,count_type weight=1) {
        int q = layer1.query_insert(item,weight);
        if (q)return q;
        q = layer2.query_insert(item,weight);
        if (q)return q;
        return q;
    }

    int del(key_type item) {
        if (layer1.del(item)) {
            --num;
            return 1;
        }
        if (layer2.del(item)) {
            --num;
            return 1;
        }
        return 0;
    }
};

class tBucket {
public:
    
    key_type bslot[bup];
    tBucket() {
        memset(bslot, 0, sizeof(key_type) * bup);
    }

    int insert(key_type key) {
        if (query(key)) return -1;
        for (int i = 0; i < bup; ++i) {
            if (bslot[i] == 0) {
                bslot[i] = key;
                return 1;
            }
        }
        return 0;
    }

    bool query(key_type key) {
        for (int i = 0; i < bup; ++i)
            if (bslot[i] == key)
                return true;
        return false;
    }

    bool tq(key_type key) {
        for (int i = 0; i < bup; ++i)
            if (bslot[i] == key)
                return true;
        return false;
    }
};

#define CMlen 300000

#define CMl 3
#define CMS1 0
#define CMS2 221
#define CMS3 19
static int cmseeds[3] = { 0,221,19 };
class CMSketch {
public:
    count_type cm[CMl][CMlen];
    CMSketch() {
        memset(cm, 0, sizeof(count_type) * CMl * CMlen);
    }

    count_type query(key_type key) {
        count_type res = 0xffffffff;
        for (int i = 0; i < CMl; ++i) {
            res = std::min(res, cm[i][mmhash(key, cmseeds[i]) % CMlen]);
        }
        return res;
    }

    void insert(key_type key) {
        for (int i = 0; i < CMl; ++i) {
            ++cm[i][mmhash(key, cmseeds[i]) % CMlen];
        }

    }
};

#endif