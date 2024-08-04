#ifndef WINDBELL_H
#define WINDBELL_H





#include <stdio.h>
#include <string.h>
#include <fstream>
#include <bitset>
#include <time.h>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <unordered_set>

#include "BobHash.h"
#include"Util.h"
using namespace std;
const int MAX_NUM1 = 128;
const int MAX_NUM2 = 256;
int MAXNUM = 5 * 1e7;
const int ID_LEN = 4;
#define OptFilter
#define OptRectangle
#define OptCross
#define OptCuckoo

#ifdef TEST_MEM
static uint64_t wind_mem = 0;
#endif

static int rawseed[2] = { 0,221 };
static int colseed[2] = { 0,13 };


struct chain {
	int src;
	int dst;
	chain* pointer_chand;
	chain* pointer_chand_re;





};

struct listChain {
	int src;
	int dst;
	listChain* pointer_suc;
	listChain* pointer_suc_re;
	listChain* pointer_pre;
	listChain* pointer_pre_re;

};

class Chandelier
{
	static const int MAX_TCM = 7000;
public:
	int raw;
	int col;
	int len;
	int counter[MAX_TCM][MAX_TCM];
	chain* pointer_head[MAX_TCM][MAX_TCM];
	chain* pointer_tail[MAX_TCM][MAX_TCM];


	Chandelier(int length, int raw, int column);
	~Chandelier() {};
	void insert(chain* edge_new, int num);
	void insert_new(chain* edge_new, int num);
	bool query_edge(chain* edge_q);
	int query_success(int src);
	int query_successor(int src, key_type successor[], int U);
	vector<Neighbor> GetNeighbors(key_type src);
	int query_precursor(int dst, key_type precursor[], int U);
	int query_triangles(key_type src);
	int query_triangle_new(key_type src);
	bool delete_edge(chain* edge_q);
};

Chandelier::Chandelier(int length = 2000, int r = 2, int c = 2) {
	raw = r;
	col = c;
	len = length;
	printf("len:%d\n", length);






	for (int i = 0; i < length; ++i)
		for (int j = 0; j < length; ++j) {
			counter[i][j] = 0;
			pointer_head[i][j] = 0;
			pointer_tail[i][j] = 0;
		}


#ifdef TEST_MEM
	wind_mem = 3 * sizeof(int) + len * len * sizeof(int) + len * len * sizeof(chain*) + len * len * sizeof(chain*);
#endif   
}

int min(int* arr, int length) {
	int minn = MAXNUM;
	int index = -1;
	for (int i = 0; i < length; ++i) {
		if (arr[i] < minn) {
			minn = arr[i];
			index = i;
		}
	}
	return index;
}

int max(int* arr, int length) {
	int maxx = -1;
	int index = -1;
	for (int i = 0; i < length; ++i) {
		if (arr[i] > maxx) {
			maxx = arr[i];
			index = i;
		}
	}
	return index;
}

void Chandelier::insert(chain* edge_new, int num = 0) {
	chain* temp;
	bool ans = query_edge(edge_new);
	unsigned int ra[raw];
	unsigned int co[col];
	int cnt[raw * col];
	if (ans == false) {

#ifdef TEST_MEM
		wind_mem = wind_mem + sizeof(chain);
#endif

		for (int i = 0; i < raw; ++i) {
			ra[i] = (mmhash(edge_new->src, rawseed[i])) % len;
		}
		for (int i = 0; i < col; ++i) {
			co[i] = mmhash((edge_new->dst), colseed[i]) % len;
		}
		for (int i = 0; i < raw; ++i)
			for (int j = 0; j < col; ++j) {
				cnt[i * col + j] = counter[ra[i]][co[j]];
			}
		int minn = min(cnt, raw * col);

		int r = minn / col;
		int c = minn % col;
		r = ra[r];
		c = co[c];
		temp = pointer_head[r][c];
		edge_new->pointer_chand = temp;
		if (temp != nullptr)
			temp->pointer_chand_re = edge_new;
		else
			pointer_tail[r][c] = edge_new;
		pointer_head[r][c] = edge_new;
		++counter[r][c];

#ifdef OptCuckoo
		int maxx = max(cnt, raw * col);
		if (cnt[maxx] > 10 && cnt[minn] * 2 < cnt[maxx] && num < 3) {
			r = maxx / col;
			c = maxx % col;
			r = ra[r];
			c = co[c];
			temp = pointer_tail[r][c];
			pointer_tail[r][c] = pointer_tail[r][c]->pointer_chand_re;
			if (pointer_tail[r][c] == nullptr)
				pointer_head[r][c] = nullptr;
			else
				pointer_tail[r][c]->pointer_chand = nullptr;
			--counter[r][c];
			insert_new(temp, num + 1);
		}
#endif
	}



}

void Chandelier::insert_new(chain* edge_new, int num = 0) {
	unsigned int ra[raw];
	unsigned int co[col];
	int cnt[raw * col];
	chain* temp;
	for (int i = 0; i < raw; ++i) {
		ra[i] = (mmhash(edge_new->src, rawseed[i])) % len;
	}
	for (int i = 0; i < col; ++i) {
		co[i] = mmhash((edge_new->dst), colseed[i]) % len;
	}
	for (int i = 0; i < raw; ++i)
		for (int j = 0; j < col; ++j) {
			cnt[i * col + j] = counter[ra[i]][co[j]];
		}
	int minn = min(cnt, raw * col);


	int r = minn / col;
	int c = minn % col;
	r = ra[r];
	c = co[c];
	temp = pointer_head[r][c];
	edge_new->pointer_chand = temp;
	if (temp != nullptr)
		temp->pointer_chand_re = edge_new;
	else {
		pointer_tail[r][c] = edge_new;
	}
	pointer_head[r][c] = edge_new;
	++counter[r][c];

#ifdef OptCuckoo
	int maxx = max(cnt, raw * col);
	if (cnt[maxx] > 10 && cnt[minn] * 2 < cnt[maxx] && num < 3) {
		r = maxx / col;
		c = maxx % col;
		r = ra[r];
		c = co[c];
		temp = pointer_tail[r][c];
		pointer_tail[r][c] = temp->pointer_chand_re;
		if (pointer_tail[r][c] == nullptr)
			pointer_head[r][c] = nullptr;
		else
			pointer_tail[r][c]->pointer_chand = nullptr;
		--counter[r][c];
		insert_new(temp, num + 1);
	}
#endif
}


bool cmp(chain* a, chain* b) {
	if (a->src == b->src && a->dst == b->dst)
		return true;
	return false;

}

bool Chandelier::query_edge(chain* edge_q) {
	unsigned int ra[raw];
	unsigned int co[col];
	for (int i = 0; i < raw; ++i) {
		ra[i] = (mmhash(edge_q->src, rawseed[i])) % len;
	}
	for (int i = 0; i < col; ++i) {
		co[i] = mmhash((edge_q->dst), colseed[i]) % len;
	}

	int cnt[raw * col];
	for (int i = 0; i < raw; ++i)
		for (int j = 0; j < col; ++j) {
			cnt[i * col + j] = counter[ra[i]][co[j]];
		}
	int minn = min(cnt, raw * col);
	int r = minn / col;
	int c = minn % col;
	chain* temp = pointer_head[ra[r]][co[c]];
	for (; temp != nullptr; temp = temp->pointer_chand) {
		if (cmp(edge_q, temp))
			return true;
	}

	for (int i = 0; i < raw; ++i) {
		for (int j = 0; j < col; ++j) {

			if (i * col + j == minn)
				continue;
			temp = pointer_head[ra[i]][co[j]];
			for (; temp != nullptr; temp = temp->pointer_chand) {
				if (cmp(edge_q, temp))
					return true;
			}
		}
	}
	return false;
}

bool Chandelier::delete_edge(chain* edge_q) {

	unsigned int ra[raw];
	unsigned int co[col];
	for (int i = 0; i < raw; ++i) {
		ra[i] = (mmhash(edge_q->src, rawseed[i])) % len;
	}
	for (int i = 0; i < col; ++i) {
		co[i] = mmhash((edge_q->dst), colseed[i]) % len;
	}

	int cnt[raw * col];
	for (int i = 0; i < raw; ++i)
		for (int j = 0; j < col; ++j) {
			cnt[i * col + j] = counter[ra[i]][co[j]];
		}
	int minn = min(cnt, raw * col);
	int r = minn / col;
	int c = minn % col;
	chain* temp = pointer_head[ra[r]][co[c]];

	for (; temp != nullptr; temp = temp->pointer_chand) {


		if (cmp(edge_q, temp)) {

			if (pointer_head[ra[r]][co[c]] == temp) {

				pointer_head[ra[r]][co[c]] = temp->pointer_chand;

			}
			else {

				temp->pointer_chand_re->pointer_chand = temp->pointer_chand;

			}
			if (temp->pointer_chand != nullptr) {

				temp->pointer_chand->pointer_chand_re = temp->pointer_chand;

			}
			else {

				pointer_tail[ra[r]][co[c]] = temp->pointer_chand_re;

			}
			--counter[ra[r]][co[c]];
			delete temp;
			return true;
		}
	}

	for (int i = 0; i < raw; ++i) {
		for (int j = 0; j < col; ++j) {

			if (i * col + j == minn)
				continue;
			temp = pointer_head[ra[i]][co[j]];
			for (; temp != nullptr; temp = temp->pointer_chand) {
				if (cmp(edge_q, temp))
				{
					if (pointer_head[ra[i]][co[j]] == temp) {

						pointer_head[ra[i]][co[j]] = temp->pointer_chand;

					}
					else {

						temp->pointer_chand_re->pointer_chand = temp->pointer_chand;

					}
					if (temp->pointer_chand != nullptr) {

						temp->pointer_chand->pointer_chand_re = temp->pointer_chand;

					}
					else {

						pointer_tail[ra[i]][co[j]] = temp->pointer_chand_re;

					}
					--counter[ra[i]][co[j]];
					delete temp;
					return true;
				}
			}
		}
	}
	return false;
}

int Chandelier::query_success(int src_q) {
	unsigned int ra[raw];
	int cnt = 0;
	for (int i = 0; i < raw; ++i) {
		ra[i] = mmhash(src_q, rawseed[i]) % len;
	}
	int i = 0;
	if (ra[0] == ra[1])
		i = 1;
	for (; i < raw; ++i) {
		for (int j = 0; j < len; ++j) {
			chain* temp = pointer_head[ra[i]][j];
			for (; temp != nullptr; temp = temp->pointer_chand) {
				if (temp->src == src_q) {
					cnt++;

				}
			}
		}
	}
	return cnt;
}
const int MAX_Threshold = 500000;

int Chandelier::query_successor(int src_q, key_type successor[], int U) {
	unsigned int ra[raw];
	int cnt = U;
	for (int i = 0; i < raw; ++i) {
		ra[i] = mmhash(src_q, rawseed[i]) % len;
	}
	int i = 0;
	if (ra[0] == ra[1])
		i = 1;
	for (; i < raw; ++i) {
		for (int j = 0; j < len; ++j) {
			chain* temp = pointer_head[ra[i]][j];
			for (; temp != nullptr; temp = temp->pointer_chand) {
				if (temp->src == src_q) {
					successor[U] = temp->dst;
					++U;

				}
			}
		}
	}
	return U - cnt;
}

vector<Neighbor> Chandelier::GetNeighbors(key_type src) {
	vector<Neighbor> v;
	v.clear();
	unsigned int ra[raw];

	for (int i = 0; i < raw; ++i) {
		ra[i] = mmhash(src, rawseed[i]) % len;
	}
	int i = 0;
	if (ra[0] == ra[1])
		i = 1;
	for (; i < raw; ++i) {
		for (int j = 0; j < len; ++j) {
			chain* temp = pointer_head[ra[i]][j];
			for (; temp != nullptr; temp = temp->pointer_chand) {
				if (temp->src == src) {


					v.push_back({ temp->dst ,1 });

				}
			}
		}
	}
	return v;
}

int Chandelier::query_precursor(int dst, key_type precursor[], int U) {
	unsigned int co[col];
	int cnt = U;
	for (int i = 0; i < col; ++i) {
		co[i] = mmhash(dst, colseed[i]) % len;
	}
	int i = 0;
	if (co[0] == co[1])
		i = 1;
	for (; i < col; ++i) {
		for (int j = 0; j < len; ++j) {
			chain* temp = pointer_head[j][co[i]];
			for (; temp != nullptr; temp = temp->pointer_chand) {
				if (temp->dst == dst) {
					precursor[U] = temp->src;
					++U;

				}
			}
		}
	}
	return U - cnt;
}


int Chandelier::query_triangles(key_type src) {
	int cnt = 0;
	int sum1 = query_successor(src, successor, 0);
	int sum2 = query_precursor(src, precursor, 0);
	chain* tempC = new chain();
	for (int i = 0; i < sum1; ++i)
		for (int j = 0; j < sum2; ++j) {
			tempC->src = successor[i];
			tempC->dst = precursor[j];
			if ((successor[i] != src) && (precursor[j] != src) && (precursor[j] != successor[i])) {
				bool tempC_new = query_edge(tempC);
				if (tempC_new)++cnt;
			}
		}
	delete tempC;
	return cnt;
}

int Chandelier::query_triangle_new(key_type src) {
	chain* tempC = new chain();
	int cnt = 0;

	int sum = query_successor(src, successor, 0);

	tempC->dst = src;
	for (int i = 0; i < sum; ++i) {

		int sum2 = query_successor(successor[i], precursor, 0);
		for (int j = 0; j < sum2; ++j) {
			tempC->src = precursor[j];
			if ((precursor[j] != successor[i]) && (successor[i] != src) && (precursor[j] != src)) {
				if (query_edge(tempC))++cnt;
			}
		}
	}
	return cnt;
}

Chandelier* cs;
unordered_map<long long, int> realAns;
unordered_map<int, listChain*> crossList;
unordered_map<int, listChain*> crossList_rev;

listChain* query(listChain* list, listChain* edge) {
	listChain* it = list;
	for (; it != nullptr; it = it->pointer_suc) {
		if (it->dst == edge->dst)
			return it;
	}
	return nullptr;
}

void updateList(int dst, listChain* edge) {
	if (crossList_rev[dst] == nullptr) {
		crossList_rev[dst] = edge;
		return;
	}
	crossList_rev[dst]->pointer_pre_re = edge;
	edge->pointer_pre = crossList_rev[dst];
	crossList_rev[dst] = edge;
}

void insertList(int src, listChain* edge) {
	if (crossList[src] == nullptr) {
		crossList[src] = edge;
		updateList(edge->dst, edge);
		return;
	}
	listChain* temp = query(crossList[src], edge);
	if (temp == nullptr) {
		crossList[src]->pointer_suc_re = edge;
		edge->pointer_suc = crossList[src];
		crossList[src] = edge;
		updateList(edge->dst, edge);
	}



}

int asrc[63497059];
int adst[63497059];

int load_data13_CAIDA(int length = MAXNUM)
{
	ifstream input("/share/datasets/CAIDA2018/dataset/130000.dat", ios::in | ios::binary);
	char buf[200] = { 0 };
	int i;
	for (i = 0; i < length; ++i)
	{
		if (!input.read(buf, 21))
		{
			break;
		}
		asrc[i] = *(int*)buf;
		adst[i] = *(int*)(buf + 4);
	}
	return i;
}

int load_data20_CAIDA(int length = 100000000)
{
	ifstream input("20.dat", ios::in | ios::binary);
	char buf[200] = { 0 };
	int i;
	for (i = 0; i < length; ++i)
	{
		if (!input.read(buf, 26))
		{
			break;
		}
		asrc[i] = *(int*)buf;
		adst[i] = *(int*)(buf + 4);
	}
	return i;
}

int load_data1_EuAll(int length = 420045)
{
	ifstream input("Email-EuAll.txt", ios::in);
	int i;
	for (i = 0; i < length; ++i)
	{
		input >> asrc[i] >> adst[i];
	}
	return i;
}

long insert_cross(int len) {
	clock_t time_start = clock();
	for (int i = 0; i < len; ++i) {
		listChain* lc = new listChain();
		int s = asrc[i];
		int d = adst[i];
		lc->src = s;
		lc->dst = d;

		lc->pointer_suc = nullptr;
		lc->pointer_pre = nullptr;
		insertList(s, lc);
#ifdef Debugger
		if (i % 100000 == 0)
			printf("%d", i / 100000);
#endif
	}
	clock_t time_end = clock();
	long time = time_end - time_start;
	return time;
}



long insert_chand(int len) {
	clock_t time_start = clock();
	for (int i = 0; i < len; ++i) {
		chain* c = new chain();
		c->src = asrc[i];
		c->dst = adst[i];

		c->pointer_chand = nullptr;
		cs->insert(c);
#ifdef Debugger
		if (i % 100000 == 0)
			printf("%d", i / 100000);
#endif
	}
	clock_t time_end = clock();
	long time = time_end - time_start;
	return time;
}


int qsrc[20000000];
int qdst[20000000];
int wgh[3][1000000];









#endif