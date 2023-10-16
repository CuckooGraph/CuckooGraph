#include <chrono>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <set>
#include<queue>
#include<vector>
#include <fstream>
#include<time.h>
#include<algorithm>
#include"BobHash.h"
#include"Util.h"
#include"Cuckoo.h"
#include"WindBell.h"


#define lt 6
#define rate 0.8
#define u1 4
#define u2 2
#define tts (u1+u2)*bup
#define line tts*rate
#define brate 0.75
#define bl1 4
#define bl2 2
#define sure 9
#define blist 33
#define us1 0
#define us2 221
#define link_num 3
#define blink_num 3
#define minc1 4
#define minc2 2
#define cthres 0.35
#define bminc1 4
#define bminc2 2
#define bcthres 0.35
#define sbl 128
#define bbl 128

using namespace std::chrono;
using namespace std;

static uint64_t useful = 0xffffffff;
static int bstep = 0;
static int layernum = 0;
static int idth = 0;
static int bslotnum = 0;

TUPLES* read_data(const char* PATH, const count_type length,
	count_type* cnt) {
	TUPLES* items = new TUPLES[length];
	TUPLES* it = items;

	TIMESTAMP* timestamps = new TIMESTAMP[length];
	TIMESTAMP* timestamp = timestamps;

	FILE* data = fopen(PATH, "rb");
	*cnt = 0;
	while (fread(it++, sizeof(TUPLES), 1, data) > 0 && fread(timestamp++, sizeof(TIMESTAMP), 1, data) > 0) {
		(*cnt)++;
	}

	fclose(data);
	return items;
}

const string folder = "./";
const string filenames[] = { "130000.dat" };


Edge* read_wiki_data(count_type* cnt, const count_type length = 100000000) {
	Edge* items = new Edge[length];
	Edge* it = items;

	int intbig = 2147483647;

	ifstream infile;
	infile.open("./out.wiki_talk_en");
	int i = 0;
	char useless[50];
	infile.getline(useless, intbig);
	while (1) {


		char s[50];
		if (!infile.getline(s, intbig)) break;
		string src = "";
		string dst = "";
		int d = 0;

		for (int k = 0; k < 50;) {
			if (s[k] >= '0' && s[k] <= '9') {
				int r = k;
				while (s[r] >= '0' && s[r] <= '9') {
					string cc; cc = s[r];
					if (d == 0) { src.append(cc); ++r; }
					else if (d == 1) { dst.append(cc); ++r; }
				}
				k = r;
				if (d == 0)++d;
				else if (d == 1)k = 50;
			}
			else ++k;
		}

		items[i].src = stoi(src);
		items[i].dst = stoi(dst);
		asrc[i] = items[i].src;
		adst[i] = items[i].dst;
		++i;

	}
	(*cnt) = i;


	infile.close();
	return items;
}

Edge* read_stackoverflow_data(count_type* cnt, const count_type length = 100000000) {
	Edge* items = new Edge[length];
	Edge* it = items;

	int intbig = 2147483647;

	ifstream infile;
	infile.open("./sx-stackoverflow.txt");
	int i = 0;
	while (1) {


		char s[50];
		if (!infile.getline(s, intbig)) break;
		string src = "";
		string dst = "";
		int d = 0;

		for (int k = 0; k < 50;) {
			if (s[k] >= '0' && s[k] <= '9') {
				int r = k;
				while (s[r] >= '0' && s[r] <= '9') {
					string cc; cc = s[r];
					if (d == 0) { src.append(cc); ++r; }
					else if (d == 1) { dst.append(cc); ++r; }
				}
				k = r;
				if (d == 0)++d;
				else if (d == 1)k = 50;
			}
			else ++k;
		}

		items[i].src = stoi(src);
		items[i].dst = stoi(dst);
		asrc[i] = items[i].src;
		adst[i] = items[i].dst;
		++i;

	}
	(*cnt) = i;


	infile.close();
	return items;
}



Edge* read_NotreDame_data(count_type* cnt, const count_type length = 100000000) {
	Edge* items = new Edge[length];
	Edge* it = items;

	int intbig = 2147483647;

	ifstream infile;
	infile.open("./web-NotreDame.txt");
	int i = 0;
	char useless[50];
	infile.getline(useless, intbig);
	infile.getline(useless, intbig);
	infile.getline(useless, intbig);
	infile.getline(useless, intbig);
	while (1) {


		char s[50];
		if (!infile.getline(s, intbig)) break;
		string src = "";
		string dst = "";
		int d = 0;

		for (int k = 0; k < 50;) {
			if (s[k] >= '0' && s[k] <= '9') {
				int r = k;
				while (s[r] >= '0' && s[r] <= '9') {
					string cc; cc = s[r];
					if (d == 0) { src.append(cc); ++r; }
					else if (d == 1) { dst.append(cc); ++r; }
				}
				k = r;
				if (d == 0)++d;
				else if (d == 1)k = 50;
			}
			else ++k;
		}

		items[i].src = stoi(src);
		items[i].dst = stoi(dst);
		items[i].src += 1;
		items[i].dst += 1;
		asrc[i] = items[i].src;
		adst[i] = items[i].dst;
		++i;

	}
	(*cnt) = i;


	infile.close();
	return items;
}

void test(int uy1, int uy2) {
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	Cuckoo cuckoo(uy1, uy2, 0, 221);
	int sum = 0;
	for (int i = 0; i < cnt; ++i) {
		if (!(cuckoo.insert(edge[i].src)))
			++sum;
		if (sum == 2) {
			double total = (uy1 + uy2) * bup;
			double s = 0;
			for (int j = 0; j < uy1; ++j)
				for (int k = 0; k < bup; ++k)
				{
					if (!cuckoo.layer1.layer[j].bslot[k] == 0)
						s = s + 1;
				}
			for (int j = 0; j < uy2; ++j)
				for (int k = 0; k < bup; ++k) {
					if (!cuckoo.layer2.layer[j].bslot[k] == 0)
						s = s + 1;
				}
			double ans = s / total;
			cout << "Rate:　　" << ans << "  l1:l2　 :   " << uy1 << ":" << uy2 << endl;
			delete[] items;
			delete[] edge;
			return;
		}
	}
}


void test_mul(int uy1, int uy2) {
	int mul = 3;
	count_type cnt;
	Edge* edge = read_stackoverflow_data(&cnt, 100000000);
	double ans = 0;
	for (int M = 0; M < mul; ++M) {
		Cuckoo cuckoo(uy1, uy2, 0, 221);
		int sum = 0;
		for (int i = 0; i < cnt; ++i) {
			if (!(cuckoo.insert(edge[i].dst)))
				++sum;
			if (sum == 1) {
				double total = (uy1 + uy2) * bup;
				double s = 0;
				for (int j = 0; j < uy1; ++j)
					for (int k = 0; k < bup; ++k)
					{
						if (!cuckoo.layer1.layer[j].bslot[k] == 0)
							s = s + 1;
					}
				for (int j = 0; j < uy2; ++j)
					for (int k = 0; k < bup; ++k) {
						if (!cuckoo.layer2.layer[j].bslot[k] == 0)
							s = s + 1;
					}
				ans = ans + s / total;
				break;
			}
		}
		if (sum == 0) cout << "no kick out!" << endl;
	}
	ans = ans / mul;
	cout << "Rate:  " << ans << "  l1:l2  :  " << uy1 << ":" << uy2 << endl;
	delete[] edge;
}



static vector<Edge> sblacklist;

class bSlot {
public:
	Cuckoo* slot[3];
	key_type src;
	int pt;
	bSlot() {
		src = 0;
		pt = 0;
		memset(slot, 0, sizeof(Cuckoo*) * 3);

	}





#ifdef TEST_MEM
	int get_mem() {
		int mr = 0;
		mr += sizeof(bSlot);
		if (pt < 0) {
			for (int u = 0; u < 3; ++u)
				if (slot[u] != NULL) { mr += slot[u]->get_mem(); }
		}


		return mr;
	}
#endif


	double ret_num() {
		if (pt >= 0)  return ((double)pt);
		double num = 0;
		for (int i = 0; i < 3; ++i) {
			if (slot[i] != NULL) {
				num = num + slot[i]->num;
			}
		}

		return num;
	}

	double ret_deno() {

		if (pt >= 0)  return lt;
		double sum = lt;
		for (int i = 0; i < 3; ++i) {
			if (slot[i] != NULL)
				sum = sum + ((slot[i]->len1) + (slot[i]->len2)) * bup;
		}
		return sum;
	}

	double test_rate() {
		double sum = lt;
		double num = 0;
		if (pt >= 0)  return ((double)pt) / lt;

		for (int i = 0; i < 3; ++i) {
			if (slot[i] != NULL) {
				sum = sum + ((slot[i]->len1) + (slot[i]->len2)) * bup;
				num = num + slot[i]->num;
			}
		}
		return num / sum;
	}

	Cuckoo* merge(Cuckoo* c1, Cuckoo* c2) {
		int l1 = c1->len1;
		int l2 = c1->len2;
		queue<key_type> mq;
		Cuckoo* c = new Cuckoo(l1 * 2, l2 * 2, us1, us2);
		memcpy(c->layer1.layer, c1->layer1.layer, l1 * sizeof(Bucket));
		memcpy(c->layer1.layer + l1, c1->layer1.layer, l1 * sizeof(Bucket));
		memcpy(c->layer2.layer, c1->layer2.layer, l2 * sizeof(Bucket));
		memcpy(c->layer2.layer + l2, c1->layer2.layer, l2 * sizeof(Bucket));
		c->num = c1->num;
		for (int i = 0; i < l1; ++i) {
			int k = i + l1;
			for (int j = 0; j < bup; ++j) {
				if (c->layer1.layer[i].bslot[j] == 0)continue;
				if (i != (int)(mmhash(c->layer1.layer[i].bslot[j], us1) % (uint32_t)(l1 * 2)))
					c->layer1.layer[i].bslot[j] = 0;
				else c->layer1.layer[k].bslot[j] = 0;
			}
			for (int j = 0; j < bup; ++j) {
				if (c2->layer1.layer[i].bslot[j] == 0)continue;
				if ((mmhash(c2->layer1.layer[i].bslot[j], us1) % (uint32_t)(l1 * 2)) == i) {
					if ((c->layer1.layer[i].insert(c2->layer1.layer[i].bslot[j])) == 0)
						mq.push(c2->layer1.layer[i].bslot[j]);
					else
						c->num = (c->num) + 1;
				}
				else {
					if ((c->layer1.layer[k].insert(c2->layer1.layer[i].bslot[j])) == 0)
						mq.push(c2->layer1.layer[i].bslot[j]);
					else
						c->num = (c->num) + 1;
				}
			}
		}

		for (int i = 0; i < l2; ++i) {
			int k = i + l2;
			for (int j = 0; j < bup; ++j) {
				if (c->layer2.layer[i].bslot[j] == 0)continue;
				if (i != (int)(mmhash(c->layer2.layer[i].bslot[j], us2) % (uint32_t)(l2 * 2)))
					c->layer2.layer[i].bslot[j] = 0;
				else c->layer2.layer[k].bslot[j] = 0;
			}
			for (int j = 0; j < bup; ++j) {
				if (c2->layer2.layer[i].bslot[j] == 0)continue;
				if ((mmhash(c2->layer2.layer[i].bslot[j], us2) % (uint32_t)(l2 * 2)) == i) {
					if ((c->layer2.layer[i].insert(c2->layer2.layer[i].bslot[j])) == 0)
						mq.push(c2->layer2.layer[i].bslot[j]);
					else
						c->num = (c->num) + 1;
				}
				else {
					if ((c->layer2.layer[k].insert(c2->layer2.layer[i].bslot[j])) == 0)
						mq.push(c2->layer2.layer[i].bslot[j]);
					else
						c->num = (c->num) + 1;
				}
			}
		}

		while (!mq.empty()) {
			if (!c->insert(mq.front())) {
				Edge now_insert;
				now_insert.src = src;
				now_insert.dst = mykick;
				sblacklist.push_back(now_insert);
			}
			mq.pop();
		}
		delete c1;
		delete c2;
		return c;
	}


	int insert(Edge EE) {
		if (pt == 0) {
			src = EE.src;
			*((key_type*)slot) = EE.dst;
			++pt;
			return pt;
		}
		else if (src != EE.src)
			return 0;
		else if (pt < 0) {
			int query_result = query(EE);
			if (query_result) return query_result;
			int state = -1;

			Edge e;
			e.src = EE.src;
			e.dst = EE.dst;
			if (sblacklist.size() < sbl) {
				for (int i = 2; i >= 0; --i) {
					if (slot[i] != NULL) {
						++state;
						if ((slot[i]->num < (slot[i]->len1 + slot[i]->len2) * bup * rate)) {
							if (!slot[i]->insert(EE.dst))
							{
								Edge now_insert;
								now_insert.src = EE.src;
								now_insert.dst = mykick;
								sblacklist.push_back(now_insert);
							}
							return i + 9;
						}
					}
				}
			}
			else {
				bool fff = false;
				for (int i = 2; i >= 0; --i) {
					if (slot[i] != NULL) {
						++state;
						if ((slot[i]->num < (slot[i]->len1 + slot[i]->len2) * bup * rate) && (fff == false)
							) {
							if (!slot[i]->insert(EE.dst))
							{
								fff = true;
								e.src = EE.src;
								e.dst = mykick;

							}
							else
								return i + 9;
						}
					}
				}
			}

			if (state == 0) {
				slot[1] = new Cuckoo((slot[0]->len1) / 2, (slot[0]->len2) / 2, us1, us2);
#ifdef TEST_MEM
				cuckoo_mem += sizeof(Cuckoo) + ((slot[0]->len1) / 2 + (slot[0]->len2) / 2) * sizeof(Bucket);
#endif
				queue<key_type> qq;
				for (auto it = sblacklist.begin(); it != sblacklist.end();) {
					if ((*it).src == src) {
						if (!(slot[1]->insert((*it).dst)))qq.push(mykick);
						it = sblacklist.erase(it);
					}
					else ++it;
				}
				while (!qq.empty()) {
					Edge now_insert;
					now_insert.src = src;
					now_insert.dst = qq.front();
					sblacklist.push_back(now_insert);
					qq.pop();
				}


				if (!slot[1]->insert(e.dst))
				{
					Edge now_insert;
					now_insert.src = e.src;
					now_insert.dst = mykick;
					sblacklist.push_back(now_insert);
				}
				return 10;
			}


			if (state == 1) {
				slot[2] = new Cuckoo((slot[1]->len1), (slot[1]->len2), us1, us2);
#ifdef TEST_MEM
				cuckoo_mem += sizeof(Cuckoo) + ((slot[1]->len1) + (slot[1]->len2)) * sizeof(Bucket);
#endif
				queue<key_type> qq;
				for (auto it = sblacklist.begin(); it != sblacklist.end();) {
					if ((*it).src == src) {
						if (!(slot[2]->insert((*it).dst)))qq.push(mykick);
						it = sblacklist.erase(it);
					}
					else ++it;
				}
				while (!qq.empty()) {
					Edge now_insert;
					now_insert.src = src;
					now_insert.dst = qq.front();
					sblacklist.push_back(now_insert);
					qq.pop();
				}


				if (!slot[2]->insert(e.dst))
				{
					Edge now_insert;
					now_insert.src = e.src;
					now_insert.dst = mykick;
					sblacklist.push_back(now_insert);
				}
				return 11;
			}

			if (state == 2) {
				Cuckoo* cm = merge(slot[1], slot[2]);
				slot[1] = NULL;
				slot[2] = NULL;
				slot[0] = merge(slot[0], cm);
				slot[1] = new Cuckoo((slot[0]->len1) / 2, (slot[0]->len2) / 2, us1, us2);
#ifdef TEST_MEM
				cuckoo_mem += sizeof(Cuckoo) + ((slot[0]->len1) / 2 + (slot[0]->len2) / 2) * sizeof(Bucket);
#endif
				queue<key_type> qq;
				for (auto it = sblacklist.begin(); it != sblacklist.end();) {
					if ((*it).src == src) {
						if (!(slot[1]->insert((*it).dst)))qq.push(mykick);
						it = sblacklist.erase(it);
					}
					else ++it;
				}
				while (!qq.empty()) {
					Edge now_insert;
					now_insert.src = src;
					now_insert.dst = qq.front();
					sblacklist.push_back(now_insert);
					qq.pop();
				}


				if (!slot[1]->insert(e.dst))
				{
					Edge now_insert;
					now_insert.src = e.src;
					now_insert.dst = mykick;
					sblacklist.push_back(now_insert);
				}
				return 10;
			}


			if (state == -1) {
				slot[0] = new Cuckoo(u1, u2, us1, us2);
#ifdef TEST_MEM
				cuckoo_mem += sizeof(Cuckoo) + (u1 + u2) * sizeof(Bucket);
#endif
				if (!slot[0]->insert(e.dst))
				{
					Edge now_insert;
					now_insert.src = e.src;
					now_insert.dst = mykick;
					sblacklist.push_back(now_insert);
				}
				return 9;
			}
			assert(0);
		}
		else {
			if (pt == lt) {

				for (int i = 0; i < 3; ++i) {

					key_type* pv = (key_type*)(&slot[i]);
					if (*pv == EE.dst || *(pv + 1) == EE.dst)return pt;
				}

				Cuckoo* pv = slot[0];
				key_type* cp = (key_type*)(&pv);
				slot[0] = new Cuckoo(u1, u2, us1, us2);
#ifdef TEST_MEM
				cuckoo_mem += sizeof(Cuckoo) + (u1 + u2) * sizeof(Bucket);
#endif

				if (!(slot[0]->insert(*cp)))
				{
					Edge now_insert;
					now_insert.src = src;
					now_insert.dst = mykick;
					sblacklist.push_back(now_insert);
				}

				if (!slot[0]->insert(*(cp + 1)))
				{
					Edge now_insert;
					now_insert.src = src;
					now_insert.dst = mykick;
					sblacklist.push_back(now_insert);
				}
				pv = slot[1];
				if (!slot[0]->insert(*cp))
				{
					Edge now_insert;
					now_insert.src = src;
					now_insert.dst = mykick;
					sblacklist.push_back(now_insert);
				}
				if (!slot[0]->insert(*(cp + 1)))
				{
					Edge now_insert;
					now_insert.src = src;
					now_insert.dst = mykick;
					sblacklist.push_back(now_insert);
				}
				pv = slot[2];

				if (!slot[0]->insert(*cp))
				{
					Edge now_insert;
					now_insert.src = src;
					now_insert.dst = mykick;
					sblacklist.push_back(now_insert);
				}
				if (!slot[0]->insert(*(cp + 1)))
				{
					Edge now_insert;
					now_insert.src = src;
					now_insert.dst = mykick;
					sblacklist.push_back(now_insert);
				}
				slot[1] = NULL;
				slot[2] = NULL;

				if (!slot[0]->insert(EE.dst))
				{
					Edge now_insert;
					now_insert.src = EE.src;
					now_insert.dst = mykick;
					sblacklist.push_back(now_insert);
				}
				pt = -1;
				return pt;
			}


			else {
				for (int i = 0; i < 3; ++i) {
					key_type* pv = (key_type*)(&slot[i]);
					if (*pv == EE.dst || *(pv + 1) == EE.dst)return pt;
				}

				for (int i = 0; i < 3; ++i) {
					key_type* pv = (key_type*)(&slot[i]);
					if (*pv == 0) {
						*pv = EE.dst;
						++pt;
						return pt;
					}
					if (*(pv + 1) == 0) {
						*(pv + 1) = EE.dst;
						++pt;
						return pt;
					}
				}
				assert(0);
			}

		}
	}


	int query(Edge e) {
		if (e.src != src) {
			return 0;
		}
		if (pt < 0) {
			for (int i = 0; i < 3; ++i) {
				if (slot[i] != NULL && slot[i]->query(e.dst))return i + 9;
			}
			for (auto it = sblacklist.begin(); it != sblacklist.end(); ++it) {
				if ((*it).src == e.src && (*it).dst == e.dst) {
					return blist;
				}
			}
			return 0;
		}
		else {
			for (int i = 0; i < 3; ++i) {
				key_type* pv = (key_type*)(&slot[i]);
				if (*pv == e.dst || *(pv + 1) == e.dst)return pt;
			}
			return 0;
		}
	}

	int query_successor(key_type src, key_type successor[], int r) {
		int init = r;
		if (pt < 0) {
			for (int i = 0; i < 3; ++i) {
				if (slot[i] != NULL) {
					for (int j = 0; j < slot[i]->len1; ++j)
						for (int k = 0; k < bup; ++k) {
							if (slot[i]->layer1.layer[j].bslot[k] != 0) {
								successor[r] = slot[i]->layer1.layer[j].bslot[k];
								++r;
							}
						}
					for (int j = 0; j < slot[i]->len2; ++j)
						for (int k = 0; k < bup; ++k) {
							if (slot[i]->layer2.layer[j].bslot[k] != 0) {
								successor[r] = slot[i]->layer2.layer[j].bslot[k];
								++r;
							}
						}
				}
			}
			for (auto it = sblacklist.begin(); it != sblacklist.end(); ++it) {
				if ((*it).src == src) {
					successor[r] = (*it).dst;
					++r;
				}
			}
			return r - init;
		}
		if (pt == 0)return 0;

		for (int i = 0; i < 3; ++i) {
			key_type* pv = (key_type*)(&slot[i]);
			if (0 != *pv) {
				successor[r] = *pv;
				++r;
			}
			if (0 != *(pv + 1)) {
				successor[r] = *(pv + 1);
				++r;
			}
		}
		return r - init;
	}

	Cuckoo* compress(Cuckoo* c1) {
		int l1 = c1->len1;
		int l2 = c1->len2;
		int cl1 = l1 / 2;
		int cl2 = l2 / 2;
		queue<key_type> mq;
		int asum = c1->num;
		Cuckoo* c = new Cuckoo(cl1, cl2, us1, us2);

		memcpy(c->layer1.layer, c1->layer1.layer, cl1 * sizeof(Bucket));
		memcpy(c->layer2.layer, c1->layer2.layer, cl2 * sizeof(Bucket));

		for (int i = cl1; i < l1; ++i) {
			for (int j = 0; j < bup; ++j) {
				if (c1->layer1.layer[i].bslot[j] == 0)continue;
				if ((c->layer1.layer[i - cl1].insert(c1->layer1.layer[i].bslot[j])) == 0)
					mq.push(c1->layer1.layer[i].bslot[j]);
			}
		}

		for (int i = cl2; i < l2; ++i) {
			for (int j = 0; j < bup; ++j) {
				if (c1->layer2.layer[i].bslot[j] == 0)continue;
				if ((c->layer2.layer[i - cl2].insert(c1->layer2.layer[i].bslot[j])) == 0)
					mq.push(c1->layer2.layer[i].bslot[j]);
			}
		}

		while (!mq.empty()) {
			if (!c->insert(mq.front())) {
				Edge E;
				E.src = src;
				E.dst = mykick;
				sblacklist.push_back(E);
				--asum;
			}
			mq.pop();
		}

		c->num = asum;

		delete c1;
		return c;
	}

	int del(Edge e) {
		if (src != e.src)return 0;
		if (pt == 0) return 2;

		if (pt > 0) {
			for (int i = 0; i < 3; ++i) {
				key_type* pv = (key_type*)(&slot[i]);
				if (*pv == e.dst) {
					*pv = 0;
					--pt;
					if (pt == 0) {
						src = 0;
						return -1;
					}
					else return 1;
				}
				if (*(pv + 1) == e.dst) {
					*(pv + 1) = 0;
					--pt;
					if (pt == 0) {
						src = 0;
						return -1;
					}
					else return 1;
				}
			}
			return 2;
		}

		for (int II = 0; II < 3; ++II) {
			if (slot[II] != NULL && slot[II]->del(e.dst)) {
				int state = -1;
				double num = 0;
				double sum = 0;

				for (int i = 0; i < 3; ++i) {
					if (NULL != slot[i]) {
						++state;
						num = num + slot[i]->num;
						sum = sum + (slot[i]->len1 + slot[i]->len2) * bup;
					}
				}

				double nr = num / sum;
				if (state == 0) {
					if (slot[0]->num == 0) {
						delete slot[0];
						for (auto it = sblacklist.begin(); it != sblacklist.end(); ++it) {
							if ((*it).src == e.src) {
								slot[0] = NULL;
								return 1;
							}
						}
						src = 0;
						pt = 0;
						memset(slot, 0, sizeof(Cuckoo*) * 3);
						return -1;
					}
					if ((slot[0]->len2 >= minc2) && nr < cthres) {
						slot[0] = compress(slot[0]);
						return 1;
					}
					return 1;
				}

				else if (state == 1) {
					if (slot[1]->num == 0) {
						delete slot[1];
						slot[1] = NULL;
						return 1;
					}

					if (nr < cthres) {

						for (int i = 0; i < slot[1]->len1; ++i) {
							for (int j = 0; j < bup; ++j) {
								if (slot[1]->layer1.layer[i].bslot[j] == 0)continue;
								if ((slot[0]->insert(slot[1]->layer1.layer[i].bslot[j])) == false)
								{
									Edge E;
									E.src = src;
									E.dst = mykick;
									sblacklist.push_back(E);
								}
							}
						}

						for (int i = 0; i < slot[1]->len2; ++i) {
							for (int j = 0; j < bup; ++j) {
								if (slot[1]->layer2.layer[i].bslot[j] == 0)continue;
								if ((slot[0]->insert(slot[1]->layer2.layer[i].bslot[j])) == false)
								{
									Edge E;
									E.src = src;
									E.dst = mykick;
									sblacklist.push_back(E);
								}
							}
						}

						delete slot[1];
						slot[1] = NULL;
						return 1;
					}
					return 1;
				}

				else if (state == 2) {
					int bi, sm;
					if (slot[2]->num > slot[1]->num) {
						sm = 1;
						bi = 2;
					}
					else {
						sm = 2;
						bi = 1;
					}

					if (slot[sm]->num == 0) {
						delete slot[sm];
						slot[1] = slot[bi];
						slot[2] = NULL;
						return 1;
					}

					if (nr < cthres) {
						for (int i = 0; i < slot[sm]->len1; ++i) {
							for (int j = 0; j < bup; ++j) {
								if (slot[sm]->layer1.layer[i].bslot[j] == 0)continue;

								if (slot[0]->num < (slot[0]->len1 + slot[0]->len2) * bup * rate)
								{
									if ((!(slot[0]->insert(slot[sm]->layer1.layer[i].bslot[j]))) && (!(slot[bi]->insert(slot[sm]->layer1.layer[i].bslot[j]))))
									{
										Edge E;
										E.src = src;
										E.dst = mykick;
										sblacklist.push_back(E);
									}
								}

								else {
									if ((!(slot[bi]->insert(slot[sm]->layer1.layer[i].bslot[j]))))
									{
										Edge E;
										E.src = src;
										E.dst = mykick;
										sblacklist.push_back(E);
									}
								}
							}
						}

						for (int i = 0; i < slot[sm]->len2; ++i) {
							for (int j = 0; j < bup; ++j) {
								if (slot[sm]->layer2.layer[i].bslot[j] == 0)continue;

								if (slot[0]->num < (slot[0]->len1 + slot[0]->len2) * bup * rate)
								{
									if ((!(slot[0]->insert(slot[sm]->layer2.layer[i].bslot[j]))) && (!(slot[bi]->insert(slot[sm]->layer2.layer[i].bslot[j]))))
									{
										Edge E;
										E.src = src;
										E.dst = mykick;
										sblacklist.push_back(E);
									}
								}

								else {
									if ((!(slot[bi]->insert(slot[sm]->layer2.layer[i].bslot[j]))))
									{
										Edge E;
										E.src = src;
										E.dst = mykick;
										sblacklist.push_back(E);
									}
								}
							}
						}

						delete slot[sm];
						slot[1] = slot[bi];
						slot[2] = NULL;
						return 1;
					}
					return 1;
				}
			}
		}

		bool flag = false;
		bool f2 = false;
		for (auto it = sblacklist.begin(); it != sblacklist.end();) {
			if ((*it).src == e.src) {
				if ((*it).dst == e.dst) {
					it = sblacklist.erase(it);
					f2 = true;
				}
				else {
					flag = true;
					++it;
				}
			}
			else ++it;
		}
		if (f2) {
			if (flag)return 1;
			else {
				if (slot[0] == NULL && slot[1] == NULL && slot[2] == NULL)return -1;
				else return 1;
			}
		}
		return 2;
	}

	bool isSlotEmpty() {
		return pt == 0;
	}

};


static bSlot holekick;

class bBucket {
public:
	bSlot bslot[bup];
	int insert(Edge e) {
		for (int i = 0; i < bup; ++i)
			if (bslot[i].src == e.src) {
				bslot[i].insert(e);
				return -1;
			}
		for (int i = 0; i < bup; ++i)
			if (bslot[i].insert(e)) {
				return 1;
			}
		return 0;
	}
	bool insert(bSlot e) {
		for (int i = 0; i < bup; ++i)
			if (bslot[i].isSlotEmpty()) {
				bslot[i] = e;
				return true;
			}
		return false;
	}


	int query(Edge e) {
		for (int i = 0; i < bup; ++i) {
			if (bslot[i].src == e.src) {
				return bslot[i].query(e);
			}
		}
		return 0;
	}

	int query3(Edge e) {
		for (int i = 0; i < bup; ++i) {
			if (bslot[i].src == e.src) {
				if (bslot[i].query(e))
					return sure;
				bslotnum = i;
				return 1;
			}
		}
		return 0;
	}

	int del(Edge e) {
		for (int i = 0; i < bup; ++i) {
			if (bslot[i].src == e.src) {
				return bslot[i].del(e);
			}
		}
		return 0;
	}
};

class bLayer {
public:
	bBucket* layer;
	int length;
	int seed;
	bLayer() {}
	bLayer(int num, int s) {
		layer = new bBucket[num];
		length = num;
		seed = s;
	}
	~bLayer() {
		delete[] layer;

	}
	int query(Edge e) {
		return layer[(int)(mmhash(e.src, seed) % (uint32_t)length)].query(e);
	}
	int insert(Edge e) {
		return layer[(int)(mmhash(e.src, seed) % (uint32_t)length)].insert(e);
	}
	bool insert(bSlot e) {
		return layer[(int)(mmhash(e.src, seed) % (uint32_t)length)].insert(e);
	}

	int del(Edge e) {
		return layer[(int)(mmhash(e.src, seed) % (uint32_t)length)].del(e);
	}
};

class bCuckoo {
public:
	int len1;
	int len2;
	int seed1, seed2;
	int num;
	bLayer layer1;
	bLayer layer2;
	bCuckoo(int l1, int l2, int s1, int s2) :len1(l1), len2(l2), seed1(s1), seed2(s2), num(0), layer1(l1, s1), layer2(l2, s2) {
	}


	bool insert(Edge item) {
		int ir = layer1.insert(item);
		if (ir) {
			if (ir == 1)
				++num;
			return true;
		}
		ir = layer2.insert(item);
		if (ir) {
			if (ir == 1)
				++num;
			return true;
		}
		int times = 500;
		bool flag = true;
		int pos1 = (int)(mmhash(item.src, seed1) % (uint32_t)len1);
		int pos2 = (int)(mmhash(item.src, seed2) % (uint32_t)len2);
		bSlot kick;
		bSlot ins;

		ins.insert(item);
		while (times) {
			int xx = rand();
			if (flag) {
				kick = layer1.layer[pos1].bslot[xx % bup];
				layer1.layer[pos1].bslot[xx % bup] = ins;
				if (layer2.insert(kick)) {
					++num;
					return true;
				}
				--times;
				flag = false;
				ins = kick;
				pos2 = (int)(mmhash(ins.src, seed2) % (uint32_t)len2);
			}
			else {
				kick = layer2.layer[pos2].bslot[xx % bup];
				layer2.layer[pos2].bslot[xx % bup] = ins;
				if (layer1.insert(kick)) {
					++num;
					return true;
				}
				--times;
				flag = true;
				ins = kick;
				pos1 = (int)(mmhash(ins.src, seed1) % (uint32_t)len1);
			}
		}
		holekick = ins;
		return false;
	}

	bool insert(bSlot item) {
		if (layer1.insert(item)) {
			++num;
			return true;
		}
		if (layer2.insert(item)) {
			++num;
			return true;
		}
		int times = 500;
		bool flag = true;
		int pos1 = (int)(mmhash(item.src, seed1) % (uint32_t)len1);
		int pos2 = (int)(mmhash(item.src, seed2) % (uint32_t)len2);
		bSlot kick = item;
		bSlot ins = item;
		while (times) {
			int xx = rand();
			if (flag) {
				kick = layer1.layer[pos1].bslot[xx % bup];
				layer1.layer[pos1].bslot[xx % bup] = ins;
				if (layer2.insert(kick)) {
					++num;
					return true;
				}
				--times;
				flag = false;
				ins = kick;
				pos2 = (int)(mmhash(ins.src, seed2) % (uint32_t)len2);
			}
			else {
				kick = layer2.layer[pos2].bslot[xx % bup];
				layer2.layer[pos2].bslot[xx % bup] = ins;
				if (layer1.insert(kick)) {
					++num;
					return true;
				}
				--times;
				flag = true;
				ins = kick;
				pos1 = (int)(mmhash(ins.src, seed1) % (uint32_t)len1);
			}
		}
		holekick = ins;
		return false;
	}

	int query(Edge item) {
		int ans = layer1.query(item);
		if (ans)return ans;
		return layer2.query(item);
	}


	int query3(Edge item) {
		int ha = (int)(mmhash(item.src, seed1) % (uint32_t)len1);
		int ans = layer1.layer[ha].query3(item);
		if (ans == sure)return sure;
		if (ans) { layernum = 1; idth = ha; return 1; }
		ha = (int)(mmhash(item.src, seed2) % (uint32_t)len2);
		ans = layer2.layer[ha].query3(item);
		if (ans == sure)return sure;
		if (ans) { layernum = 2; idth = ha; return 2; }
		return 0;
	}

	int del(Edge item) {
		int ans = layer1.del(item);
		if (ans != 0) {
			if (ans == 2)return 2;
			if (ans < 0) {
				--num;
				return -1;
			}
			else return 1;
		}
		ans = layer2.del(item);
		if (ans != 0) {
			if (ans == 2)return 2;
			if (ans < 0) {
				--num;
				return -1;
			}
			else return 1;
		}
		else
			return 0;
	}
};

class holemerge {
public:
	int state;

	int ll1, ll2;
	int s1, s2;
	bCuckoo* link[3];
	vector<bSlot> blacklist;

	double test_rate() {
		double num = 0;
		double deno = 0;
		for (int T = 0; T < 3; ++T) {
			if (link[T] != NULL) {
				for (int j = 0; j < link[T]->len1; ++j)
					for (int k = 0; k < bup; ++k)
					{
						num += link[T]->layer1.layer[j].bslot[k].ret_num();
						deno += link[T]->layer1.layer[j].bslot[k].ret_deno();
					}

				for (int j = 0; j < link[T]->len2; ++j)
					for (int k = 0; k < bup; ++k)
					{
						num += link[T]->layer2.layer[j].bslot[k].ret_num();
						deno += link[T]->layer2.layer[j].bslot[k].ret_deno();
					}

			}
		}
		return num / deno;
	}

	holemerge(int _l1, int _l2, int _s1, int _s2) {
		state = -1;
		link[0] = NULL;
		link[1] = NULL;
		link[2] = NULL;
		ll1 = _l1;
		ll2 = _l2;
		s1 = _s1;
		s2 = _s2;
		sblacklist.clear();


	}

	~holemerge() {


		bCuckoo* ptr;
		for (int II = 0; II < 3; ++II) {
			if (NULL == link[II])continue;
			ptr = link[II];
			link[II] = NULL;
			int len1 = ptr->len1;
			int len2 = ptr->len2;
			for (int i = 0; i < len1; ++i)
				for (int j = 0; j < bup; ++j) {
					if (ptr->layer1.layer[i].bslot[j].pt == -1) {
						for (int ij = 0; ij < 3; ++ij) {
							if (ptr->layer1.layer[i].bslot[j].slot[ij] != NULL)
							{
								delete ptr->layer1.layer[i].bslot[j].slot[ij];
								ptr->layer1.layer[i].bslot[j].slot[ij] = NULL;
							}
						}
					}
				}


			for (int i = 0; i < len2; ++i)
				for (int j = 0; j < bup; ++j) {
					if (ptr->layer2.layer[i].bslot[j].pt == -1) {
						for (int ij = 0; ij < 3; ++ij) {
							if (ptr->layer2.layer[i].bslot[j].slot[ij] != NULL) {
								delete ptr->layer2.layer[i].bslot[j].slot[ij];
								ptr->layer2.layer[i].bslot[j].slot[ij] = NULL;
							}
						}
					}
				}

			delete ptr;
		}


		for (auto it = blacklist.begin(); it != blacklist.end(); ++it) {
			if ((*it).pt == -1) {
				for (int ij = 0; ij < 3; ++ij) {
					if ((*it).slot[ij] != NULL)
						delete (*it).slot[ij];
				}
			}
		}
		blacklist.clear();
	}

	bCuckoo* merge(bCuckoo* c1, bCuckoo* c2) {
		int l1 = c1->len1;
		int l2 = c1->len2;
		queue<bSlot> mq;
		bCuckoo* c = new bCuckoo(l1 * 2, l2 * 2, s1, s2);
		memcpy(c->layer1.layer, c1->layer1.layer, l1 * sizeof(bBucket));
		memcpy(c->layer1.layer + l1, c1->layer1.layer, l1 * sizeof(bBucket));
		memcpy(c->layer2.layer, c1->layer2.layer, l2 * sizeof(bBucket));
		memcpy(c->layer2.layer + l2, c1->layer2.layer, l2 * sizeof(bBucket));
		c->num = c1->num;
		for (int i = 0; i < l1; ++i) {
			int k = i + l1;
			for (int j = 0; j < bup; ++j) {
				if (c->layer1.layer[i].bslot[j].src == 0)continue;
				if (i != (int)(mmhash(c->layer1.layer[i].bslot[j].src, s1) % (uint32_t)(l1 * 2))) {
					c->layer1.layer[i].bslot[j].src = 0;
					c->layer1.layer[i].bslot[j].pt = 0;
					memset(c->layer1.layer[i].bslot[j].slot, 0, sizeof(Cuckoo*) * 3);
				}
				else {
					c->layer1.layer[k].bslot[j].src = 0;
					c->layer1.layer[k].bslot[j].pt = 0;
					memset(c->layer1.layer[k].bslot[j].slot, 0, sizeof(Cuckoo*) * 3);
				}
			}
			for (int j = 0; j < bup; ++j) {
				if (c2->layer1.layer[i].bslot[j].src == 0)continue;
				if ((mmhash(c2->layer1.layer[i].bslot[j].src, s1) % (uint32_t)(l1 * 2)) == i) {
					if ((c->layer1.layer[i].insert(c2->layer1.layer[i].bslot[j])) == false)mq.push(c2->layer1.layer[i].bslot[j]);
					else c->num = (c->num) + 1;
				}
				else {
					if ((c->layer1.layer[k].insert(c2->layer1.layer[i].bslot[j])) == false)mq.push(c2->layer1.layer[i].bslot[j]);
					else c->num = (c->num) + 1;
				}
			}
		}
		for (int i = 0; i < l2; ++i) {
			int k = i + l2;
			for (int j = 0; j < bup; ++j) {
				if (c->layer2.layer[i].bslot[j].src == 0)continue;
				if (i != (int)(mmhash(c->layer2.layer[i].bslot[j].src, s2) % (uint32_t)(l2 * 2))) {
					c->layer2.layer[i].bslot[j].src = 0;
					c->layer2.layer[i].bslot[j].pt = 0;
					memset(c->layer2.layer[i].bslot[j].slot, 0, sizeof(Cuckoo*) * 3);
				}
				else {
					c->layer2.layer[k].bslot[j].src = 0;
					c->layer2.layer[k].bslot[j].pt = 0;
					memset(c->layer2.layer[k].bslot[j].slot, 0, sizeof(Cuckoo*) * 3);
				}
			}
			for (int j = 0; j < bup; ++j) {
				if (c2->layer2.layer[i].bslot[j].src == 0)continue;
				if ((mmhash(c2->layer2.layer[i].bslot[j].src, s2) % (uint32_t)(l2 * 2)) == i) {
					if ((c->layer2.layer[i].insert(c2->layer2.layer[i].bslot[j])) == false)mq.push(c2->layer2.layer[i].bslot[j]);
					else c->num = (c->num) + 1;
				}
				else {
					if ((c->layer2.layer[k].insert(c2->layer2.layer[i].bslot[j])) == false)mq.push(c2->layer2.layer[i].bslot[j]);
					else c->num = (c->num) + 1;
				}
			}
		}

		while (!mq.empty()) {
			if (!c->insert(mq.front())) {
				blacklist.push_back(holekick);
			}
			mq.pop();
		}
		delete c1;
		delete c2;
		return c;
	}

	int query_successor(key_type src, key_type successor[], int U) {
		for (int i = 0; i <= state; ++i) {
			key_type h1 = mmhash(src, link[i]->seed1) % ((uint32_t)(link[i]->layer1.length));
			key_type h2 = mmhash(src, link[i]->seed2) % ((uint32_t)(link[i]->layer2.length));
			for (int j = 0; j < bup; ++j) {
				if (src == link[i]->layer1.layer[h1].bslot[j].src) {
					return link[i]->layer1.layer[h1].bslot[j].query_successor(src, successor, U);
				}
				else if (src == link[i]->layer2.layer[h2].bslot[j].src) {
					return link[i]->layer2.layer[h2].bslot[j].query_successor(src, successor, U);
				}
			}
		}

		for (auto it = blacklist.begin(); it != blacklist.end(); ++it) {
			if ((*it).src == src) {
				return (*it).query_successor(src, successor, U);
			}
		}
		return 0;
	}

	int query_triangles(key_type src) {
		int cnt = 0;
		int sum = query_successor(src, successor1, 0);
		Edge e;
		e.dst = src;
		for (int i = 0; i < sum; ++i) {
			int sum2 = query_successor(successor1[i], precursor1, 0);
			for (int j = 0; j < sum2; ++j) {
				e.src = precursor1[j];
				if (query(e))++cnt;
			}
		}
		return cnt;
	}

	bool insert(Edge e) {

		int ans = query3(e);
		if (ans == sure) return true;
		if (ans) {
			if (ans == blist) { blacklist[idth].insert(e); return true; }
			bCuckoo* ptr = link[ans - 1];
			if (layernum == 1) {
				ptr->layer1.layer[idth].bslot[bslotnum].insert(e);
				return true;
			}
			else if (layernum == 2) {
				ptr->layer2.layer[idth].bslot[bslotnum].insert(e);
				return true;
			}
		}


		if (state == -1) {
			link[0] = new bCuckoo(ll1, ll2, s1, s2);
#ifdef TEST_MEM
			cuckoo_mem += sizeof(bCuckoo) + (ll1 + ll2) * sizeof(bBucket);
#endif
			state = 0;
			if (!((*link[0]).insert(e))) {
				blacklist.push_back(holekick);


			}
			return true;
		}

		if (state == 0) {
			if ((link[0]->num >= (link[0]->len1 + link[0]->len2) * bup * rate) || (blacklist.size() >= bbl)) {
				link[1] = new bCuckoo(link[0]->len1 / 2, link[0]->len2 / 2, s1, s2);
#ifdef TEST_MEM
				cuckoo_mem += sizeof(bCuckoo) + (link[0]->len1 / 2 + link[0]->len2 / 2) * sizeof(bBucket);
#endif
				state = 1;
				queue<bSlot> qq;
				for (auto it = blacklist.begin(); it != blacklist.end(); ++it) {
					if (!(link[1]->insert(*it)))qq.push(holekick);


				}
				blacklist.clear();
				while (!qq.empty()) {
					blacklist.push_back(qq.front());
					qq.pop();
				}
				if (!((*link[1]).insert(e))) {
					blacklist.push_back(holekick);




				}
				return true;
			}
			else {
				if (!((*link[0]).insert(e))) {
					blacklist.push_back(holekick);


				}
				return true;
			}

		}
		if (state == 1) {
			for (int i = state; i >= 0; --i) {
				if ((link[i]->num < (link[i]->len1 + link[i]->len2) * bup * rate) && (blacklist.size() < bbl))
				{
					if (!((*link[i]).insert(e))) {
						blacklist.push_back(holekick);
					}
					return true;
				}
			}


			link[2] = new bCuckoo(link[1]->len1, link[1]->len2, s1, s2);
#ifdef TEST_MEM
			cuckoo_mem += sizeof(bCuckoo) + (link[1]->len1 + link[1]->len2) * sizeof(bBucket);
#endif
			state = 2;
			queue<bSlot> qq;
			for (auto it = blacklist.begin(); it != blacklist.end(); ++it) {
				if (!(link[2]->insert(*it)))qq.push(holekick);
			}
			blacklist.clear();
			while (!qq.empty()) {
				blacklist.push_back(qq.front());
				qq.pop();
			}

			if (!((*link[2]).insert(e))) {
				blacklist.push_back(holekick);

			}
			return true;
		}
		if (state == 2) {
			for (int i = state; i >= 0; --i) {
				if ((link[i]->num < (link[i]->len1 + link[i]->len2) * bup * rate) && (blacklist.size() < bbl))
				{
					if (!((*link[i]).insert(e))) {
						blacklist.push_back(holekick);

						;

					}
					return true;
				}
			}

			bCuckoo* cm = merge(link[1], link[2]);
			link[0] = merge(link[0], cm);
			link[1] = new bCuckoo(link[0]->len1 / 2, link[0]->len2 / 2, s1, s2);
#ifdef TEST_MEM
			cuckoo_mem += sizeof(bCuckoo) + (link[0]->len1 / 2 + link[0]->len2 / 2) * sizeof(bBucket);
#endif
			link[2] = NULL;            state = 1;
			queue<bSlot> qq;
			for (auto it = blacklist.begin(); it != blacklist.end(); ++it) {
				if (!(link[1]->insert(*it)))qq.push(holekick);
			}
			blacklist.clear();
			while (!qq.empty()) {
				blacklist.push_back(qq.front());
				qq.pop();
			}
			if (!((*link[1]).insert(e))) {
				blacklist.push_back(holekick);

			}
			return true;
		}
		cout << "BUG!" << endl;
		return false;
	}

	int query(Edge e) {
		for (int i = 0; i <= state; ++i) {
			int ans = link[i]->query(e);
			if (ans)return ans;
		}

		for (auto it = blacklist.begin(); it != blacklist.end(); ++it) {
			if ((*it).src == e.src) {
				int ans = (*it).query(e);
				if (ans)
					return ans;
				return 0;
			}
		}
		return 0;
	}





	int query3(Edge e) {
		for (int i = 0; i <= state; ++i) {
			int ans = link[i]->query3(e);
			if (ans == sure)return sure;
			if (ans)return i + 1;
		}
		int j = 0;
		for (auto it = blacklist.begin(); it != blacklist.end(); ++it) {
			if ((*it).src == e.src) {
				if ((*it).query(e)) {
					return sure;
				}
				idth = j;
				return blist;
			}
			++j;
		}

		return 0;
	}


	bCuckoo* compress(bCuckoo* c1) {
		int l1 = c1->len1;
		int l2 = c1->len2;
		int cl1 = l1 / 2;
		int cl2 = l2 / 2;
		queue<bSlot> mq;
		int asum = c1->num;
		bCuckoo* c = new bCuckoo(cl1, cl2, s1, s2);

		memcpy(c->layer1.layer, c1->layer1.layer, cl1 * sizeof(bBucket));
		memcpy(c->layer2.layer, c1->layer2.layer, cl2 * sizeof(bBucket));

		for (int i = cl1; i < l1; ++i) {
			for (int j = 0; j < bup; ++j) {
				if (c1->layer1.layer[i].bslot[j].src == 0)continue;
				bool flag = false;
				for (int m = 0; m < bup; ++m) {
					if (c->layer1.layer[i - cl1].bslot[m].src == 0) {
						c->layer1.layer[i - cl1].bslot[m] = c1->layer1.layer[i].bslot[j];
						flag = true;
						break;
					}

				}
				if (flag == false)mq.push(c1->layer1.layer[i].bslot[j]);
			}
		}

		for (int i = cl2; i < l2; ++i) {
			for (int j = 0; j < bup; ++j) {
				if (c1->layer2.layer[i].bslot[j].src == 0)continue;
				bool flag = false;
				for (int m = 0; m < bup; ++m) {
					if (c->layer2.layer[i - cl2].bslot[m].src == 0) {
						c->layer2.layer[i - cl2].bslot[m] = c1->layer2.layer[i].bslot[j];
						flag = true;
						break;
					}

				}
				if (flag == false)mq.push(c1->layer2.layer[i].bslot[j]);
			}
		}


		while (!mq.empty()) {
			if (!c->insert(mq.front())) {
				blacklist.push_back(holekick);
				--asum;
			}
			mq.pop();
		}
		c->num = asum;
		delete c1;
		return c;
	}


	int del(Edge e) {
		for (int II = 0; II <= state; ++II) {
			int ans = link[II]->del(e);
			if (ans) {
				if (ans == 2 || ans == 1)return ans;
				double num = 0;
				double sum = 0;

				for (int i = 0; i <= state; ++i) {
					num = num + link[i]->num;
					sum = sum + (link[i]->len1 + link[i]->len2) * bup;
				}

				double nr = num / sum;
				if (state == 0) {
					if (link[0]->num == 0) {
						delete link[0];
						link[0] = NULL;
						state = -1;
						return ans;
					}
					if ((link[0]->len2 >= bminc2) && nr < bcthres) {
						link[0] = compress(link[0]);
						return ans;
					}
					return ans;
				}

				else if (state == 1) {
					if (link[1]->num == 0) {
						delete link[1];
						link[1] = NULL;
						state = 0;
						return ans;
					}
					if (nr < bcthres) {
						for (int i = 0; i < link[1]->len1; ++i) {
							for (int j = 0; j < bup; ++j) {
								if (link[1]->layer1.layer[i].bslot[j].isSlotEmpty())continue;
								if ((link[0]->insert(link[1]->layer1.layer[i].bslot[j])) == false)
									blacklist.push_back(holekick);
							}
						}

						for (int i = 0; i < link[1]->len2; ++i) {
							for (int j = 0; j < bup; ++j) {
								if (link[1]->layer2.layer[i].bslot[j].isSlotEmpty())continue;
								if ((link[0]->insert(link[1]->layer2.layer[i].bslot[j])) == false)
									blacklist.push_back(holekick);
							}
						}

						delete link[1];
						link[1] = NULL;
						state = 0;
						return ans;
					}
					return ans;
				}

				else if (state == 2) {
					int bi, sm;
					if (link[2]->num > link[1]->num) {
						sm = 1;
						bi = 2;
					}
					else {
						sm = 2;
						bi = 1;
					}

					if (link[sm]->num == 0) {
						delete link[sm];
						link[1] = link[bi];
						link[2] = NULL;
						state = 1;
						return ans;
					}
					if (nr < cthres) {
						for (int i = 0; i < link[sm]->len1; ++i) {
							for (int j = 0; j < bup; ++j) {
								if (link[sm]->layer1.layer[i].bslot[j].isSlotEmpty())continue;

								if (link[0]->num < (link[0]->len1 + link[0]->len2) * bup * rate)
								{
									if ((!(link[0]->insert(link[sm]->layer1.layer[i].bslot[j]))) && (!(link[bi]->insert(link[sm]->layer1.layer[i].bslot[j]))))
										blacklist.push_back(holekick);
								}

								else {
									if ((!(link[bi]->insert(link[sm]->layer1.layer[i].bslot[j]))))
										blacklist.push_back(holekick);
								}
							}
						}

						for (int i = 0; i < link[sm]->len2; ++i) {
							for (int j = 0; j < bup; ++j) {
								if (link[sm]->layer2.layer[i].bslot[j].isSlotEmpty())continue;

								if (link[0]->num < (link[0]->len1 + link[0]->len2) * bup * rate)
								{
									if ((!(link[0]->insert(link[sm]->layer2.layer[i].bslot[j]))) && (!(link[bi]->insert(link[sm]->layer2.layer[i].bslot[j]))))
										blacklist.push_back(holekick);
								}

								else {
									if ((!(link[bi]->insert(link[sm]->layer2.layer[i].bslot[j]))))
										blacklist.push_back(holekick);
								}
							}
						}

						delete link[sm];
						link[1] = link[bi];
						link[2] = NULL;
						state = 1;
						return ans;
					}
					return ans;
				}
			}
		}
		for (auto it = blacklist.begin(); it != blacklist.end(); ++it) {
			if ((*it).src == e.src) {
				int ans = (*it).del(e);
				if (ans == 2)return 2;
				if (ans > 0)return 1;
				if (ans < 0) {
					blacklist.erase(it);
					return -1;
				}
			}
		}
		return 0;
	}

};



void test_err() {
	sblacklist.clear();
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	set<Edge, EdgeLess> ed;
	ed.insert(edge, edge + cnt);
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	int l2 = edsize / (3 * bup * brate);
	int l1 = l2 * 2;
	bCuckoo hole(l1, l2, 0, 221);
	asum = 0;
	int ernum = 0;
	int ernum2 = 0;
	set<Edge, EdgeLess>::iterator it;
	for (it = ed.begin(); it != ed.end(); it++) {
		hole.insert(*it);
		if (!hole.query(*it))
			++ernum;
	}
	cout << "error num:  " << ernum << endl;
	for (it = ed.begin(); it != ed.end(); it++) {
		if (!hole.query(*it))
			++ernum2;
	}
	cout << "sum:  " << asum << endl;
	cout << "error num 2 :   " << ernum2 << endl;
	delete[] items;
	delete[] edge;
}

void test_th() {
	sblacklist.clear();
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	int l2 = cnt / (3 * bup * brate);
	int l1 = l2 * 2;
	bCuckoo hole(l1, l2, 0, 221);
	auto t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	auto t2 = steady_clock::now();
	auto t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Insert throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		hole.query(edge[i]);
	}
	t2 = steady_clock::now();
	t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Query throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	delete[] items;
	delete[] edge;
}


void test_hole_err() {
	sblacklist.clear();
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	set<Edge, EdgeLess> ed;
	ed.insert(edge, edge + cnt);
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	holemerge hole(bl1, bl2, 0, 221);
	asum = 0;
	int ernum = 0;
	int ernum2 = 0;
	set<Edge, EdgeLess>::iterator it;
	for (it = ed.begin(); it != ed.end(); it++) {
		hole.insert(*it);
		if (!hole.query(*it))
			++ernum;
	}
	cout << "error num:  " << ernum << endl;
	for (it = ed.begin(); it != ed.end(); it++) {
		if (!hole.query(*it))
			++ernum2;
	}
	cout << "sum:  " << asum << endl;
	cout << "error num 2 :   " << ernum2 << endl;
	delete[] items;
	delete[] edge;
}

void test_hole_err2() {
	sblacklist.clear();
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	holemerge hole(bl1, bl2, 0, 221);
	asum = 0;
	int ernum = 0;
	int ernum2 = 0;
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	cout << "begin query!" << endl;
	for (int i = 0; i < cnt; ++i) {
		if (!hole.query(edge[i]))
			++ernum2;
	}
	cout << "sum:  " << asum << endl;
	cout << "error num 2 :   " << ernum2 << endl;
	delete[] items;
	delete[] edge;
}

void test_hole_err3() {
	sblacklist.clear();
	count_type cnt;
	Edge* edge = read_stackoverflow_data(&cnt, 100000000);
	holemerge hole(bl1, bl2, 0, 221);
	asum = 0;
	int ernum = 0;
	int ernum2 = 0;
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	cout << "begin query!" << endl;
	for (int i = 0; i < cnt; ++i) {
		if (!hole.query(edge[i]))
			++ernum2;
	}
	cout << "sum:  " << asum << endl;
	cout << "error num 2 :   " << ernum2 << endl;
	delete[] edge;
}

void test_hole_err4() {
	sblacklist.clear();
	count_type cnt;
	Edge* edge = read_stackoverflow_data(&cnt, 100000000);
	set<Edge, EdgeLess> ed;
	ed.insert(edge, edge + cnt);
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	holemerge hole(bl1, bl2, 0, 221);
	asum = 0;
	int ernum = 0;
	int ernum2 = 0;
	set<Edge, EdgeLess>::iterator it;
	for (it = ed.begin(); it != ed.end(); it++) {
		hole.insert(*it);
		if (!hole.query(*it))
			++ernum;
	}
	cout << "error num:  " << ernum << endl;
	for (it = ed.begin(); it != ed.end(); it++) {
		if (!hole.query(*it))
			++ernum2;
	}
	cout << "sum:  " << asum << endl;
	cout << "error num 2 :   " << ernum2 << endl;
	delete[] edge;
}

void test_hole_th() {
	sblacklist.clear();
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	holemerge hole(bl1, bl2, 0, 221);
	int sum = 0;
	auto t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	auto t2 = steady_clock::now();
	auto t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Insert throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		sum += hole.query(edge[i]);
	}
	t2 = steady_clock::now();
	t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Query throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	cout << "sum: " << sum << endl;
	delete[] items;
	delete[] edge;
}

void test_hole_time() {
	sblacklist.clear();
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	holemerge hole(bl1, bl2, 0, 221);
	int sum = 0;
	clock_t t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	clock_t t2 = clock();
	long t3 = t2 - t1;
	cout << "Insert average time: " << (double)t3 / (double)cnt << endl;
	t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		sum += hole.query(edge[i]);
	}
	t2 = clock();
	t3 = t2 - t1;
	cout << "Query average time: " << (double)t3 / (double)cnt << endl;
	cout << "sum: " << sum << endl;
	delete[] items;
	delete[] edge;
}


void test_Bucket_th() {
	cout << "bucket th: " << endl;
	sblacklist.clear();
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	int U2 = cnt / 32;
	int U1 = 2 * U2;
	Bucket hole;
	int sq = 0;
	uint32_t length = 100000;
	int seed = 0;
	auto t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i].dst);
		sq += (int)(mmhash(edge[i].dst, seed) % (uint32_t)length);
		sq += (int)(mmhash(edge[i].dst, seed) % (uint32_t)length);
		sq += (int)(mmhash(edge[i].dst, seed) % (uint32_t)length);
		sq += (int)(mmhash(edge[i].dst, seed) % (uint32_t)length);
		sq += (int)(mmhash(edge[i].dst, seed) % (uint32_t)length);
	}
	auto t2 = steady_clock::now();
	auto t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "duration: " << t3 << endl;
	cout << "Insert throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		sq += hole.query(edge[i].dst);
		sq += (int)(mmhash(edge[i].dst, seed) % (uint32_t)length);
		sq += (int)(mmhash(edge[i].dst, seed) % (uint32_t)length);
		sq += (int)(mmhash(edge[i].dst, seed) % (uint32_t)length);
		sq += (int)(mmhash(edge[i].dst, seed) % (uint32_t)length);
		sq += (int)(mmhash(edge[i].dst, seed) % (uint32_t)length);
	}

	t2 = steady_clock::now();
	t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "duration: " << t3 << endl;
	cout << "Query throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	cout << "sq: " << sq << endl;
	delete[] items;
	delete[] edge;
}

void test_bSlot_err() {
	sblacklist.clear();
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	set<key_type> ed;
	for (count_type i = 0; i < cnt; ++i)
		ed.insert(edge[i].dst);
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	bSlot hole;
	asum = 0;
	int ernum = 0;
	int ernum2 = 0;
	set<key_type>::iterator it;
	int i = 0;
	for (it = ed.begin(); it != ed.end(); it++) {
		++i;
		Edge now_insert;
		now_insert.src = edge[0].src;
		now_insert.dst = *it;
		hole.insert(now_insert);

		if (!hole.query(now_insert)) {
			cout << "i:" << i << endl;
			++ernum;
		}
	}
	cout << "error num:  " << ernum << endl;
	for (it = ed.begin(); it != ed.end(); it++) {
		Edge now_insert;
		now_insert.src = edge[0].src;
		now_insert.dst = *it;
		if (!hole.query(now_insert))
			++ernum2;
	}
	cout << "sum:  " << asum << endl;
	cout << "error num 2 :   " << ernum2 << endl;
	delete[] items;
	delete[] edge;
}


void test_bSlot_err2() {
	sblacklist.clear();
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	bSlot hole;
	asum = 0;
	int ernum = 0;
	int ernum2 = 0;
	for (int i = 0; i < cnt; ++i) {
		edge[i].src = edge[0].src;
		hole.insert(edge[i]);
	}
	for (int i = 0; i < cnt; ++i) {
		if (!hole.query(edge[i])) {
			cout << "i:" << i << endl;
			++ernum2;
		}
	}
	cout << "sum:  " << asum << endl;
	cout << "error num 2 :   " << ernum2 << endl;
	delete[] items;
	delete[] edge;
}

void test_bslot_th() {
	sblacklist.clear();
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	for (int i = 0; i < cnt; ++i) {
		edge[i].src = edge[0].src;
	}
	bSlot hole;
	auto t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	auto t2 = steady_clock::now();
	auto t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Insert throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		hole.query(edge[i]);
	}
	t2 = steady_clock::now();
	t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Query throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	delete[] items;
	delete[] edge;
}

void test_CMSketch_th() {
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	CMSketch hole;
	int sum = 0;
	auto t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i].src);
	}
	auto t2 = steady_clock::now();
	auto t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Insert throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		sum += hole.query(edge[i].src);
	}
	t2 = steady_clock::now();
	t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Query throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	cout << "sum: " << sum << endl;
	delete[] items;
	delete[] edge;
}

void test_CMSketch_time() {
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	CMSketch hole;
	int sum = 0;
	clock_t t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i].src);
	}
	clock_t t2 = clock();
	long t3 = t2 - t1;
	cout << "Insert average time: " << (double)t3 / (double)cnt << endl;
	t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		sum += hole.query(edge[i].src);
	}
	t2 = clock();
	t3 = t2 - t1;
	cout << "Query average time: " << (double)t3 / (double)cnt << endl;
	cout << "sum: " << sum << endl;
	delete[] items;
	delete[] edge;
}

void test_hole_th2() {
	sblacklist.clear();
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i) {
		memcpy(&edge[i], &items[i], sizeof(Edge));
		edge[i].src = i + 3;
	}
	holemerge hole(bl1, bl2, 0, 221);
	int sum = 0;
	auto t1 = steady_clock::now();
	for (int i = 0; i < cnt / 1000; ++i) {
		hole.insert(edge[i]);
	}
	auto t2 = steady_clock::now();
	auto t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Insert throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	t1 = steady_clock::now();
	for (int i = 0; i < cnt / 1000; ++i) {
		sum += hole.query(edge[i]);
	}
	t2 = steady_clock::now();
	t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Query throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	cout << "sum: " << sum << endl;
	delete[] items;
	delete[] edge;
}

void test_hole_time2() {
	sblacklist.clear();
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i) {
		memcpy(&edge[i], &items[i], sizeof(Edge));
		edge[i].src = i + 3;
	}
	holemerge hole(bl1, bl2, 0, 221);
	int sum = 0;
	clock_t t1 = clock();
	for (int i = 0; i < cnt / 1000; ++i) {
		hole.insert(edge[i]);
	}
	clock_t t2 = clock();
	long t3 = t2 - t1;
	cout << "Insert average time: " << (double)t3 / (double)cnt << endl;
	t1 = clock();
	for (int i = 0; i < cnt / 1000; ++i) {
		sum += hole.query(edge[i]);
	}
	t2 = clock();
	t3 = t2 - t1;
	cout << "Query average time: " << (double)t3 / (double)cnt << endl;
	cout << "sum: " << sum << endl;
	delete[] items;
	delete[] edge;
}

void test_hole_th3() {
	sblacklist.clear();
	count_type cnt = 5 * 1024 * 1024;
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i) {
		edge[i].src = i + 3;
		edge[i].dst = i + 9;
	}
	holemerge hole(bl1, bl2, 0, 221);
	int sum = 0;
	auto t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	auto t2 = steady_clock::now();
	auto t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Insert throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		sum += hole.query(edge[i]);
	}
	t2 = steady_clock::now();
	t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Query throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	cout << "sum: " << sum << endl;
	delete[] edge;
}

void test_hole_time3() {
	sblacklist.clear();
	count_type cnt = 5 * 1024 * 1024;
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i) {
		edge[i].src = i + 3;
		edge[i].dst = i + 9;
	}
	holemerge hole(bl1, bl2, 0, 221);
	int sum = 0;
	clock_t t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	clock_t t2 = clock();
	long t3 = t2 - t1;
	cout << "Insert average time: " << (double)t3 / (double)cnt << endl;
	t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		sum += hole.query(edge[i]);
	}
	t2 = clock();
	t3 = t2 - t1;
	cout << "Query average time: " << (double)t3 / (double)cnt << endl;
	cout << "sum: " << sum << endl;
	delete[] edge;
}

void test_hole_th4() {
	sblacklist.clear();
	count_type cnt = 100 * 1024 * 1024;
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i) {
		edge[i].src = (i % 10240) + 3;
		edge[i].dst = i + 9;
	}
	holemerge hole(bl1, bl2, 0, 221);
	int sum = 0;
	auto t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	auto t2 = steady_clock::now();
	auto t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Insert throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		sum += hole.query(edge[i]);
	}
	t2 = steady_clock::now();
	t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Query throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	cout << "sum: " << sum << endl;
	delete[] edge;
	cout << "sblacklist.size():  " << sblacklist.size() << endl;
}

void test_hole_time4() {
	sblacklist.clear();
	count_type cnt = 100 * 1024 * 1024;
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i) {
		edge[i].src = (i % 10240) + 3;
		edge[i].dst = i + 9;
	}
	holemerge hole(bl1, bl2, 0, 221);
	int sum = 0;
	clock_t t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	clock_t t2 = clock();
	long t3 = t2 - t1;
	cout << "Insert average time: " << (double)t3 / (double)cnt << endl;
	t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		sum += hole.query(edge[i]);
	}
	t2 = clock();
	t3 = t2 - t1;
	cout << "Query average time: " << (double)t3 / (double)cnt << endl;
	cout << "sum: " << sum << endl;
	delete[] edge;
	cout << "sblacklist.size():  " << sblacklist.size() << endl;
}

void test_hole_th5() {
	sblacklist.clear();
	count_type cnt = 100 * 1024 * 1024;
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i) {
		if (i % 3) {
			edge[i].src = (i % 10240) + 3;
			edge[i].dst = i + 9;
		}
		else {
			edge[i].src = i + 10240 + 3;
			edge[i].dst = i + 9;
		}
	}
	holemerge hole(bl1, bl2, 0, 221);
	int sum = 0;
	auto t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	auto t2 = steady_clock::now();
	auto t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Insert throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		sum += hole.query(edge[i]);
	}
	t2 = steady_clock::now();
	t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Query throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	cout << "sum: " << sum << endl;
	delete[] edge;
}

void test_hole_time5() {
	sblacklist.clear();
	count_type cnt = 100 * 1024 * 1024;
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i) {
		if (i % 3) {
			edge[i].src = (i % 10240) + 3;
			edge[i].dst = i + 9;
		}
		else {
			edge[i].src = i + 10240 + 3;
			edge[i].dst = i + 9;
		}
	}
	holemerge hole(bl1, bl2, 0, 221);
	int sum = 0;
	clock_t t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	clock_t t2 = clock();
	long t3 = t2 - t1;
	cout << "Insert average time: " << (double)t3 / (double)cnt << endl;
	t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		sum += hole.query(edge[i]);
	}
	t2 = clock();
	t3 = t2 - t1;
	cout << "Query average time: " << (double)t3 / (double)cnt << endl;
	cout << "sum: " << sum << endl;
	delete[] edge;
}

void test_hole_del_err() {
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	holemerge hole(bl1, bl2, 0, 221);
	asum = 0;
	int ernum = 0;
	int ernum2 = 0;
	cout << "test1 begin !" << endl;
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
		hole.del(edge[i]);
		assert(!hole.query(edge[i]));
	}
	cout << "we passed test1 !" << endl;
	cout << "test2 begin !" << endl;
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	for (int i = 0; i < cnt; ++i) {
		hole.del(edge[i]);
		assert(!hole.query(edge[i]));
	}
	cout << "we passed test2 !" << endl;


	cout << "test3 begin !" << endl;
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	for (int i = 0; i < cnt; ++i) {
		hole.del(edge[i]);
		assert(!hole.query(edge[i]));
		hole.insert(edge[i]);
		assert(hole.query(edge[i]));
	}
	cout << "we passed test3 !" << endl;

	cout << "test4 begin !" << endl;
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
		hole.del(edge[i]);
		assert(!hole.query(edge[i]));
	}
	cout << "we passed test4 !" << endl;

	cout << "we passed all the tests !!!!!" << endl;
	delete[] items;
	delete[] edge;
}

holemerge ours(bl1, bl2, 0, 221);

void test_edge_new(int len)
{
	printf("test edge new begin\n");
	unordered_map<long long, int>::iterator it;
	listChain* tempLC = new listChain();
	clock_t time_start = clock();
	for (int i = 0; i < len; ++i) {
		int s = asrc[i];
		int d = adst[i];
		tempLC->src = s;
		tempLC->dst = d;

		listChain* tempLC_new = query(crossList[s], tempLC);
	}
	clock_t time_end = clock();
	long time1 = time_end - time_start;
	printf("cross end\n");
	chain* tempC = new chain();
	clock_t time_start_1 = clock();
	for (int i = 0; i < len; ++i) {
		int s = asrc[i];
		int d = adst[i];
		tempC->src = s;
		tempC->dst = d;

		bool tempC_new = cs->query_edge(tempC);
	}
	clock_t time_end_1 = clock();
	long time2 = time_end_1 - time_start_1;
	printf("chand end\n");

	Edge edge;
	int sum = 0;    clock_t time_start_2 = clock();
	for (int i = 0; i < len; ++i) {
		edge.src = asrc[i];
		edge.dst = adst[i];
		sum += ours.query(edge);
	}
	clock_t time_end_2 = clock();
	long time3 = time_end_2 - time_start_2;
	printf("ours end\n");


	ofstream fout;
	fout.open("test_edge_new.txt", ios::out);
	printf("open file writing\n");
	fout << "len " << len << endl;
	fout << "crossList:\t" << time1 << endl;
	fout << " averageTime:\t" << (double)time1 / (double)len << endl;
	fout << "chand:\t" << time2 << endl;
	fout << " averageTime:\t" << (double)time2 / (double)len << endl;
	fout << "ours:\t" << time3 << endl;
	fout << " averageTime:\t" << (double)time3 / (double)len << endl;
	fout.close();
	printf("test edge end\n");
	printf("sum:  %d\n", sum);
}

int build_query() {
	unordered_map<int, listChain*>::iterator it;
	int cnt = 0;
	for (it = crossList.begin(); it != crossList.end(); ++it) {
		int src = it->first;

		listChain* temp = it->second;
		for (; temp != nullptr; temp = temp->pointer_suc) {
			qsrc[cnt] = temp->src;
			qdst[cnt] = temp->dst;
			cnt++;
		}
	}
	return cnt;
}



long insert_ours(int len) {
	sblacklist.clear();
	Edge edge;
	int sum = 0;
	clock_t time_start = clock();
	for (int i = 0; i < len; ++i) {
		edge.src = asrc[i];
		edge.dst = adst[i];
		ours.insert(edge);
	}
	clock_t time_end = clock();
	long time = time_end - time_start;
	return time;
}

void test_insert(int len) {
	printf("test insert begin\n");
	long time1 = insert_cross(len);
	printf("cross end\n");
	long time2 = insert_chand(len);
	printf("chand end\n");
	long time3 = insert_ours(len);
	printf("ours end\n");
	ofstream fout;
	fout.open("test_insert.txt", ios::out);
	fout << "len " << len << endl;
	fout << "crossList:\t" << time1 << endl;
	fout << " averageTime:\t" << (double)time1 / (double)len << endl;
	fout << "chand:\t" << time2 << endl;
	fout << " averageTime:\t" << (double)time2 / (double)len << endl;
	fout << "ours:\t" << time3 << endl;
	fout << " averageTime:\t" << (double)time3 / (double)len << endl;
	fout.close();
}

void test_bSlot_rate() {
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	set<key_type> ed;
	for (count_type i = 0; i < cnt; ++i)
		ed.insert(edge[i].dst);
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	bSlot hole;
	set<key_type>::iterator it;
	string fname = "./bSlot.csv";
	ofstream fout;
	fout.open(fname);
	int i = 0;
	for (it = ed.begin(); it != ed.end(); it++) {
		Edge e;
		e.src = 999;
		e.dst = *it;
		hole.insert(e);
		++i;
		fout << i << "," << hole.test_rate() << endl;
	}
	fout.close();
	delete[] items;
	delete[] edge;
}


void test_Hole_rate() {
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	set<Edge, EdgeLess> ed;
	ed.insert(edge, edge + cnt);
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	holemerge hole(bl1, bl2, 0, 221);
	string fname = "./Hole.csv";
	ofstream fout;
	fout.open(fname);
	int i = 0;
	for (auto it = ed.begin(); it != ed.end(); it++) {
		hole.insert(*it);
		++i;

		fout << i << "," << hole.test_rate() << endl;
	}
	fout.close();
	delete[] items;
	delete[] edge;
}

#ifdef TEST_MEM
void test_Hole_mem() {
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	set<Edge, EdgeLess> ed;
	ed.insert(edge, edge + cnt);
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	holemerge hole(bl1, bl2, 0, 221);
#ifdef TEST_MEM 
	cuckoo_mem = sizeof(holemerge) + sizeof(sblacklist) + sbl * sizeof(Edge) + bbl * sizeof(bSlot);
#endif 
	string fname = "./Hole_mem.csv";
	ofstream fout;
	fout.open(fname);
	int i = 0;
	for (auto it = ed.begin(); it != ed.end(); it++) {
		hole.insert(*it);
		++i;

		fout << i << "," << ((double)cuckoo_mem) / 1000 << endl;
	}
	fout.close();
	delete[] items;
	delete[] edge;
#ifdef TEST_MEM
	cuckoo_mem = 0;
#endif
}
#endif  

#ifdef TEST_MEM
void test_wind_mem() {
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	set<Edge, EdgeLess> ed;
	ed.insert(edge, edge + cnt);
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	int csl = 400;
	cs = new Chandelier(csl);
	string fname = "./wind_mem.csv";
	ofstream fout;
	fout.open(fname);
	int i = 0;
	for (auto it = ed.begin(); it != ed.end(); it++) {
		chain* c = new chain();
		c->src = (*it).src;
		c->dst = (*it).dst;
		c->pointer_chand = nullptr;
		cs->insert(c);

		fout << ((double)wind_mem) / 1000 << endl;
	}
	fout.close();
	delete[] items;
	delete[] edge;
	wind_mem = 0;
}
#endif  

void test_pt() {
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	set<Edge, EdgeLess> ed;
	ed.insert(edge, edge + cnt);
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	holemerge hole(bl1, bl2, 0, 221);
	set<key_type> dsrc;
	for (count_type i = 0; i < cnt; ++i)
		dsrc.insert(edge[i].src);
	for (auto it = ed.begin(); it != ed.end(); it++) {
		hole.insert(*it);
	}
	cout << "distinct src num: " << dsrc.size() << endl;
	int a[7] = { 0 };
	if (hole.link[0] == NULL)return;
	for (int i = 0; i < hole.link[0]->len1; ++i)
		for (int j = 0; j < bup; ++j) {
			if (hole.link[0]->layer1.layer[i].bslot[j].pt >= 0)++a[hole.link[0]->layer1.layer[i].bslot[j].pt];
		}

	for (int i = 0; i < hole.link[0]->len2; ++i)
		for (int j = 0; j < bup; ++j) {
			if (hole.link[0]->layer2.layer[i].bslot[j].pt >= 0)++a[hole.link[0]->layer2.layer[i].bslot[j].pt];
		}

	double sum = (hole.link[0]->len2 + hole.link[0]->len1) * bup;
	cout << "sum: " << sum << endl;
	cout << "0: " << a[0] << "       " << ((double)a[0]) / sum << endl;
	double s = a[0];
	for (int i = 1; i < 7; ++i) {
		cout << i << ": " << a[i] << "  " << ((double)a[i]) / dsrc.size() << "  " << ((double)a[i]) / sum << endl;
		s += a[i];
	}
	s = sum - s;
	cout << "rest: " << s << " " << s / sum << endl;
	cout << a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + s << endl;
	delete[] items;
	delete[] edge;
}

void test_pt2() {
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	set<Edge, EdgeLess> ed;
	ed.insert(edge, edge + cnt);
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	holemerge hole(bl1, bl2, 0, 221);
	set<key_type> dsrc;
	for (count_type i = 0; i < cnt; ++i)
		dsrc.insert(edge[i].src);
	for (auto it = ed.begin(); it != ed.end(); it++) {
		hole.insert(*it);
	}
	cout << "distinct src num: " << dsrc.size() << endl;
	double sum = 0;
	int a[7] = { 0 };

	for (int W = 0; W < 3; ++W) {
		if (hole.link[W] == NULL)continue;
		for (int i = 0; i < hole.link[W]->len1; ++i)
			for (int j = 0; j < bup; ++j) {
				if (hole.link[W]->layer1.layer[i].bslot[j].pt >= 0)++a[hole.link[W]->layer1.layer[i].bslot[j].pt];
			}

		for (int i = 0; i < hole.link[W]->len2; ++i)
			for (int j = 0; j < bup; ++j) {
				if (hole.link[W]->layer2.layer[i].bslot[j].pt >= 0)++a[hole.link[W]->layer2.layer[i].bslot[j].pt];
			}

		sum = sum + (hole.link[W]->len2 + hole.link[W]->len1) * bup;
	}

	cout << "sum: " << sum << endl;
	cout << "0: " << a[0] << "       " << ((double)a[0]) / sum << endl;
	double s = a[0];
	for (int i = 1; i < 7; ++i) {
		cout << i << ": " << a[i] << "  " << ((double)a[i]) / dsrc.size() << "  " << ((double)a[i]) / sum << endl;
		s += a[i];
	}
	s = sum - s;
	cout << "rest: " << s << " " << s / sum << endl;
	cout << a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + s << endl;
	delete[] items;
	delete[] edge;
}


void test_pt3() {
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	set<Edge, EdgeLess> ed;
	ed.insert(edge, edge + cnt);
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	holemerge hole(bl1, bl2, 0, 221);
	set<key_type> dsrc;
	for (count_type i = 0; i < cnt; ++i)
		dsrc.insert(edge[i].src);
#ifdef TEST_MEM
	cuckoo_mem = sizeof(holemerge);
	int i = 0;
	bool ff = false;
#endif
	for (auto it = ed.begin(); it != ed.end(); it++) {
		hole.insert(*it);
#ifdef TEST_MEM
		++i;
		if (hole.link[0]->len2 == 4096 && hole.link[0]->num >= (hole.link[0]->len1 + hole.link[0]->len2) * bup * rate && ff == false)
		{
			ff = true;
			cout << "cuckoo_mem: " << cuckoo_mem << endl;
			break;
		}
#endif
	}
	cout << "distinct src num: " << dsrc.size() << endl;
	int a[7] = { 0 };
	if (hole.link[0] == NULL)return;
	for (int i = 0; i < hole.link[0]->len1; ++i)
		for (int j = 0; j < bup; ++j) {
			if (hole.link[0]->layer1.layer[i].bslot[j].pt >= 0)++a[hole.link[0]->layer1.layer[i].bslot[j].pt];
		}

	for (int i = 0; i < hole.link[0]->len2; ++i)
		for (int j = 0; j < bup; ++j) {
			if (hole.link[0]->layer2.layer[i].bslot[j].pt >= 0)++a[hole.link[0]->layer2.layer[i].bslot[j].pt];
		}

	double sum = (hole.link[0]->len2 + hole.link[0]->len1) * bup;
	cout << "sum: " << sum << endl;
	cout << "0: " << a[0] << "       " << ((double)a[0]) / sum << endl;
	double s = a[0];
	for (int i = 1; i < 7; ++i) {
		cout << i << ": " << a[i] << "  " << ((double)a[i]) / dsrc.size() << "  " << ((double)a[i]) / sum << endl;
		s += a[i];
	}
	s = sum - s;
	cout << "rest: " << s << " " << s / sum << endl;
	cout << a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + s << endl;
	cout << "hole.link[0]->len2: " << hole.link[0]->len2 << endl;
#ifdef TEST_MEM
	cout << "cuckoo_mem: " << cuckoo_mem << endl;
#endif
	delete[] items;
	delete[] edge;
#ifdef TEST_MEM
	cuckoo_mem = 0;
#endif
}


void test_delta_old() {
	sblacklist.clear();
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	cnt = 1000;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	edge[10].src = 3; edge[10].dst = 4;
	edge[111].src = 4; edge[111].dst = 5;
	edge[9].src = 5; edge[9].dst = 3;
	set<Edge, EdgeLess> ed;
	ed.insert(edge, edge + cnt);
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	set<key_type> ip;
	for (count_type i = 0; i < cnt; ++i) {
		ip.insert(edge[i].src);
		ip.insert(edge[i].dst);
	}
	int ipsize = ip.size();
	cout << "ip size:  " << ipsize << endl;
	key_type* a = new key_type[ipsize];
	int pp = 0;
	for (auto it = ip.begin(); it != ip.end(); it++) {
		a[pp] = (*it);
		++pp;
	}
	int wb_cnt = 0;
	int ours_cnt = 0;
	holemerge hole(bl1, bl2, 0, 221);
	int CSL = 400;
	Chandelier* CS = new Chandelier(CSL);
	set<Edge, EdgeLess>::iterator it;
	for (it = ed.begin(); it != ed.end(); it++) {
		hole.insert(*it);
		chain* c = new chain();
		c->src = (*it).src;
		c->dst = (*it).dst;
		CS->insert(c);
	}

	clock_t time_start_1 = clock();
	for (int i = 0; i < ipsize - 2; ++i)
		for (int j = i + 1; j < ipsize - 1; ++j)
			for (int k = j + 1; k < ipsize; ++k) {
				Edge c1, c2, c3;
				c1.src = a[i];
				c1.dst = a[j];
				c2.src = a[j];
				c2.dst = a[k];
				c3.src = a[k];
				c3.dst = a[i];
				if (hole.query(c1) && hole.query(c2) && hole.query(c3))
					++ours_cnt;
			}
	clock_t time_end_1 = clock();
	long time1 = time_end_1 - time_start_1;

	clock_t time_start_2 = clock();
	for (int i = 0; i < ipsize - 2; ++i)
		for (int j = i + 1; j < ipsize - 1; ++j)
			for (int k = j + 1; k < ipsize; ++k) {
				chain* c1 = new chain();
				chain* c2 = new chain();
				chain* c3 = new chain();
				c1->src = a[i];
				c1->dst = a[j];
				c2->src = a[j];
				c2->dst = a[k];
				c3->src = a[k];
				c3->dst = a[i];
				if (CS->query_edge(c1) && CS->query_edge(c2) && CS->query_edge(c3))
					++wb_cnt;
			}
	clock_t time_end_2 = clock();
	long time2 = time_end_2 - time_start_2;

	cout << "ours cnt: " << ours_cnt << endl;
	cout << "ours time: " << time1 << endl;
	cout << "WindBell cnt: " << wb_cnt << endl;
	cout << "WindBell time: " << time2 << endl;
	delete[] a;
	delete[] items;
	delete[] edge;
}






void test_insert_par(int len) {
	printf("test insert par begin\n");

	long time3 = insert_ours(len);
	printf("ours end\n");
	long time2 = insert_chand(len);
	printf("chand end\n");

	ofstream fout;
	fout.open("test_insert_par.txt", ios::out);
	fout << "len " << len << endl;
	fout << "chand:\t" << time2 << endl;
	fout << " averageTime:\t" << (double)time2 / (double)len << endl;
	fout << "ours:\t" << time3 << endl;
	fout << " averageTime:\t" << (double)time3 / (double)len << endl;
	fout.close();
}

void test_edge_new_par(int len)
{
	printf("test edge new par begin\n");
	chain* tempC = new chain();
	clock_t time_start_1 = clock();
	for (int i = 0; i < len; ++i) {
		int s = asrc[i];
		int d = adst[i];
		tempC->src = s;
		tempC->dst = d;

		bool tempC_new = cs->query_edge(tempC);
	}
	clock_t time_end_1 = clock();
	long time2 = time_end_1 - time_start_1;
	printf("chand end\n");

	Edge edge;
	int sum = 0;    clock_t time_start_2 = clock();
	for (int i = 0; i < len; ++i) {
		edge.src = asrc[i];
		edge.dst = adst[i];
		sum += ours.query(edge);
	}
	clock_t time_end_2 = clock();
	long time3 = time_end_2 - time_start_2;
	printf("ours end\n");


	ofstream fout;
	fout.open("test_edge_new_par.txt", ios::out);
	printf("open file writing\n");
	fout << "len " << len << endl;
	fout << "chand:\t" << time2 << endl;
	fout << " averageTime:\t" << (double)time2 / (double)len << endl;
	fout << "ours:\t" << time3 << endl;
	fout << " averageTime:\t" << (double)time3 / (double)len << endl;
	fout.close();
	printf("test edge end\n");
	printf("sum:  %d\n", sum);
}

void test_wiki_time() {
	count_type cnt;
	Edge* edge = read_wiki_data(&cnt, 100000000);
	cout << "cnt: " << cnt << endl;
	for (int i = 0; i < cnt; ++i) {
		asrc[i] = edge[i].src;
		adst[i] = edge[i].dst;
	}

	int csl = 400;
	cs = new Chandelier(csl);


	test_insert_par(cnt);
	test_edge_new_par(cnt);

	delete[] edge;
}

void test_time_wind_vs_ours_stackoverflow() {
	count_type cnt;
	Edge* edge = read_stackoverflow_data(&cnt, 100000000);
	cout << "cnt: " << cnt << endl;

	int csl = 400;
	cs = new Chandelier(csl);


	test_insert_par(cnt);
	test_edge_new_par(cnt);

	delete[] edge;
}

void test_time_wind_vs_ours_NotreDame() {
	count_type cnt;
	Edge* edge = read_NotreDame_data(&cnt, 100000000);
	cout << "cnt: " << cnt << endl;

	int csl = 400;
	cs = new Chandelier(csl);


	test_insert_par(cnt);
	test_edge_new_par(cnt);

	delete[] edge;
}

long insert_chand_new(int len) {
	int csl = 400;
	Chandelier* wind;
	wind = new Chandelier(csl);

	clock_t time_start = clock();
	for (int i = 0; i < len; ++i) {
		chain* c = new chain();
		c->src = asrc[i];
		c->dst = adst[i];
		c->pointer_chand = nullptr;
		wind->insert(c);

	}
	clock_t time_end = clock();
	long time = time_end - time_start;
	return time;
}

long insert_ours_new(int len) {
	sblacklist.clear();
	Edge edge;
	holemerge hole(bl1, bl2, 0, 221);
	int sum = 0;
	clock_t time_start = clock();
	for (int i = 0; i < len; ++i) {
		edge.src = asrc[i];
		edge.dst = adst[i];
		sum += hole.insert(edge);
	}
	clock_t time_end = clock();
	long time = time_end - time_start;
	cout << "sum: " << sum << endl;
	return time;
}

void test_insert_multi(int len, int mul) {
	printf("test insert multi begin\n");
	long time3 = 0;
	long time2 = 0;
	for (int i = 1; i < mul; ++i)
		time3 = time3 + insert_ours_new(len);
	time3 = time3 + insert_ours(len);
	printf("ours end\n");
	for (int i = 1; i < mul; ++i)
		time2 = time2 + insert_chand_new(len);
	time2 = time2 + insert_chand(len);
	printf("chand end\n");

	double t3 = (double)time3;
	t3 = t3 / mul;

	double t2 = (double)time2;
	t2 = t2 / mul;

	ofstream fout;
	fout.open("test_insert_multi.txt", ios::out);
	fout << "len " << len << endl;
	fout << "chand:\t" << t2 << endl;
	fout << " averageTime:\t" << (double)t2 / (double)len << endl;
	fout << "ours:\t" << t3 << endl;
	fout << " averageTime:\t" << (double)t3 / (double)len << endl;
	fout.close();
}

void test_edge_new_multi(int len, int mul)
{
	printf("test edge new multi begin\n");
	long time2 = 0;
	long time3 = 0;
	chain* tempC = new chain();
	for (int i = 0; i < mul; ++i) {
		clock_t time_start_1 = clock();
		for (int i = 0; i < len; ++i) {
			int s = asrc[i];
			int d = adst[i];
			tempC->src = s;
			tempC->dst = d;

			bool tempC_new = cs->query_edge(tempC);
		}
		clock_t time_end_1 = clock();
		time2 = time_end_1 - time_start_1 + time2;
	}
	printf("chand end\n");

	Edge edge;
	int sum = 0;    for (int i = 0; i < mul; ++i) {
		clock_t time_start_2 = clock();
		for (int i = 0; i < len; ++i) {
			edge.src = asrc[i];
			edge.dst = adst[i];
			sum += ours.query(edge);
		}
		clock_t time_end_2 = clock();
		time3 = time_end_2 - time_start_2 + time3;
	}
	printf("ours end\n");

	double t3 = (double)time3;
	t3 = t3 / mul;

	double t2 = (double)time2;
	t2 = t2 / mul;

	ofstream fout;
	fout.open("test_edge_new_multi.txt", ios::out);
	printf("open file writing\n");
	fout << "len " << len << endl;
	fout << "chand:\t" << time2 << endl;
	fout << " averageTime:\t" << (double)t2 / (double)len << endl;
	fout << "ours:\t" << time3 << endl;
	fout << " averageTime:\t" << (double)t3 / (double)len << endl;
	fout.close();
	printf("test edge end\n");
	printf("sum:  %d\n", sum);
}

void test_time_wind_vs_ours_multi() {
	count_type cnt;
	int mul = 3;
	Edge* edge = read_stackoverflow_data(&cnt, 100000000);
	cout << "cnt: " << cnt << endl;

	int csl = 400;
	cs = new Chandelier(csl);


	test_insert_multi(cnt, mul);
	test_edge_new_multi(cnt, mul);

	delete[] edge;
}



void hole_test_time_multi() {
	Edge edge;
	count_type cnt;
	int mul = 3;
	double insert[10];
	double query[10];
	for (int i = 0; i < 10; ++i) {
		insert[i] = 0;
		query[i] = 0;
	}
	Edge* EE = read_wiki_data(&cnt, 100000000);
	cout << "cnt: " << cnt << endl;


	int fin = 24000;
	set<key_type> ed;
	for (count_type i = 0; i < fin * 1000; ++i)
		ed.insert(asrc[i]);
	int edsize = ed.size();

	ofstream fout;
	fout.open("hole_test_time_multi.txt", ios::out);

	for (int num = 12000; num <= fin; num += 3000) {
		cnt = num * 1000;
		fout << "DATA SIZE: " << cnt << endl;
		for (int i = 0; i < mul; ++i) {
			int l2 = (edsize / (3 * bup * brate)) + 1;
			if (l2 % 2)l2 = l2 + 1;
			int l1 = l2 * 2;
			cout << "l1:  " << l1 << endl;
			cout << "l2:  " << l2 << endl;
			sblacklist.clear();
			holemerge hole(l1, l2, 0, 221);
			int sum = 0;
			clock_t time_start = clock();
			for (int i = 0; i < cnt; ++i) {
				edge.src = asrc[i];
				edge.dst = adst[i];
				sum += hole.insert(edge);
			}
			clock_t time_end = clock();
			long time = time_end - time_start;
			insert[i] = (double)time / (double)cnt;
			cout << "sum: " << sum << endl;
			sum = 0;
			clock_t time_start_2 = clock();
			for (int i = 0; i < cnt; ++i) {
				edge.src = asrc[i];
				edge.dst = adst[i];
				sum += hole.query(edge);
			}
			clock_t time_end_2 = clock();
			long time3 = time_end_2 - time_start_2;
			query[i] = (double)time3 / (double)cnt;
			cout << "sum: " << sum << endl;
			cout << "hole.state:  " << hole.state << endl;

		}

		double average_insert = 0;
		double average_query = 0;
		for (int i = 0; i < mul; ++i) {
			fout << "insert" << i << ":  " << insert[i] << endl;
			average_insert += insert[i];
			fout << "query" << i << ":  " << query[i] << endl;
			average_query += query[i];
		}
		average_insert = average_insert / mul;
		average_query = average_query / mul;
		fout << "average_insert:     " << average_insert << endl;
		fout << "average_query:      " << average_query << endl;

	}
	fout.close();
	delete[] EE;
}

void wind_test_time_multi() {
	chain* tempC = new chain();
	int csl = 400;
	count_type cnt;
	int mul = 3;
	double insert[10];
	double query[10];
	for (int i = 0; i < 10; ++i) {
		insert[i] = 0;
		query[i] = 0;
	}
	Edge* edge = read_stackoverflow_data(&cnt, 100000000);
	cout << "cnt: " << cnt << endl;
	ofstream fout;
	fout.open("wind_test_time_multi.txt", ios::out);

	for (int num = 35000; num <= 63000; num += 7000) {
		cnt = num * 1000;
		fout << "DATA SIZE: " << cnt << endl;
		for (int i = 0; i < mul; ++i) {
			Chandelier* wind;
			wind = new Chandelier(csl);
			int sum = 0;
			clock_t time_start = clock();
			for (int i = 0; i < cnt; ++i) {
				chain* c = new chain();
				c->src = asrc[i];
				c->dst = adst[i];
				c->pointer_chand = nullptr;
				wind->insert(c);
			}
			clock_t time_end = clock();
			long time = time_end - time_start;
			insert[i] = (double)time / (double)cnt;
			cout << "sum: " << sum << endl;
			sum = 0;
			clock_t time_start_2 = clock();
			for (int i = 0; i < cnt; ++i) {
				int s = asrc[i];
				int d = adst[i];
				tempC->src = s;
				tempC->dst = d;

				bool tempC_new = wind->query_edge(tempC);
			}
			clock_t time_end_2 = clock();
			long time3 = time_end_2 - time_start_2;
			query[i] = (double)time3 / (double)cnt;
			cout << "sum: " << sum << endl;
			delete wind;
		}

		double average_insert = 0;
		double average_query = 0;
		for (int i = 0; i < mul; ++i) {
			fout << "insert" << i << ":  " << insert[i] << endl;
			average_insert += insert[i];
			fout << "query" << i << ":  " << query[i] << endl;
			average_query += query[i];
		}
		average_insert = average_insert / mul;
		average_query = average_query / mul;
		fout << "average_insert:     " << average_insert << endl;
		fout << "average_query:      " << average_query << endl;
	}
	fout.close();
	delete[] edge;
}





void test_tiangles_2() {
	int csl = 400;
	count_type cnt;
	int mul = 1;
	int beg = 600000;
	int inter = 800000 - 600000;
	int sample_num = 50000;
	Edge* EE = read_NotreDame_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();


	Chandelier* wind;
	holemerge hole(bl1, bl2, 0, 221);
	wind = new Chandelier(csl);

	int inum = 0;
	int sum1 = 0;
	int sum2 = 0;
	for (int itt = 0; itt < cnt; ++itt) {
		if (ed.count(EE[itt]))continue;
		ed.insert(EE[itt]);
		chain* c = new chain();
		c->src = EE[itt].src;
		c->dst = EE[itt].dst;
		c->pointer_chand = nullptr;
		wind->insert(c);

		hole.insert(EE[itt]);
		edg.insert(EE[itt].dst);
		edg.insert(EE[itt].src);        inum = ed.size();
		if (inum == 600000) {
			cout << "inum: " << inum << endl;
			cout << "hole.state: " << hole.state << endl;
			double wind_average = 0;
			double ours_average = 0;
			cout << "distinct node num: " << edg.size() << endl;
			for (int mm = 0; mm < mul; ++mm) {
				set<key_type> sample;
				while (sample.size() != sample_num) {
					int r = rand() % edg.size();
					auto point = edg.begin();
					for (int rrr = 0; rrr < r; ++rrr) {
						++point;
					}
					sample.insert(*point);
				}
				auto it = sample.begin();
				int cyc = sample.size();
				clock_t time_start = clock();
				for (int i = 0; i < cyc; ++i) {
					key_type ms = *it;
					++it;
					sum1 += wind->query_triangles(ms);
				}
				clock_t time_end = clock();
				long time = time_end - time_start;
				cout << "wind average time: " << (double)time / (double)cyc << endl;
				wind_average += (double)time / (double)cyc;

				cout << "wind end" << endl;


				it = sample.begin();
				clock_t time_start_2 = clock();
				for (int i = 0; i < cyc; ++i) {
					key_type ms = *it;
					++it;
					sum2 += hole.query_triangles(ms);
				}
				clock_t time_end_2 = clock();
				long time3 = time_end_2 - time_start_2;
				cout << "ours average time: " << (double)time3 / (double)cyc << endl;
				ours_average += (double)time3 / (double)cyc;

				cout << "ours end" << endl;
			}
			ours_average = ours_average / (double)mul;
			wind_average = wind_average / (double)mul;
			cout << "Wind Average Average time: " << wind_average << endl;
			cout << "Ours Average Average time: " << ours_average << endl;
			break;
		}
	}

	cout << "sum1: " << sum1 << endl;
	cout << "sum2: " << sum2 << endl;
	delete[] EE;
	delete wind;

}

void test_tiangles_1() {
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int beg = 600000;
	int inter = 800000 - 600000;
	int sample_num = 5000;
	Edge* EE = read_NotreDame_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();

	ofstream fout;
	fout.open("test_tiangles_1.txt", ios::out);

	Chandelier* wind;
	holemerge hole(bl1, bl2, 0, 221);
	wind = new Chandelier(csl);

	int inum = 0;
	int sum1 = 0;
	int sum2 = 0;
	for (int itt = 0; itt < cnt; ++itt) {
		if (ed.count(EE[itt]))continue;
		ed.insert(EE[itt]);
		chain* c = new chain();
		c->src = EE[itt].src;
		c->dst = EE[itt].dst;
		c->pointer_chand = nullptr;
		wind->insert(c);

		hole.insert(EE[itt]);
		edg.insert(EE[itt].dst);
		edg.insert(EE[itt].src);        inum = ed.size();
		if ((inum >= beg) && (((inum - beg) % inter) == 0)) {
			fout << "inum: " << inum << endl;
			cout << "hole.state: " << hole.state << endl;
			double wind_average = 0;
			double ours_average = 0;
			cout << "distinct node num: " << edg.size() << endl;
			for (int mm = 0; mm < mul; ++mm) {
				set<key_type> sample;
				while (sample.size() != sample_num) {
					int r = rand() % edg.size();
					auto point = edg.begin();
					for (int rrr = 0; rrr < r; ++rrr) {
						++point;
					}
					sample.insert(*point);
				}
				for (auto y = sample.begin(); y != sample.end(); ++y) {
					need.push(*y);
				}
				auto it = sample.begin();
				int cyc = sample.size();
				clock_t time_start = clock();
				for (int i = 0; i < cyc; ++i) {
					key_type ms = *it;
					++it;
					sum1 += wind->query_triangles(ms);
				}
				clock_t time_end = clock();
				long time = time_end - time_start;
				fout << "wind average time: " << (double)time / (double)cyc << endl;
				wind_average += (double)time / (double)cyc;

				cout << "wind end" << endl;


				it = sample.begin();
				clock_t time_start_2 = clock();
				for (int i = 0; i < cyc; ++i) {
					key_type ms = *it;
					++it;
					sum2 += hole.query_triangles(ms);
				}
				clock_t time_end_2 = clock();
				long time3 = time_end_2 - time_start_2;
				fout << "ours average time: " << (double)time3 / (double)cyc << endl;
				ours_average += (double)time3 / (double)cyc;

				cout << "ours end" << endl;
			}
			ours_average = ours_average / (double)mul;
			wind_average = wind_average / (double)mul;
			fout << "Wind Average Average time: " << wind_average << endl;
			fout << "Ours Average Average time: " << ours_average << endl;
		}
	}


	cout << "sum1: " << sum1 << endl;
	cout << "sum2: " << sum2 << endl;
	fout.close();
	delete[] EE;
	delete wind;

}

void test_tiangles_co() {
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int beg = 600000;
	int inter = 800000 - 600000;
	int sample_num = 5000;

	assert(need.size() == mul * sample_num * 5);

	Edge* EE = read_NotreDame_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();

	ofstream fout;
	fout.open("test_tiangles_co.txt", ios::out);

	holemerge hole(14108, 7054, 0, 221);

	int inum = 0;
	int sum1 = 0;
	int sum2 = 0;
	for (int itt = 0; itt < cnt; ++itt) {
		if (ed.count(EE[itt]))continue;
		ed.insert(EE[itt]);
		hole.insert(EE[itt]);
		edg.insert(EE[itt].dst);
		edg.insert(EE[itt].src);        inum = ed.size();
		if ((inum >= beg) && (((inum - beg) % inter) == 0)) {
			fout << "inum: " << inum << endl;
			cout << "hole.state: " << hole.state << endl;
			double wind_average = 0;
			double ours_average = 0;
			cout << "distinct node num: " << edg.size() << endl;
			for (int mm = 0; mm < mul; ++mm) {
				set<key_type> sample;
				for (int iii = 0; iii < sample_num; ++iii) {
					sample.insert(need.front());
					need.pop();
				}
				auto it = sample.begin();
				int cyc = sample.size();
				it = sample.begin();
				clock_t time_start_2 = clock();
				for (int i = 0; i < cyc; ++i) {
					key_type ms = *it;
					++it;
					sum2 += hole.query_triangles(ms);
				}
				clock_t time_end_2 = clock();
				long time3 = time_end_2 - time_start_2;
				fout << "ours average time: " << (double)time3 / (double)cyc << endl;
				ours_average += (double)time3 / (double)cyc;

				cout << "ours end" << endl;
			}
			ours_average = ours_average / (double)mul;
			wind_average = wind_average / (double)mul;
			fout << "Wind Average Average time: " << wind_average << endl;
			fout << "Ours Average Average time: " << ours_average << endl;
		}
	}

	assert(need.empty());
	cout << "sum1: " << sum1 << endl;
	cout << "sum2: " << sum2 << endl;
	fout.close();
	delete[] EE;

}

void test_time_succ() {
	chain* tempC = new chain();
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int beg = 4100000;
	int inter = 5400000 - 4100000;
	int sample_num = 5000;
	Edge* EE = read_wiki_data(&cnt, 100000000);
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	ofstream fout;
	ofstream fr1;
	ofstream fr2;
	fout.open("test_time_succ.txt", ios::out);
	fr1.open("fr1.txt", ios::out);
	fr2.open("fr2.txt", ios::out);

	Chandelier* wind;
	holemerge hole(bl1, bl2, 0, 221);
	wind = new Chandelier(csl);
	int inum = 0;
	int sum1 = 0;
	int sum2 = 0;
	for (int it = 0; it < cnt; ++it) {
		if (ed.count(EE[it]))continue;
		ed.insert(EE[it]);
		chain* c = new chain();
		c->src = EE[it].src;
		c->dst = EE[it].dst;
		c->pointer_chand = nullptr;
		wind->insert(c);
		hole.insert(EE[it]);
		edg.insert(EE[it].src);
		inum = ed.size();
		if ((inum >= beg) && (((inum - beg) % inter) == 0)) {
			fout << "inum: " << inum << endl;
			cout << "hole.state: " << hole.state << endl;
			double wind_average = 0;
			double ours_average = 0;
			cout << "distinct src num: " << edg.size() << endl;
			for (int mm = 0; mm < mul; ++mm) {
				set<key_type> sample;
				while (sample.size() != sample_num) {
					int r = rand() % edg.size();
					auto point = edg.begin();
					for (int rrr = 0; rrr < r; ++rrr) {
						++point;
					}
					sample.insert(*point);
				}

				for (auto y = sample.begin(); y != sample.end(); ++y) {
					need.push(*y);
				}
				auto it = sample.begin();
				int cyc = sample.size();
				clock_t time_start = clock();
				for (int i = 0; i < cyc; ++i) {
					key_type ms = *it;
					++it;
					sum1 += wind->query_successor(ms, successor, sum1);
				}
				clock_t time_end = clock();
				long time = time_end - time_start;
				fout << "wind average time: " << (double)time / (double)cyc << endl;
				wind_average += (double)time / (double)cyc;

				cout << "wind end" << endl;


				it = sample.begin();
				clock_t time_start_2 = clock();
				for (int i = 0; i < cyc; ++i) {
					key_type ms = *it;
					++it;
					sum2 += hole.query_successor(ms, successor1, sum2);
				}
				clock_t time_end_2 = clock();
				long time3 = time_end_2 - time_start_2;
				fout << "ours average time: " << (double)time3 / (double)cyc << endl;
				ours_average += (double)time3 / (double)cyc;

				cout << "ours end" << endl;
			}
			ours_average = ours_average / (double)mul;
			wind_average = wind_average / (double)mul;
			fout << "Wind Average Average time: " << wind_average << endl;
			fout << "Ours Average Average time: " << ours_average << endl;
		}
	}
	cout << "sum1: " << sum1 << endl;
	for (int ii = 0; ii < sum1; ++ii)
		fr1 << successor[ii] << endl;
	cout << "sum2: " << sum2 << endl;
	for (int ii = 0; ii < sum2; ++ii)
		fr2 << successor1[ii] << endl;
	fout.close();
	fr1.close();
	fr2.close();
	delete[] EE;
	delete wind;
}

void test_time_succ_co() {
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int beg = 4100000;
	int inter = 5400000 - 4100000;
	int sample_num = 5000;
	Edge* EE = read_wiki_data(&cnt, 100000000);
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	ofstream fout;
	ofstream fr1;
	ofstream fr2;
	fout.open("test_time_succ_co.txt", ios::out);
	fr1.open("fr1_co.txt", ios::out);
	fr2.open("fr2_co.txt", ios::out);

	holemerge hole(73984, 36992, 0, 221);
	int inum = 0;
	int sum1 = 0;
	int sum2 = 0;
	for (int it = 0; it < cnt; ++it) {
		if (ed.count(EE[it]))continue;
		ed.insert(EE[it]);
		hole.insert(EE[it]);
		edg.insert(EE[it].src);
		inum = ed.size();
		if ((inum >= beg) && (((inum - beg) % inter) == 0)) {
			fout << "inum: " << inum << endl;
			cout << "hole.state: " << hole.state << endl;
			double wind_average = 0;
			double ours_average = 0;
			cout << "distinct src num: " << edg.size() << endl;
			for (int mm = 0; mm < mul; ++mm) {
				set<key_type> sample;
				for (int iii = 0; iii < sample_num; ++iii) {
					sample.insert(need.front());
					need.pop();
				}
				auto it = sample.begin();
				int cyc = sample.size();

				it = sample.begin();
				clock_t time_start_2 = clock();
				for (int i = 0; i < cyc; ++i) {
					key_type ms = *it;
					++it;
					sum2 += hole.query_successor(ms, successor1, sum2);
				}
				clock_t time_end_2 = clock();
				long time3 = time_end_2 - time_start_2;
				fout << "ours average time: " << (double)time3 / (double)cyc << endl;
				ours_average += (double)time3 / (double)cyc;

				cout << "ours end" << endl;
			}
			ours_average = ours_average / (double)mul;
			wind_average = wind_average / (double)mul;
			fout << "Wind Average Average time: " << wind_average << endl;
			fout << "Ours Average Average time: " << ours_average << endl;
		}
	}

	cout << "sum1: " << sum1 << endl;
	for (int ii = 0; ii < sum1; ++ii)
		fr1 << successor[ii] << endl;
	cout << "sum2: " << sum2 << endl;
	for (int ii = 0; ii < sum2; ++ii)
		fr2 << successor1[ii] << endl;
	fout.close();
	fr1.close();
	fr2.close();
	delete[] EE;
}

void test_time_pre() {
	chain* tempC = new chain();
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int beg = 600000;
	int inter = 800000 - 600000;
	int sample_num = 5000;
	Edge* EE = read_NotreDame_data(&cnt, 100000000);
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	ofstream fout;
	ofstream fr1;
	ofstream fr2;
	fout.open("test_time_pre.txt", ios::out);
	fr1.open("fr1.txt", ios::out);
	fr2.open("fr2.txt", ios::out);

	Chandelier* wind;
	holemerge hole(bl1, bl2, 0, 221);
	wind = new Chandelier(csl);

	int inum = 0;
	int sum1 = 0;
	int sum2 = 0;
	for (int it = 0; it < cnt; ++it) {
		if (ed.count(EE[it]))continue;
		ed.insert(EE[it]);
		chain* c = new chain();
		c->src = EE[it].src;
		c->dst = EE[it].dst;
		c->pointer_chand = nullptr;
		wind->insert(c);

		Edge he;
		he.src = EE[it].dst;
		he.dst = EE[it].src;
		hole.insert(he);
		edg.insert(EE[it].dst);
		inum = ed.size();
		if ((inum >= beg) && (((inum - beg) % inter) == 0)) {
			fout << "inum: " << inum << endl;
			cout << "hole.state: " << hole.state << endl;
			double wind_average = 0;
			double ours_average = 0;
			cout << "distinct dst num: " << edg.size() << endl;
			for (int mm = 0; mm < mul; ++mm) {
				set<key_type> sample;
				while (sample.size() != sample_num) {
					int r = rand() % edg.size();
					auto point = edg.begin();
					for (int rrr = 0; rrr < r; ++rrr) {
						++point;
					}
					sample.insert(*point);
				}
				for (auto y = sample.begin(); y != sample.end(); ++y) {
					need.push(*y);
				}
				auto it = sample.begin();
				int cyc = sample.size();
				clock_t time_start = clock();
				for (int i = 0; i < cyc; ++i) {
					key_type ms = *it;
					++it;
					sum1 += wind->query_precursor(ms, precursor, sum1);
				}
				clock_t time_end = clock();
				long time = time_end - time_start;
				fout << "wind average time: " << (double)time / (double)cyc << endl;
				wind_average += (double)time / (double)cyc;

				cout << "wind end" << endl;


				it = sample.begin();
				clock_t time_start_2 = clock();
				for (int i = 0; i < cyc; ++i) {
					key_type ms = *it;
					++it;
					sum2 += hole.query_successor(ms, precursor1, sum2);
				}
				clock_t time_end_2 = clock();
				long time3 = time_end_2 - time_start_2;
				fout << "ours average time: " << (double)time3 / (double)cyc << endl;
				ours_average += (double)time3 / (double)cyc;

				cout << "ours end" << endl;
			}
			ours_average = ours_average / (double)mul;
			wind_average = wind_average / (double)mul;
			fout << "Wind Average Average time: " << wind_average << endl;
			fout << "Ours Average Average time: " << ours_average << endl;
		}
	}



	cout << "sum1: " << sum1 << endl;
	for (int ii = 0; ii < sum1; ++ii)
		fr1 << precursor[ii] << endl;
	cout << "sum2: " << sum2 << endl;
	for (int ii = 0; ii < sum2; ++ii)
		fr2 << precursor1[ii] << endl;
	fout.close();
	fr1.close();
	fr2.close();
	delete[] EE;
	delete wind;
}

void test_time_pre_co() {
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int beg = 600000;
	int inter = 800000 - 600000;
	int sample_num = 5000;
	Edge* EE = read_NotreDame_data(&cnt, 100000000);
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	ofstream fout;
	ofstream fr1;
	ofstream fr2;
	fout.open("test_time_pre_co.txt", ios::out);
	fr1.open("fr1_co.txt", ios::out);
	fr2.open("fr2_co.txt", ios::out);

	holemerge hole(32600, 16300, 0, 221);

	int inum = 0;
	int sum1 = 0;
	int sum2 = 0;
	for (int it = 0; it < cnt; ++it) {
		if (ed.count(EE[it]))continue;
		ed.insert(EE[it]);


		Edge he;
		he.src = EE[it].dst;
		he.dst = EE[it].src;
		hole.insert(he);
		edg.insert(EE[it].dst);
		inum = ed.size();
		if ((inum >= beg) && (((inum - beg) % inter) == 0)) {
			fout << "inum: " << inum << endl;
			cout << "hole.state: " << hole.state << endl;
			double wind_average = 0;
			double ours_average = 0;
			cout << "distinct dst num: " << edg.size() << endl;
			for (int mm = 0; mm < mul; ++mm) {
				set<key_type> sample;
				for (int iii = 0; iii < sample_num; ++iii) {
					sample.insert(need.front());
					need.pop();
				}
				auto it = sample.begin();
				int cyc = sample.size();
				it = sample.begin();
				clock_t time_start_2 = clock();
				for (int i = 0; i < cyc; ++i) {
					key_type ms = *it;
					++it;
					sum2 += hole.query_successor(ms, precursor1, sum2);
				}
				clock_t time_end_2 = clock();
				long time3 = time_end_2 - time_start_2;
				fout << "ours average time: " << (double)time3 / (double)cyc << endl;
				ours_average += (double)time3 / (double)cyc;

				cout << "ours end" << endl;
			}
			ours_average = ours_average / (double)mul;
			wind_average = wind_average / (double)mul;
			fout << "Wind Average Average time: " << wind_average << endl;
			fout << "Ours Average Average time: " << ours_average << endl;
		}
	}

	cout << "sum1: " << sum1 << endl;
	for (int ii = 0; ii < sum1; ++ii)
		fr1 << precursor[ii] << endl;
	cout << "sum2: " << sum2 << endl;
	for (int ii = 0; ii < sum2; ++ii)
		fr2 << precursor1[ii] << endl;
	fout.close();
	fr1.close();
	fr2.close();
	delete[] EE;
}

void test_sssp() {
	chain* tempC = new chain();
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int maxtopk = 100;
	int rd = 10;
	key_type topk[10001] = { 0 };
	Edge* EE = read_NotreDame_data(&cnt, 100000000);
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	unordered_map<key_type, key_type> node;

	ofstream fout;
	ofstream fr1;
	ofstream fr2;
	fout.open("test_sssp.txt", ios::out);
	fr1.open("fr1.txt", ios::out);
	fr2.open("fr2.txt", ios::out);

	Chandelier* wind;
	holemerge hole(bl1, bl2, 0, 221);
	wind = new Chandelier(csl);

	for (int it = 0; it < cnt; ++it) {
		if (ed.count(EE[it]))continue;
		ed.insert(EE[it]);
		chain* c = new chain();
		c->src = EE[it].src;
		c->dst = EE[it].dst;
		c->pointer_chand = nullptr;
		wind->insert(c);
		hole.insert(EE[it]);
		edg.insert(EE[it].dst);
		edg.insert(EE[it].src);
		if (node.find(EE[it].src) != node.end()) node[EE[it].src] = node[EE[it].src] + 1;
		else node.insert(pair<key_type, key_type>(EE[it].src, 1));
		if (node.find(EE[it].dst) != node.end()) node[EE[it].dst] = node[EE[it].dst] + 1;
		else node.insert(pair<key_type, key_type>(EE[it].dst, 1));
	}

	vector<Node_degree> nd;
	for (auto it = node.begin(); it != node.end(); ++it) {
		Node_degree dd;
		dd.node = it->first;
		dd.val = it->second;
		nd.push_back(dd);
	}
	make_heap(nd.begin(), nd.end(), Compare);
	for (int i = 0; i < maxtopk; ++i) {
		topk[i] = nd[0].node;
		pop_heap(nd.begin(), nd.end(), Compare);
		nd.pop_back();
	}
	unordered_map<key_type, bool>flag;
	unordered_map<key_type, int> dist;
	int INF = 2147483647;




	long time = 0;
	for (int i = 0; i < rd; ++i) {
		flag.clear();
		dist.clear();
		clock_t time_start = clock();
		key_type vs = topk[i];
		key_type k;
		int tmp;
		for (auto it = edg.begin(); it != edg.end(); ++it) {
			flag.insert(pair<key_type, bool>(*it, false));
			tempC->src = vs;
			tempC->dst = *it;
			tempC->pointer_chand = nullptr;
			int mmm;
			if (wind->query_edge(tempC))mmm = 1;
			else mmm = INF;
			dist.insert(pair<key_type, int>(*it, mmm));
		}
		flag[vs] = true;
		dist[vs] = 0;
		for (int ii = 1; ii < edg.size(); ++ii) {
			int min = INF;
			bool fff = false;
			for (auto it = edg.begin(); it != edg.end(); ++it) {
				if (flag[*it] == false && (fff == false || dist[*it] < min))
				{
					min = dist[*it];
					k = *it;
					fff = true;
				}
			}
			flag[k] = true;
			if (min == INF)break;
			for (auto it = edg.begin(); it != edg.end(); ++it) {
				tempC->src = k;
				tempC->dst = *it;
				tempC->pointer_chand = nullptr;
				if (wind->query_edge(tempC))tmp = min + 1;
				else tmp = INF;
				if (flag[*it] == false && (tmp < dist[*it]))
				{
					dist[*it] = tmp;
				}
			}
		}
		clock_t time_end = clock();
		time = time + (time_end - time_start);
		for (auto it = dist.begin(); it != dist.end(); ++it)
			fr1 << it->first << " " << it->second << endl;
	}
	fout << "wind average time: " << (double)time / (double)rd << endl;
	cout << "wind end" << endl;




	long time2 = 0;
	for (int i = 0; i < rd; ++i) {
		flag.clear();
		dist.clear();
		clock_t time_start2 = clock();
		key_type vs = topk[i];
		key_type k;
		int tmp;
		for (auto it = edg.begin(); it != edg.end(); ++it) {
			flag.insert(pair<key_type, bool>(*it, false));
			Edge e;
			e.src = vs;
			e.dst = *it;
			int mmm;
			if (hole.query(e))mmm = 1;
			else mmm = INF;
			dist.insert(pair<key_type, int>(*it, mmm));
		}
		flag[vs] = true;
		dist[vs] = 0;
		for (int ii = 1; ii < edg.size(); ++ii) {
			int min = INF;
			bool fff = false;
			for (auto it = edg.begin(); it != edg.end(); ++it) {
				if (flag[*it] == false && (fff == false || dist[*it] < min))
				{
					min = dist[*it];
					k = *it;
					fff = true;
				}
			}
			flag[k] = true;
			if (min == INF)break;
			for (auto it = edg.begin(); it != edg.end(); ++it) {
				Edge e;
				e.src = k;
				e.dst = *it;
				if (hole.query(e))tmp = min + 1;
				else tmp = INF;
				if (flag[*it] == false && (tmp < dist[*it]))
				{
					dist[*it] = tmp;
				}
			}
		}
		clock_t time_end2 = clock();
		time2 = time2 + (time_end2 - time_start2);
		for (auto it = dist.begin(); it != dist.end(); ++it)
			fr2 << it->first << " " << it->second << endl;
	}
	fout << "ours average time: " << (double)time2 / (double)rd << endl;
	cout << "ours end" << endl;

	fout.close();
	fr1.close();
	fr2.close();
	delete[] EE;
	delete wind;
}

void test_sssp_new() {
	chain* tempC = new chain();
	int csl = 400;
	int L1 = 16000;
	int L2 = L1 / 2;
	count_type cnt;
	int mul = 3;
	int maxtopk = 4000;
	int rd = 10;
	key_type topk[10001] = { 0 };
	Edge* EE = read_NotreDame_data(&cnt, 100000000);
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	unordered_map<key_type, key_type> node;

	ofstream fout;
	ofstream fr1;
	ofstream fr2;
	ofstream fr3;
	fout.open("test_sssp.txt", ios::out);
	fr1.open("fr1.txt", ios::out);
	fr2.open("fr2.txt", ios::out);
	fr3.open("fr3.txt", ios::out);

	Chandelier* wind;
	holemerge hole(bl1, bl2, 0, 221);
	wind = new Chandelier(csl);

	for (int it = 0; it < cnt; ++it) {
		if (ed.count(EE[it]))continue;
		ed.insert(EE[it]);
		chain* c = new chain();
		c->src = EE[it].src;
		c->dst = EE[it].dst;
		c->pointer_chand = nullptr;
		wind->insert(c);
		hole.insert(EE[it]);
		edg.insert(EE[it].dst);
		edg.insert(EE[it].src);
		if (node.find(EE[it].src) != node.end()) node[EE[it].src] = node[EE[it].src] + 1;
		else node.insert(pair<key_type, key_type>(EE[it].src, 1));
		if (node.find(EE[it].dst) != node.end()) node[EE[it].dst] = node[EE[it].dst] + 1;
		else node.insert(pair<key_type, key_type>(EE[it].dst, 1));
	}

	vector<Node_degree> nd;
	for (auto it = node.begin(); it != node.end(); ++it) {
		Node_degree dd;
		dd.node = it->first;
		dd.val = it->second;
		nd.push_back(dd);
	}
	make_heap(nd.begin(), nd.end(), Compare);
	for (int i = 0; i < maxtopk; ++i) {
		topk[i] = nd[0].node;
		pop_heap(nd.begin(), nd.end(), Compare);
		nd.pop_back();
	}
	unordered_map<key_type, bool>flag;
	unordered_map<key_type, int> dist;
	int INF = 2147483647;


	for (int TT = 1; TT <= 8; ++TT) {
		int num = 500 * TT;
		fout << "num:     " << num << endl;
		double average;

		average = 0;
		for (int hh = 0; hh < mul; ++hh) {
			long time = 0;
			for (int i = 0; i < rd; ++i) {
				flag.clear();
				dist.clear();
				clock_t time_start = clock();
				key_type vs = topk[i];
				key_type k;
				int tmp;
				for (int it = 0; it < num; ++it) {
					flag.insert(pair<key_type, bool>(topk[it], false));
					tempC->src = vs;
					tempC->dst = topk[it];
					tempC->pointer_chand = nullptr;
					int mmm;
					if (wind->query_edge(tempC))mmm = 1;
					else mmm = INF;
					dist.insert(pair<key_type, int>(topk[it], mmm));
				}
				flag[vs] = true;
				dist[vs] = 0;
				for (int ii = 1; ii < num; ++ii) {
					int min = INF;
					bool fff = false;
					for (int it = 0; it < num; ++it) {
						if (flag[topk[it]] == false && (fff == false || dist[topk[it]] < min))
						{
							min = dist[topk[it]];
							k = topk[it];
							fff = true;
						}
					}
					flag[k] = true;
					if (min == INF)break;
					for (int it = 0; it < num; ++it) {
						tempC->src = k;
						tempC->dst = topk[it];
						tempC->pointer_chand = nullptr;
						if (wind->query_edge(tempC))tmp = min + 1;
						else tmp = INF;
						if (flag[topk[it]] == false && (tmp < dist[topk[it]]))
						{
							dist[topk[it]] = tmp;
						}
					}
				}
				clock_t time_end = clock();
				time = time + (time_end - time_start);
				for (auto it = dist.begin(); it != dist.end(); ++it)
					fr1 << it->first << " " << it->second << endl;
			}
			fout << "wind average time: " << (double)time / (double)rd << endl;
			average = average + (double)time / (double)rd;
		}
		average = average / (double)mul;
		fout << "average wind average time: " << average << endl;
		cout << "wind end" << endl;



		average = 0;
		for (int hh = 0; hh < mul; ++hh) {
			long time2 = 0;
			for (int i = 0; i < rd; ++i) {
				flag.clear();
				dist.clear();
				clock_t time_start2 = clock();
				key_type vs = topk[i];
				key_type k;
				int tmp;
				for (int it = 0; it < num; ++it) {
					flag.insert(pair<key_type, bool>(topk[it], false));
					Edge e;
					e.src = vs;
					e.dst = topk[it];
					int mmm;
					if (hole.query(e))mmm = 1;
					else mmm = INF;
					dist.insert(pair<key_type, int>(topk[it], mmm));
				}
				flag[vs] = true;
				dist[vs] = 0;
				for (int ii = 1; ii < num; ++ii) {
					int min = INF;
					bool fff = false;
					for (int it = 0; it < num; ++it) {
						if (flag[topk[it]] == false && (fff == false || dist[topk[it]] < min))
						{
							min = dist[topk[it]];
							k = topk[it];
							fff = true;
						}
					}
					flag[k] = true;
					if (min == INF)break;
					for (int it = 0; it < num; ++it) {
						Edge e;
						e.src = k;
						e.dst = topk[it];
						if (hole.query(e))tmp = min + 1;
						else tmp = INF;
						if (flag[topk[it]] == false && (tmp < dist[topk[it]]))
						{
							dist[topk[it]] = tmp;
						}
					}
				}
				clock_t time_end2 = clock();
				time2 = time2 + (time_end2 - time_start2);
				for (auto it = dist.begin(); it != dist.end(); ++it)
					fr2 << it->first << " " << it->second << endl;
			}
			fout << "ours average time: " << (double)time2 / (double)rd << endl;
			average = average + (double)time2 / (double)rd;
		}
		average = average / (double)mul;
		fout << "average ours average time: " << average << endl;
		cout << "ours end" << endl;
	}




	sblacklist.clear();
	ed.clear();
	holemerge hole2(L1, L2, 0, 221);
	for (int it = 0; it < cnt; ++it) {
		if (ed.count(EE[it]))continue;
		ed.insert(EE[it]);
		hole2.insert(EE[it]);
	}
	cout << "hole2.state:  " << hole2.state << endl;
	for (int TT = 1; TT <= 8; ++TT) {
		int num = 500 * TT;
		fout << "num:     " << num << endl;
		double average;
		average = 0;
		for (int hh = 0; hh < mul; ++hh) {
			long time3 = 0;
			for (int i = 0; i < rd; ++i) {
				flag.clear();
				dist.clear();
				clock_t time_start3 = clock();
				key_type vs = topk[i];
				key_type k;
				int tmp;
				for (int it = 0; it < num; ++it) {
					flag.insert(pair<key_type, bool>(topk[it], false));
					Edge e;
					e.src = vs;
					e.dst = topk[it];
					int mmm;
					if (hole2.query(e))mmm = 1;
					else mmm = INF;
					dist.insert(pair<key_type, int>(topk[it], mmm));
				}
				flag[vs] = true;
				dist[vs] = 0;
				for (int ii = 1; ii < num; ++ii) {
					int min = INF;
					bool fff = false;
					for (int it = 0; it < num; ++it) {
						if (flag[topk[it]] == false && (fff == false || dist[topk[it]] < min))
						{
							min = dist[topk[it]];
							k = topk[it];
							fff = true;
						}
					}
					flag[k] = true;
					if (min == INF)break;
					for (int it = 0; it < num; ++it) {
						Edge e;
						e.src = k;
						e.dst = topk[it];
						if (hole2.query(e))tmp = min + 1;
						else tmp = INF;
						if (flag[topk[it]] == false && (tmp < dist[topk[it]]))
						{
							dist[topk[it]] = tmp;
						}
					}
				}
				clock_t time_end3 = clock();
				time3 = time3 + (time_end3 - time_start3);
				for (auto it = dist.begin(); it != dist.end(); ++it)
					fr3 << it->first << " " << it->second << endl;
			}
			fout << "NT hole average time: " << (double)time3 / (double)rd << endl;
			average = average + (double)time3 / (double)rd;
		}
		average = average / (double)mul;
		fout << "average NT hole average time: " << average << endl;
		cout << "NT hole end" << endl;
	}
	fout.close();
	fr1.close();
	fr2.close();
	fr3.close();
	delete[] EE;
	delete wind;
}

void test_merge_cost() {
	count_type cnt;
	int mul = 10;
	bSlot use;
	Cuckoo* c1;
	Cuckoo* c2;
	ofstream fout;
	ofstream fr1;
	fr1.open("fr1.txt", ios::out);
	fout.open("test_merge_time.txt", ios::out);
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* EE = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&EE[i], &items[i], sizeof(Edge));
	std::cout << "The number of packet:" << cnt << std::endl;
	set<key_type> node; node.clear();
	for (int i = 0; i < cnt; ++i) {
		node.insert(EE[i].dst);
		node.insert(EE[i].src);
	}
	for (int L = 4; L <= 1024 * 16; L = L * 4) {
		long time = 0;
		int tt = 0;
		for (int mm = 0; mm < mul; ++mm) {
			sblacklist.clear();
			c1 = new Cuckoo(L, L / 2, us1, us2);
			c2 = new Cuckoo(L, L / 2, us1, us2);
			for (auto it = node.begin(); it != node.end(); ) {
				if (c1->num < (c1->len1 + c1->len2) * bup * rate)
				{
					c1->insert(*it); ++it;
				}
				if (it == node.end()) { cout << "false!" << endl; break; }
				if (c2->num < (c2->len1 + c2->len2) * bup * rate)
				{
					c2->insert(*it); ++it;
				}
				if ((c1->num >= (c1->len1 + c1->len2) * bup * rate) && (c2->num >= (c2->len1 + c2->len2) * bup * rate)) {
					++tt;
					clock_t time_start = clock();
					Cuckoo* c = use.merge(c1, c2);
					clock_t time_end = clock();
					time = time + (time_end - time_start);
					for (int j = 0; j < L * 2; ++j)
						for (int k = 0; k < bup; ++k)
						{
							fr1 << c->layer1.layer[j].bslot[k];
						}
					for (int j = 0; j < L; ++j)
						for (int k = 0; k < bup; ++k) {
							fr1 << c->layer2.layer[j].bslot[k];
						}
					delete c;
					break;
				}
			}
		}
		double average = (double)time / (double)mul;
		if (tt != mul) { cout << "false" << endl; }
		assert(tt == mul);
		fout << "L=" << L << " :" << endl;
		fout << "average merge time:  " << average << endl;
	}
}
int main()
{
	srand(time(NULL));
	exit(0);
}


