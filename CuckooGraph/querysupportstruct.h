#ifndef Q_H_INCLUDED
#define Q_H_INCLUDED
#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<set>
#include<map>
#include<cmath>
#include<stdlib.h>
#include<bitset>
#include<memory.h>
#include <sys/time.h>
#include "omp.h"
#include<time.h>
#include <fstream>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <sstream>

using namespace std;

#define UNMAX 0xffffffff

#define prime 739
#define bigger_p 1048576
#define timer 5

#define Type 0
typedef unsigned long long uint64;
typedef unsigned short weightSize;
typedef unsigned int ui;
typedef unsigned long long ul;
typedef unsigned char uc;

class edgeval{
public:
    unsigned int val;
    string from,to;
    edgeval(string _from,string _to,unsigned int _val);
    bool operator < (const edgeval& b)const;
};



class heap{
public:
    edgeval **num;
    int s;
    void heapSubTree(int k,int n);
    heap(){}
    void initialHeap(int l);
    int find(string s1,string s2);
    ~heap();
    void pop();
    void push(string s1,string s2,unsigned int weight);
    bool increase(string s1,string s2,unsigned int weight);
    unsigned int top();
    void topres(string &s1,string &s2);
    bool empty();
    int size();

};

class Blizzard_MPQ{
private:
    ul MagicTable[0x500];
public:
    Blizzard_MPQ();
    ul MPQHash(const char* str, ul HashType);

};


class Gbasket
{
public:
	uint64 *srcFig,*dstFig;
	uint64 *idx;
	weightSize* weight;
	Gbasket(uint64* _srcFig,uint64*_dstFig,uint64* _idx,weightSize* _weight);
	~Gbasket();
};
class basketNode{
public:
    Gbasket *value;
    basketNode *next[2];
    double bound;
    int level;

    basketNode();
    ~basketNode();
};




class Auxo
{
private:
	int w;
	int r;
	int p;
	int f;
	int rfnum;
	int halfR;
	int rper;
	unsigned int* fvalue;
	int *fper;
	Blizzard_MPQ *mpq;

	vector<basketNode*> checkMatrices;
    vector<int> posfind;
    vector<int> weiRes;
    vector<int> matrixLevel;


	public:
	    double fullCount;
	    double loadFactor;
	    basketNode *mainTree;
	    ul valueCou;
	    uint64 memoryAllocated1,memoryAllocated2,memoryAllocated3;
	    double insertNum;
	    ul matrixSize;
		Auxo(int width, int range, int p_num, int f_num);
		~Auxo()
		{
			freeValueTree(mainTree);
			delete []fvalue;
			delete []fper;

		 }
		 Gbasket* initialGbasket(int fsize1,int fsize2);
		 void MatrixAppend(basketNode* basketCurr,int level);
		 bool insert(string s1, string s2,int weight=1);
		 void cleanupBuffer();
		 int edgeQuery(string s1, string s2);
		 bool query(string s1, string s2);
		 int nodeValueQuery(string s1, int type, double& queryMatrix);
		 int nodeDegreeQuery(string s1, int type);
		 void nodeSuccessorQuery(string s1, vector<string> &IDs);
		 void nodePrecursorQuery(string s2, vector<string> &IDs);
		 unsigned int getNodeValue(basketNode* basketCurr,int level,unsigned int *poses, unsigned int g1,bool type);
		 void freeValueTree(basketNode* root);
		 void sucessorQuery(basketNode* basketCurr,int level,unsigned int *poses, ul g1,vector<ul>& neis,ul prefix,int preLen,bool type);
		 bool reachQuery(string s1,string s2,double &queryTime);
};



ul calHash(ul tmp_g,ul tmp_s,int k,int w);

#endif 
