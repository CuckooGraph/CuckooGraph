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
#include<map>
#include<unordered_map>
#include"BobHash.h"
#include"Util.h"
#include"Cuckoo.h"
#include"WindBell.h"
#include"GSS.h"
#include"Auxo.h"
#include"tcm.h"
#define lt 3
#define rate 0.9
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
#define minc1 8
#define minc2 4
#define cthres 0.35
#define bminc1 8
#define bminc2 4
#define bcthres 0.35
#define sbl 128
#define bbl 128
#define million 1000000
static int hole_del = 0;
static int big_del = 0;
static int BIG_DEL = 0;
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
const string folder = "/share/datasets/CAIDA2018/dataset/";
const string filenames[] = { "130000.dat" ,"130100.dat" , "130200.dat" , "130300.dat" , "130400.dat" , "130600.dat" , "130700.dat" , "130800.dat" , "130900.dat" , "131000.dat" , "131100.dat" , "131200.dat" ,
"131300.dat" , "131400.dat" , "131500.dat" , "131600.dat" , "131700.dat", "131800.dat" , "131900.dat" , "132000.dat" , "132100.dat" , "132200.dat" , "132300.dat" , "132400.dat" , "132500.dat" ,
 "132600.dat" , "132700.dat" , "132800.dat" ,  "132900.dat" , "133000.dat" , "133100.dat" , "133200.dat" , "133300.dat" , "133400.dat" ,  "133500.dat" , "133600.dat" , "133700.dat" ,
"133800.dat" , "133900.dat" , "134000.dat" ,  "134100.dat" , "134200.dat" , "134300.dat" , "134400.dat" , "134500.dat" , "134600.dat" ,  "134700.dat" , "134800.dat" , "134900.dat" ,
"135000.dat" , "135100.dat" , "135200.dat" ,  "135300.dat" , "135400.dat" , "135500.dat" ,"135600.dat" ,  "135700.dat" , "135800.dat" , "135900.dat" };
Edge* read_caida_data(count_type* cnt_, const count_type length = 100000000) {
	count_type cnt = load_data13_CAIDA();
	unordered_map<key_type, key_type> mp;
	int node_num = 0;
	for (int it = 0; it < cnt; ++it) {
		if (!mp.count(asrc[it])) {
			++node_num;
			mp.insert({ asrc[it], node_num });
		}
		if (!mp.count(adst[it])) {
			++node_num;
			mp.insert({ adst[it], node_num });
		}
	}
	cout << "caida node num:  " << node_num << endl;
	for (int it = 0; it < cnt; ++it) {
		asrc[it] = mp[asrc[it]];
		adst[it] = mp[adst[it]];
	}
	Edge* items = new Edge[length];
	for (int it = 0; it < cnt; ++it) {
		items[it].src = asrc[it];
		items[it].dst = adst[it];
	}
	(*cnt_) = cnt;
	cout << "CAIDA count: " << cnt << endl;
	return items;
}
Edge* read_wiki_data(count_type* cnt, const count_type length = 100000000) {
	Edge* items = new Edge[length];
	Edge* it = items;
	int intbig = 2147483647;
	ifstream infile;
	infile.open("/root/out.wiki_talk_en");
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
	infile.open("/root/sx-stackoverflow.txt");
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
	infile.open("/root/web-NotreDame.txt");
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
void split(const std::string& s, char delimiter, std::vector<std::string>& v)
{
	std::string::size_type i = 0;
	std::string::size_type j = s.find(delimiter);
	while (j != std::string::npos)
	{
		v.push_back(s.substr(i, j - i));
		i = ++j;
		j = s.find(delimiter, i);
	}
	if (j == std::string::npos)
		v.push_back(s.substr(i, s.length()));
}
Edge* read_sina_data(count_type* cnt, count_type length = 1000000000) {
	Edge* items = new Edge[length];
	Edge* it = items;
	int intbig = 2147483647;
	ifstream infile;
	infile.open("/root/soc-sinaweibo.mtx");
	int i = 0;
	int strange = 0;
	while (1) {
		string src;
		if (!getline(infile, src)) break;
		if (src[0] < '0' || src[0]>'9')continue;
		vector<string> aaa;
		split(src, ' ', aaa);
		if (aaa.size() == 2) {
			key_type u = stoi(aaa[0]);
			key_type v = stoi(aaa[1]);
			items[i].src = u;
			items[i].dst = v;
		}
		else if (aaa.size() >= 3) {
			++strange;
			key_type u = stoi(aaa[1]);
			key_type v = stoi(aaa[2]);
			items[i].src = u;
			items[i].dst = v;
		}
		++i;
	}
	(*cnt) = i;
	cout << "i:  " << i << endl;
	cout << "strange:  " << strange << endl;
	infile.close();
	return items;
}
void read_sina_data_test() {
	ifstream infile;
	infile.open("/root/soc-sinaweibo.mtx");
	for (int i = 1; i <= 30; ++i) {
		string src;
		getline(infile, src);
		cout << i << "  src:  " << endl;
		cout << src << endl;
		if (i >= 2) {
			vector<string> aaa;
			split(src, ' ', aaa);
			cout << "size: " << aaa.size() << endl;
			if (aaa.size() == 2) {
				key_type u = stoi(aaa[0]);
				key_type v = stoi(aaa[1]);
				cout << "uv:  " << u << " " << v << endl;
			}
			else if (aaa.size() >= 3) {
				key_type u = stoi(aaa[1]);
				key_type v = stoi(aaa[2]);
				cout << "uv:  " << u << " " << v << endl;
				cout << "strange: " << aaa[0] << endl;
			}
		}
	}
	infile.close();
}
Edge* read_CuckooGraph_data(count_type* cnt, const count_type length = 100000000) {
	Edge* items = new Edge[length];
	Edge* it = items;
	int intbig = 2147483647;
	ifstream infile;
	infile.open("/root/Cuckoo/0126kddbeifen1/CuckooGraphDatasetNewNew.txt");
	int i = 0;
	while (1) {
		string src;
		string dst;
		if (!getline(infile, src, ',')) break;
		getline(infile, dst);
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
Edge* read_sparse_data(count_type* cnt, const count_type length = 100000000) {
	Edge* items = new Edge[length];
	Edge* it = items;
	int intbig = 2147483647;
	ifstream infile;
	infile.open("/root/Cuckoo/kdd_rebuttal/sparse/240407bf1/Sparse.txt");
	int i = 0;
	while (1) {
		string src;
		string dst;
		if (!getline(infile, src, ',')) break;
		getline(infile, dst);
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
void test(int uy1, int uy2) {
	count_type cnt = 100 * 1024 * 1024;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i) {
		edge[i].src = i + 3;
		edge[i].dst = i + 9;
	}
	Cuckoo cuckoo(uy1, uy2, 0, 99999);
	int sum = 0;
	for (int i = 0; i < cnt; ++i) {
		if (!(cuckoo.insert(edge[i].src)))
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
			double ans = s / total;
			cout << "Rate:　　" << ans << "  l1:l2　 :   " << uy1 << ":" << uy2 << endl;
			delete[] edge;
			return;
		}
	}
}
class mergehash1 {
public:
	Cuckoo* link[link_num];
	int ll1, ll2;
	int s1, s2;
	vector<key_type> blacklist;
	mergehash1(int _l1, int _l2, int _s1, int _s2) {
		for (int i = 0; i < link_num; ++i)
			link[i] = NULL;
		s1 = _s1;
		s2 = _s2;
		ll1 = _l1;
		ll2 = _l2;
	}
	~mergehash1() {
		for (int i = 0; i < link_num; ++i)
		{
			if (link[i] != NULL)
				delete link[i];
		}
		blacklist.clear();
	}
	double test_rate() {
		double sum = blacklist.size();
		double num = blacklist.size();
		for (int i = 0; i < link_num; ++i)
		{
			if (link[i] != NULL)
			{
				sum = sum + ((link[i]->len1) + (link[i]->len2)) * bup;
				num = num + link[i]->num;
			}
		}
		return num / sum;
	}
	Cuckoo* expand(Cuckoo* c1) {
		int l1 = c1->len1;
		int l2 = c1->len2;
		Cuckoo* c = new Cuckoo(l1 * 2, l2 * 2, s1, s2);
		memcpy(c->layer1.layer, c1->layer1.layer, l1 * sizeof(Bucket));
		memcpy(c->layer1.layer + l1, c1->layer1.layer, l1 * sizeof(Bucket));
		memcpy(c->layer2.layer, c1->layer2.layer, l2 * sizeof(Bucket));
		memcpy(c->layer2.layer + l2, c1->layer2.layer, l2 * sizeof(Bucket));
		c->num = c1->num;
		for (int i = 0; i < l1; ++i) {
			int k = i + l1;
			for (int j = 0; j < bup; ++j) {
				if (c->layer1.layer[i].bslot[j] == 0)continue;
				if (i != (int)(mmhash(c->layer1.layer[i].bslot[j], s1) % (uint32_t)(l1 * 2)))
					c->layer1.layer[i].bslot[j] = 0;
				else c->layer1.layer[k].bslot[j] = 0;
			}
		}
		for (int i = 0; i < l2; ++i) {
			int k = i + l2;
			for (int j = 0; j < bup; ++j) {
				if (c->layer2.layer[i].bslot[j] == 0)continue;
				if (i != (int)(mmhash(c->layer2.layer[i].bslot[j], s2) % (uint32_t)(l2 * 2)))
					c->layer2.layer[i].bslot[j] = 0;
				else c->layer2.layer[k].bslot[j] = 0;
			}
		}
		delete c1;
		return c;
	}
	bool insert(key_type e) {
		if (query(e)) return true;
		double tr = 2;
		int aim = 0;
		int tnull = -1;
		bool flag = false;
		for (int i = 0; i < link_num; ++i)
		{
			if (link[i] != NULL)
			{
				double tn = link[i]->num;
				double sum = (link[i]->len1 + link[i]->len2) * bup;
				double ir = (tn / sum);
				if (ir < tr) {
					tr = ir;
					aim = i;
				}
			}
			else if (flag == false) {
				flag = true;
				tnull = i;
			}
		}
		if (tr < rate) {
			if (!(link[aim]->insert(e))) blacklist.push_back(mykick);
			return true;
		}
		if (tnull >= 0) {
			int _l1 = ll1;
			int _l2 = ll2;
			for (int ww = 0; ww < tnull; ++ww) {
				_l1 = _l1 / 2;
				_l2 = _l2 / 2;
			}
			link[tnull] = new Cuckoo(_l1, _l2, s1, s2);
			queue<key_type> qq;
			for (auto it = blacklist.begin(); it != blacklist.end(); ++it) {
				if (!(link[tnull]->insert(*it)))qq.push(mykick);
			}
			blacklist.clear();
			while (!qq.empty()) {
				blacklist.push_back(qq.front());
				qq.pop();
			}
			if (!(link[tnull]->insert(e))) blacklist.push_back(mykick);
			return true;
		}
		int small = link[0]->len1;
		aim = 0;
		for (int i = 1; i < link_num; ++i) {
			if (link[i]->len1 < small) {
				small = link[i]->len1;
				aim = i;
			}
		}
		link[aim] = expand(link[aim]);
		queue<key_type> qq;
		for (auto it = blacklist.begin(); it != blacklist.end(); ++it) {
			if (!(link[aim]->insert(*it)))qq.push(mykick);
		}
		blacklist.clear();
		while (!qq.empty()) {
			blacklist.push_back(qq.front());
			qq.pop();
		}
		if (!(link[aim]->insert(e))) blacklist.push_back(mykick);
		return true;
	}
	bool query(key_type e) {
		for (int i = 0; i < link_num; ++i)
		{
			if (link[i] != NULL)
			{
				if (link[i]->query(e))
					return true;
			}
		}
		for (auto it = blacklist.begin(); it != blacklist.end(); ++it) {
			if (*it == e)
				return true;
		}
		return false;
	}
};
class wEdge {
public:
	key_type src;
	key_type dst;
	count_type weight;
	wEdge() {
		weight = 1;
	}
};
static vector<wEdge> sblacklist;
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
	Cuckoo* merge(Cuckoo* c0, Cuckoo* c1, Cuckoo* c2) {
		int l1 = c0->len1;
		int l2 = c0->len2;
		queue<key_type> mq;
		queue<count_type> mq_w;
		Cuckoo* c = new Cuckoo(l1 * 2, l2 * 2, us1, us2);
		memcpy(c->layer1.layer, c0->layer1.layer, l1 * sizeof(Bucket));
		memcpy(c->layer1.layer + l1, c0->layer1.layer, l1 * sizeof(Bucket));
		memcpy(c->layer2.layer, c0->layer2.layer, l2 * sizeof(Bucket));
		memcpy(c->layer2.layer + l2, c0->layer2.layer, l2 * sizeof(Bucket));
		c->num = c0->num;
		for (int i = 0; i < l1; ++i) {
			int k = i + l1;
			for (int j = 0; j < bup; ++j) {
				if (c->layer1.layer[i].bslot[j] == 0)continue;
				if (i != (int)(mmhash(c->layer1.layer[i].bslot[j], us1) % (uint32_t)(l1 * 2)))
				{
					c->layer1.layer[i].bslot[j] = 0;
					c->layer1.layer[i].weight[j] = 0;
				}
				else {
					c->layer1.layer[k].bslot[j] = 0;
					c->layer1.layer[k].weight[j] = 0;
				}
			}
		}
		for (int i = 0; i < l1 / 2; ++i) {
			for (int j = 0; j < bup; ++j) {
				if (c1->layer1.layer[i].bslot[j] == 0)continue;
				if ((c->layer1.layer[(mmhash(c1->layer1.layer[i].bslot[j], us1) % (uint32_t)(l1 * 2))].insert(c1->layer1.layer[i].bslot[j], c1->layer1.layer[i].weight[j])) == 0) {
					mq.push(c1->layer1.layer[i].bslot[j]);
					mq_w.push(c1->layer1.layer[i].weight[j]);
				}
				else
					c->num = (c->num) + 1;
			}
			for (int j = 0; j < bup; ++j) {
				if (c2->layer1.layer[i].bslot[j] == 0)continue;
				if ((c->layer1.layer[(mmhash(c2->layer1.layer[i].bslot[j], us1) % (uint32_t)(l1 * 2))].insert(c2->layer1.layer[i].bslot[j], c2->layer1.layer[i].weight[j])) == 0) {
					mq.push(c2->layer1.layer[i].bslot[j]);
					mq_w.push(c2->layer1.layer[i].weight[j]);
				}
				else
					c->num = (c->num) + 1;
			}
		}
		for (int i = 0; i < l2; ++i) {
			int k = i + l2;
			for (int j = 0; j < bup; ++j) {
				if (c->layer2.layer[i].bslot[j] == 0)continue;
				if (i != (int)(mmhash(c->layer2.layer[i].bslot[j], us2) % (uint32_t)(l2 * 2))) {
					c->layer2.layer[i].bslot[j] = 0;
					c->layer2.layer[i].weight[j] = 0;
				}
				else {
					c->layer2.layer[k].bslot[j] = 0;
					c->layer2.layer[k].weight[j] = 0;
				}
			}
		}
		for (int i = 0; i < l2 / 2; ++i) {
			for (int j = 0; j < bup; ++j) {
				if (c1->layer2.layer[i].bslot[j] == 0)continue;
				if ((c->layer2.layer[(mmhash(c1->layer2.layer[i].bslot[j], us2) % (uint32_t)(l2 * 2))].insert(c1->layer2.layer[i].bslot[j], c1->layer2.layer[i].weight[j])) == 0) {
					mq.push(c1->layer2.layer[i].bslot[j]);
					mq_w.push(c1->layer2.layer[i].weight[j]);
				}
				else
					c->num = (c->num) + 1;
			}
			for (int j = 0; j < bup; ++j) {
				if (c2->layer2.layer[i].bslot[j] == 0)continue;
				if ((c->layer2.layer[(mmhash(c2->layer2.layer[i].bslot[j], us2) % (uint32_t)(l2 * 2))].insert(c2->layer2.layer[i].bslot[j], c2->layer2.layer[i].weight[j])) == 0) {
					mq.push(c2->layer2.layer[i].bslot[j]);
					mq_w.push(c2->layer2.layer[i].weight[j]);
				}
				else
					c->num = (c->num) + 1;
			}
		}
		while (!mq.empty()) {
			if (!c->insert(mq.front(), mq_w.front())) {
				wEdge now_insert;
				now_insert.src = src;
				now_insert.dst = mykick;
				now_insert.weight = mykick_w;
				sblacklist.push_back(now_insert);
			}
			mq.pop();
			mq_w.pop();
		}
		delete c0;
		delete c1;
		delete c2;
		return c;
	}
	int query_insert(Edge e, count_type weight = 1) {
		if (e.src == debug_src && e.dst == debug_dst)++debug;
		if (e.src != src) {
			return 0;
		}
		if (pt < 0) {
			for (int i = 0; i < 3; ++i) {
				if (slot[i] != NULL && slot[i]->query_insert(e.dst, weight))return i + 9;
			}
			for (auto it = sblacklist.begin(); it != sblacklist.end(); ++it) {
				if ((*it).src == e.src && (*it).dst == e.dst) {
					(*it).weight = (*it).weight + 1;
					return blist;
				}
			}
			return 0;
		}
		else {
			for (int i = 0; i < 3; ++i) {
				key_type* pv = (key_type*)(&slot[i]);
				if (*pv == e.dst) {
					*(pv + 1) = *(pv + 1) + 1;
					return pt;
		}
	}
			return 0;
}
	}
	int insert(Edge EE, count_type weight = 1) {
		if (EE.src == debug_src && EE.dst == debug_dst)++debug;
		if (pt == 0) {
			src = EE.src;
			*((key_type*)slot) = EE.dst;
			key_type* pv = (key_type*)(&slot[0]);
			*(pv + 1) = weight;
			++pt;
			return pt;
		}
		else if (src != EE.src)
			return 0;
		else if (pt < 0) {
			int state = -1;
			Edge e;
			e.src = EE.src;
			e.dst = EE.dst;
			count_type e_w = weight;
			if (sblacklist.size() < sbl) {
				for (int i = 2; i >= 0; --i) {
					if (slot[i] != NULL) {
						++state;
						if ((slot[i]->num < (slot[i]->len1 + slot[i]->len2) * bup * rate)
							) {
							if (!slot[i]->insert(EE.dst, weight))
							{
								wEdge now_insert;
								now_insert.src = EE.src;
								now_insert.dst = mykick;
								now_insert.weight = mykick_w;
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
							if (!slot[i]->insert(EE.dst, weight))
							{
								fff = true;
								e.src = EE.src;
								e.dst = mykick;
								e_w = mykick_w;
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
				queue<count_type> qq_w;
				for (auto it = sblacklist.begin(); it != sblacklist.end();) {
					if ((*it).src == src) {
						if (!(slot[1]->insert((*it).dst, (*it).weight))) { qq.push(mykick); qq_w.push(mykick_w); }
						it = sblacklist.erase(it);
					}
					else ++it;
				}
				while (!qq.empty()) {
					wEdge now_insert;
					now_insert.src = src;
					now_insert.dst = qq.front();
					now_insert.weight = qq_w.front();
					sblacklist.push_back(now_insert);
					qq.pop();
					qq_w.pop();
				}
				if (!slot[1]->insert(e.dst, e_w))
				{
					wEdge now_insert;
					now_insert.src = e.src;
					now_insert.dst = mykick;
					now_insert.weight = mykick_w;
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
				queue<count_type> qq_w;
				for (auto it = sblacklist.begin(); it != sblacklist.end();) {
					if ((*it).src == src) {
						if (!(slot[2]->insert((*it).dst, (*it).weight))) { qq.push(mykick); qq_w.push(mykick_w); }
						it = sblacklist.erase(it);
					}
					else ++it;
				}
				while (!qq.empty()) {
					wEdge now_insert;
					now_insert.src = src;
					now_insert.dst = qq.front();
					now_insert.weight = qq_w.front();
					sblacklist.push_back(now_insert);
					qq.pop();
					qq_w.pop();
				}
				if (!slot[2]->insert(e.dst, e_w))
				{
					wEdge now_insert;
					now_insert.src = e.src;
					now_insert.dst = mykick;
					now_insert.weight = mykick_w;
					sblacklist.push_back(now_insert);
				}
				return 11;
			}
			if (state == 2) {
				Cuckoo* cm = merge(slot[0], slot[1], slot[2]);
				slot[1] = NULL;
				slot[2] = NULL;
				slot[0] = cm;
				slot[1] = new Cuckoo((slot[0]->len1) / 2, (slot[0]->len2) / 2, us1, us2);
#ifdef TEST_MEM
				cuckoo_mem += ((slot[0]->len1) / 2 + (slot[0]->len2) / 2) * sizeof(Bucket);
				cuckoo_mem = cuckoo_mem - sizeof(Cuckoo);
#endif
				queue<key_type> qq;
				queue<count_type> qq_w;
				for (auto it = sblacklist.begin(); it != sblacklist.end();) {
					if ((*it).src == src) {
						if (!(slot[1]->insert((*it).dst, (*it).weight))) { qq.push(mykick); qq_w.push(mykick_w); }
						it = sblacklist.erase(it);
					}
					else ++it;
				}
				while (!qq.empty()) {
					wEdge now_insert;
					now_insert.src = src;
					now_insert.dst = qq.front();
					now_insert.weight = qq_w.front();
					sblacklist.push_back(now_insert);
					qq.pop();
					qq_w.pop();
				}
				if (!slot[1]->insert(e.dst, e_w))
				{
					wEdge now_insert;
					now_insert.src = e.src;
					now_insert.dst = mykick;
					now_insert.weight = mykick_w;
					sblacklist.push_back(now_insert);
				}
				return 10;
			}
			if (state == -1) {
				slot[0] = new Cuckoo(u1, u2, us1, us2);
#ifdef TEST_MEM
				cuckoo_mem += sizeof(Cuckoo) + (u1 + u2) * sizeof(Bucket);
#endif
				if (!slot[0]->insert(e.dst, e_w))
				{
					wEdge now_insert;
					now_insert.src = e.src;
					now_insert.dst = mykick;
					now_insert.weight = mykick_w;
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
					if (*pv == EE.dst) {
						*(pv + 1) = *(pv + 1) + weight;
						return pt;
					}
				}
				Cuckoo* pv = slot[0];
				key_type* cp = (key_type*)(&pv);
				slot[0] = new Cuckoo(u1, u2, us1, us2);
#ifdef TEST_MEM
				cuckoo_mem += sizeof(Cuckoo) + (u1 + u2) * sizeof(Bucket);
#endif
				if (!(slot[0]->insert(*cp, *(cp + 1))))
				{
					wEdge now_insert;
					now_insert.src = src;
					now_insert.dst = mykick;
					now_insert.weight = mykick_w;
					sblacklist.push_back(now_insert);
				}
				pv = slot[1];
				if (!slot[0]->insert(*cp, *(cp + 1)))
				{
					wEdge now_insert;
					now_insert.src = src;
					now_insert.dst = mykick;
					now_insert.weight = mykick_w;
					sblacklist.push_back(now_insert);
				}
				pv = slot[2];
				if (!slot[0]->insert(*cp, *(cp + 1)))
				{
					wEdge now_insert;
					now_insert.src = src;
					now_insert.dst = mykick;
					now_insert.weight = mykick_w;
					sblacklist.push_back(now_insert);
				}
				slot[1] = NULL;
				slot[2] = NULL;
				if (!slot[0]->insert(EE.dst, weight))
				{
					wEdge now_insert;
					now_insert.src = EE.src;
					now_insert.dst = mykick;
					now_insert.weight = mykick_w;
					sblacklist.push_back(now_insert);
				}
				pt = -1;
				return pt;
			}
			else {
				for (int i = 0; i < 3; ++i) {
					key_type* pv = (key_type*)(&slot[i]);
					if (*pv == EE.dst) {
						*(pv + 1) = *(pv + 1) + weight;
						return pt;
					}
				}
				for (int i = 0; i < 3; ++i) {
					key_type* pv = (key_type*)(&slot[i]);
					if (*pv == 0) {
						*pv = EE.dst;
						*(pv + 1) = weight;
						++pt;
						return pt;
					}
				}
				assert(0);
			}
		}
	}
	count_type query(Edge e) {
		if (e.src != src) {
			return 0;
		}
		if (pt < 0) {
			for (int i = 0; i < 3; ++i) {
				count_type aa;
				if (slot[i] != NULL) aa = slot[i]->query(e.dst);
				if (slot[i] != NULL && aa)return aa;
			}
			for (auto it = sblacklist.begin(); it != sblacklist.end(); ++it) {
				if ((*it).src == e.src && (*it).dst == e.dst) {
					return (*it).weight;
				}
			}
			return 0;
		}
		else {
			for (int i = 0; i < 3; ++i) {
				key_type* pv = (key_type*)(&slot[i]);
				if (*pv == e.dst)return *(pv + 1);
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
	vector<Neighbor> GetNeighbors(key_type src
	)
	{
		vector<Neighbor> v;
		v.clear();
		if (pt < 0) {
			for (int i = 0; i < 3; ++i) {
				if (slot[i] != NULL) {
					for (int j = 0; j < slot[i]->len1; ++j)
						for (int k = 0; k < bup; ++k) {
							if (slot[i]->layer1.layer[j].bslot[k] != 0) {
								v.push_back({ slot[i]->layer1.layer[j].bslot[k],1 });
							}
						}
					for (int j = 0; j < slot[i]->len2; ++j)
						for (int k = 0; k < bup; ++k) {
							if (slot[i]->layer2.layer[j].bslot[k] != 0) {
								v.push_back({ slot[i]->layer2.layer[j].bslot[k],1 });
							}
						}
				}
			}
			for (auto it = sblacklist.begin(); it != sblacklist.end(); ++it) {
				if ((*it).src == src) {
					v.push_back({ (*it).dst,1 });
				}
			}
			return v;
		}
		if (pt == 0)return v;
		for (int i = 0; i < 3; ++i) {
			key_type* pv = (key_type*)(&slot[i]);
			if (0 != *pv) {
				v.push_back({ *pv,1 });
			}
		}
		return v;
		}
	bool isSlotEmpty() {
		return pt == 0;
	}
	};
static bSlot holekick;
class bBucket {
public:
	bSlot bslot[bup];
	int insert(Edge e, count_type weight) {
		for (int i = 0; i < bup; ++i)
			if (bslot[i].src == e.src) {
				bslot[i].insert(e, weight);
				return -1;
			}
		for (int i = 0; i < bup; ++i)
			if (bslot[i].insert(e, weight)) {
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
	count_type query(Edge e) {
		for (int i = 0; i < bup; ++i) {
			if (bslot[i].src == e.src) {
				return bslot[i].query(e);
			}
		}
		return 0;
	}
	int insert_query3(Edge e, count_type weight) {
		for (int i = 0; i < bup; ++i) {
			if (bslot[i].src == e.src) {
				if (bslot[i].query_insert(e, weight))
					return sure;
				bslotnum = i;
				return 1;
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
	count_type query(Edge e) {
		return layer[(int)(mmhash(e.src, seed) % (uint32_t)length)].query(e);
	}
	int insert(Edge e, count_type weight) {
		return layer[(int)(mmhash(e.src, seed) % (uint32_t)length)].insert(e, weight);
	}
	bool insert(bSlot e) {
		return layer[(int)(mmhash(e.src, seed) % (uint32_t)length)].insert(e);
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
	bCuckoo(int l1, int l2, int s1, int s2) :len1(l1), len2(l2), seed1(s1), seed2(s2), num(0), layer1(l1, s1), layer2(l2, s2)
	{
	}
	bool insert(Edge item, count_type weight) {
#ifdef TEST_KICK
		l_kick = 0;
		l_num += 1;
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
		int times = 500;
		bool flag = true;
		int pos1 = (int)(mmhash(item.src, seed1) % (uint32_t)len1);
		int pos2 = (int)(mmhash(item.src, seed2) % (uint32_t)len2);
		bSlot kick;
		bSlot ins;
		ins.insert(item, weight);
		while (times) {
			int xx = rand();
			if (flag) {
				kick = layer1.layer[pos1].bslot[xx % bup];
				layer1.layer[pos1].bslot[xx % bup] = ins;
				if (layer2.insert(kick)) {
					++num;
#ifdef TEST_KICK
					l_kick = 500 + 1 - times;
					l_hole += l_kick;
#endif
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
#ifdef TEST_KICK
					l_kick = 500 + 1 - times;
					l_hole += l_kick;
#endif
					return true;
				}
				--times;
				flag = true;
				ins = kick;
				pos1 = (int)(mmhash(ins.src, seed1) % (uint32_t)len1);
			}
		}
		holekick = ins;
#ifdef TEST_KICK
		l_kick = 500;
		l_hole += l_kick;
#endif
		return false;
	}
	bool insert(bSlot item) {
#ifdef TEST_KICK
		l_kick = 0;
		l_num += 1;
#endif
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
#ifdef TEST_KICK
					l_kick = 500 + 1 - times;
					l_hole += l_kick;
#endif
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
#ifdef TEST_KICK
					l_kick = 500 + 1 - times;
					l_hole += l_kick;
#endif
					return true;
				}
				--times;
				flag = true;
				ins = kick;
				pos1 = (int)(mmhash(ins.src, seed1) % (uint32_t)len1);
			}
		}
		holekick = ins;
#ifdef TEST_KICK
		l_kick = 500;
		l_hole += l_kick;
#endif
		return false;
	}
	count_type query(Edge item) {
		int ans = layer1.query(item);
		if (ans)return ans;
		return layer2.query(item);
	}
	int insert_query3(Edge item, count_type weight) {
		int ha = (int)(mmhash(item.src, seed1) % (uint32_t)len1);
		int ans = layer1.layer[ha].insert_query3(item, weight);
		if (ans == sure)return sure;
		if (ans) { layernum = 1; idth = ha; return 1; }
		ha = (int)(mmhash(item.src, seed2) % (uint32_t)len2);
		ans = layer2.layer[ha].insert_query3(item, weight);
		if (ans == sure)return sure;
		if (ans) { layernum = 2; idth = ha; return 2; }
		return 0;
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
	bCuckoo* merge(bCuckoo* c0, bCuckoo* c1, bCuckoo* c2) {
		int l1 = c0->len1;
		int l2 = c0->len2;
		queue<bSlot> mq;
		bCuckoo* c = new bCuckoo(l1 * 2, l2 * 2, s1, s2);
		memcpy(c->layer1.layer, c0->layer1.layer, l1 * sizeof(bBucket));
		memcpy(c->layer1.layer + l1, c0->layer1.layer, l1 * sizeof(bBucket));
		memcpy(c->layer2.layer, c0->layer2.layer, l2 * sizeof(bBucket));
		memcpy(c->layer2.layer + l2, c0->layer2.layer, l2 * sizeof(bBucket));
		c->num = c0->num;
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
		}
		for (int i = 0; i < l1 / 2; ++i) {
			for (int j = 0; j < bup; ++j) {
				if (c1->layer1.layer[i].bslot[j].src == 0)continue;
				if ((c->layer1.layer[(mmhash(c1->layer1.layer[i].bslot[j].src, s1) % (uint32_t)(l1 * 2))].insert(c1->layer1.layer[i].bslot[j])) == false)mq.push(c1->layer1.layer[i].bslot[j]);
				else c->num = (c->num) + 1;
			}
			for (int j = 0; j < bup; ++j) {
				if (c2->layer1.layer[i].bslot[j].src == 0)continue;
				if ((c->layer1.layer[(mmhash(c2->layer1.layer[i].bslot[j].src, s1) % (uint32_t)(l1 * 2))].insert(c2->layer1.layer[i].bslot[j])) == false)mq.push(c2->layer1.layer[i].bslot[j]);
				else c->num = (c->num) + 1;
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
		}
		for (int i = 0; i < l2 / 2; ++i) {
			for (int j = 0; j < bup; ++j) {
				if (c1->layer2.layer[i].bslot[j].src == 0)continue;
				if ((c->layer2.layer[(mmhash(c1->layer2.layer[i].bslot[j].src, s2) % (uint32_t)(l2 * 2))].insert(c1->layer2.layer[i].bslot[j])) == false)mq.push(c1->layer2.layer[i].bslot[j]);
				else c->num = (c->num) + 1;
			}
			for (int j = 0; j < bup; ++j) {
				if (c2->layer2.layer[i].bslot[j].src == 0)continue;
				if ((c->layer2.layer[(mmhash(c2->layer2.layer[i].bslot[j].src, s2) % (uint32_t)(l2 * 2))].insert(c2->layer2.layer[i].bslot[j])) == false)mq.push(c2->layer2.layer[i].bslot[j]);
				else c->num = (c->num) + 1;
			}
		}
		while (!mq.empty()) {
			if (!c->insert(mq.front())) {
				blacklist.push_back(holekick);
			}
			mq.pop();
			}
		delete c0;
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
	vector<Neighbor> GetNeighbors(key_type src
	) {
		for (int i = 0; i <= state; ++i) {
			key_type h1 = mmhash(src, link[i]->seed1) % ((uint32_t)(link[i]->layer1.length));
			key_type h2 = mmhash(src, link[i]->seed2) % ((uint32_t)(link[i]->layer2.length));
			for (int j = 0; j < bup; ++j) {
				if (src == link[i]->layer1.layer[h1].bslot[j].src) {
					return link[i]->layer1.layer[h1].bslot[j].GetNeighbors(src);
		}
				else if (src == link[i]->layer2.layer[h2].bslot[j].src) {
					return link[i]->layer2.layer[h2].bslot[j].GetNeighbors(src);
				}
	}
		}
		for (auto it = blacklist.begin(); it != blacklist.end(); ++it) {
			if ((*it).src == src) {
				return (*it).GetNeighbors(src);
			}
		}
		vector<Neighbor> v;
		v.clear();
		return v;
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
				if ((precursor1[j] != successor1[i]) && (successor1[i] != src) && (precursor1[j] != src)) {
					if (query(e))++cnt;
			}
		}
	}
		return cnt;
	}
	bool insert(Edge e, count_type weight = 1) {
		int ans = insert_query3(e, weight);
		if (ans == sure) return true;
		if (ans) {
			if (ans == blist) { blacklist[idth].insert(e, weight); return true; }
			bCuckoo* ptr = link[ans - 1];
			if (layernum == 1) {
				ptr->layer1.layer[idth].bslot[bslotnum].insert(e, weight);
				return true;
			}
			else if (layernum == 2) {
				ptr->layer2.layer[idth].bslot[bslotnum].insert(e, weight);
				return true;
		}
	}
		if (state == -1) {
			link[0] = new bCuckoo(ll1, ll2, s1, s2);
#ifdef TEST_MEM
			cuckoo_mem += sizeof(bCuckoo) + (ll1 + ll2) * sizeof(bBucket);
#endif
			state = 0;
			if (!((*link[0]).insert(e, weight))) {
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
				if (!((*link[1]).insert(e, weight))) {
					blacklist.push_back(holekick);
				}
				return true;
			}
			else {
				if (!((*link[0]).insert(e, weight))) {
					blacklist.push_back(holekick);
				}
				return true;
			}
		}
		if (state == 1) {
			for (int i = state; i >= 0; --i) {
				if ((link[i]->num < (link[i]->len1 + link[i]->len2) * bup * rate) && (blacklist.size() < bbl))
				{
					if (!((*link[i]).insert(e, weight))) {
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
			if (!((*link[2]).insert(e, weight))) {
				blacklist.push_back(holekick);
			}
			return true;
		}
		if (state == 2) {
			for (int i = state; i >= 0; --i) {
				if ((link[i]->num < (link[i]->len1 + link[i]->len2) * bup * rate) && (blacklist.size() < bbl))
				{
					if (!((*link[i]).insert(e, weight))) {
						blacklist.push_back(holekick);
						;
					}
					return true;
				}
			}
			bCuckoo* cm = merge(link[0], link[1], link[2]);
			link[0] = cm;
			link[1] = new bCuckoo(link[0]->len1 / 2, link[0]->len2 / 2, s1, s2);
#ifdef TEST_MEM
			cuckoo_mem += (link[0]->len1 / 2 + link[0]->len2 / 2) * sizeof(bBucket);
			cuckoo_mem = cuckoo_mem - sizeof(bCuckoo);
#endif
			link[2] = NULL;
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
			if (!((*link[1]).insert(e, weight))) {
				blacklist.push_back(holekick);
			}
			return true;
		}
		cout << "BUG!" << endl;
		return false;
	}
	count_type query(Edge e) {
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
	int insert_query3(Edge e, count_type weight) {
		for (int i = 0; i <= state; ++i) {
			int ans = link[i]->insert_query3(e, weight);
			if (ans == sure)return sure;
			if (ans)return i + 1;
		}
		int j = 0;
		for (auto it = blacklist.begin(); it != blacklist.end(); ++it) {
			if ((*it).src == e.src) {
				if ((*it).query_insert(e, weight)) {
					return sure;
				}
				idth = j;
				return blist;
			}
			++j;
		}
		return 0;
	}
	int query3(Edge e) {
		cout << "query3" << endl;
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
};
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
		if (!hole.query(*it)) {
			cout << "error" << endl;
			break;
			++ernum;
		}
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
		if (!hole.query(edge[i])) {
			cout << "error" << endl;
			break;
			++ernum2;
		}
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
	Edge* edge = read_wiki_data(&cnt, 100000000);
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
void test_hole_time() {
	cout << "CuckooGrpah" << endl;
	sblacklist.clear();
	count_type cnt;
	Edge* edge = read_NotreDame_data(&cnt, 100000000);
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
	int sum = 0;
	clock_t time_start_2 = clock();
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
	int mout = (edsize / 1000000) + 1;
	holemerge hole(bl1, bl2, 0, 221);
#ifdef TEST_MEM 
	cuckoo_mem = sizeof(holemerge) + sizeof(sblacklist) + sbl * sizeof(wEdge) + bbl * sizeof(bSlot);
#endif 
	string fname = "./Hole_mem.csv";
	ofstream fout;
	fout.open(fname);
	int i = 0;
	int small = 0;
	int big = 0;
	for (auto it = ed.begin(); it != ed.end(); it++) {
		hole.insert(*it);
		++i;
		if (sblacklist.size() > sbl)++small;
		if (hole.blacklist.size() > bbl)++big;
		if (0 == ((i - 1) % mout))
			fout << ((double)cuckoo_mem) / 1000 << endl;
	}
	cout << "small: " << small << endl;
	cout << "big: " << big << endl;
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
	Edge* edge = read_wiki_data(&cnt, 100000000);
	set<Edge, EdgeLess> ed;
	ed.insert(edge, edge + cnt);
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	int mout = (edsize / 1000000) + 1;
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
		++i;
		if (0 == ((i - 1) % mout))
			fout << ((double)wind_mem) / 1000 << endl;
	}
	fout.close();
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
	int sum = 0;
	clock_t time_start_2 = clock();
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
	int sum = 0;
	for (int i = 0; i < mul; ++i) {
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
void tcm_test_time_multi() {
	Edge edge;
	count_type cnt;
	int mul = 3;
	double insert[10];
	double query[10];
	for (int i = 0; i < 10; ++i) {
		insert[i] = 0;
		query[i] = 0;
	}
	Edge* EE = read_stackoverflow_data(&cnt, 100000000);
	cout << "cnt: " << cnt << endl;
	int fin = 63000;
	set<key_type> ed;
	for (count_type i = 0; i < fin * 1000; ++i)
		ed.insert(asrc[i]);
	int edsize = ed.size();
	ofstream fout;
	fout.open("tcm_test_time_multi.txt", ios::out);
	for (int num = 5 * 7000; num <= fin; num += 7000) {
		cnt = num * 1000;
		fout << "DATA SIZE: " << cnt << endl;
		for (int i = 0; i < mul; ++i) {
			TCM hole(19200, 19200, 9, false);
			int sum = 0;
			clock_t time_start = clock();
			for (int i = 0; i < cnt; ++i) {
				string s1 = to_string(asrc[i]);
				string s2 = to_string(adst[i]);
				hole.insert((unsigned char*)(s1.c_str()), (unsigned char*)(s2.c_str()), 1, s1.length(), s2.length());
			}
			clock_t time_end = clock();
			long time = time_end - time_start;
			insert[i] = (double)time / (double)cnt;
			cout << "sum: " << sum << endl;
			sum = 0;
			clock_t time_start_2 = clock();
			for (int i = 0; i < cnt; ++i) {
				string s1 = to_string(asrc[i]);
				string s2 = to_string(adst[i]);
				sum += hole.query((unsigned char*)(s1.c_str()), (unsigned char*)(s2.c_str()), s1.length(), s2.length());
			}
			clock_t time_end_2 = clock();
			long time3 = time_end_2 - time_start_2;
			query[i] = (double)time3 / (double)cnt;
			cout << "sum: " << sum << endl;
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
void gss_test_time_multi() {
	Edge edge;
	count_type cnt;
	int mul = 3;
	double insert[10];
	double query[10];
	for (int i = 0; i < 10; ++i) {
		insert[i] = 0;
		query[i] = 0;
	}
	Edge* EE = read_stackoverflow_data(&cnt, 100000000);
	cout << "cnt: " << cnt << endl;
	int fin = 63000;
	set<key_type> ed;
	for (count_type i = 0; i < fin * 1000; ++i)
		ed.insert(asrc[i]);
	int edsize = ed.size();
	ofstream fout;
	fout.open("gss_test_time_multi.txt", ios::out);
	for (int num = 7000; num <= fin; num += 7000) {
		cnt = num * 1000;
		fout << "DATA SIZE: " << cnt << endl;
		for (int i = 0; i < mul; ++i) {
			GSS hole(19200, 16, 16, 2, 16, false);
			int sum = 0;
			clock_t time_start = clock();
			for (int i = 0; i < cnt; ++i) {
				string s1 = to_string(asrc[i]);
				string s2 = to_string(adst[i]);
				hole.insert(s1, s2, 1);
			}
			clock_t time_end = clock();
			long time = time_end - time_start;
			insert[i] = (double)time / (double)cnt;
			cout << "sum: " << sum << endl;
			sum = 0;
			clock_t time_start_2 = clock();
			for (int i = 0; i < cnt; ++i) {
				string s1 = to_string(asrc[i]);
				string s2 = to_string(adst[i]);
				sum += hole.edgeQuery(s1, s2);
			}
			clock_t time_end_2 = clock();
			long time3 = time_end_2 - time_start_2;
			query[i] = (double)time3 / (double)cnt;
			cout << "sum: " << sum << endl;
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
void Auxo_test_time_multi() {
	Edge edge;
	count_type cnt;
	int mul = 3;
	double insert[10];
	double query[10];
	for (int i = 0; i < 10; ++i) {
		insert[i] = 0;
		query[i] = 0;
	}
	int fpl = 16, cols = 4, candiNum = 16, width = 100;
	Edge* EE = read_stackoverflow_data(&cnt, 100000000);
	cout << "cnt: " << cnt << endl;
	int fin = 63000;
	set<key_type> ed;
	for (count_type i = 0; i < fin * 1000; ++i)
		ed.insert(asrc[i]);
	int edsize = ed.size();
	ofstream fout;
	fout.open("Auxo_test_time_multi.txt", ios::out);
	for (int num = 7000; num <= fin; num += 7000) {
		cnt = num * 1000;
		fout << "DATA SIZE: " << cnt << endl;
		for (int i = 0; i < mul; ++i) {
			Auxo hole(width, cols, candiNum, fpl);
			int sum = 0;
			clock_t time_start = clock();
			for (int i = 0; i < cnt; ++i) {
				string s1 = to_string(asrc[i]);
				string s2 = to_string(adst[i]);
				sum += hole.insert(s1, s2, 1);
			}
			clock_t time_end = clock();
			long time = time_end - time_start;
			insert[i] = (double)time / (double)cnt;
			cout << "sum: " << sum << endl;
			sum = 0;
			clock_t time_start_2 = clock();
			for (int i = 0; i < cnt; ++i) {
				string s1 = to_string(asrc[i]);
				string s2 = to_string(adst[i]);
				sum += hole.edgeQuery(s1, s2);
			}
			clock_t time_end_2 = clock();
			long time3 = time_end_2 - time_start_2;
			query[i] = (double)time3 / (double)cnt;
			cout << "sum: " << sum << endl;
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
	cnt = load_data13_CAIDA();
	cout << "cnt: " << cnt << endl;
	int fin = 27000;
	set<key_type> ed;
	for (count_type i = 0; i < fin * 1000; ++i)
		ed.insert(asrc[i]);
	int edsize = ed.size();
	ofstream fout;
	fout.open("hole_test_time_multi.txt", ios::out);
	for (int num = 3000; num <= fin; num += 6000) {
		cnt = num * 1000;
		fout << "DATA SIZE: " << cnt << endl;
		for (int i = 0; i < mul; ++i) {
			int l2 = (edsize / (3 * bup * brate)) + 1;
			if (l2 % 2)l2 = l2 + 1;
			int l1 = l2 * 2;
			cout << "l1:  " << l1 << endl;
			cout << "l2:  " << l2 << endl;
			sblacklist.clear();
			holemerge hole(bl1, bl2, 0, 221);
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
	Edge* edge = read_sparse_data(&cnt, 100000000);
	cout << "cnt: " << cnt << endl;
	ofstream fout;
	fout.open("wind_test_time_multi.txt", ios::out);
	for (int num = 6000; num <= 30000; num += 6000) {
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
void test_triangles_2() {
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
		edg.insert(EE[itt].src);
		inum = ed.size();
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
void test_triangles_1() {
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int beg = 20000000;
	int inter = 24000000 - 20000000;
	int sample_num = 5000;
	Edge* EE = read_stackoverflow_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();
	ofstream fout;
	fout.open("test_triangles_1.txt", ios::out);
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
		edg.insert(EE[itt].src);
		inum = ed.size();
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
void test_triangles_co() {
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int beg = 20000000;
	int inter = 24000000 - 20000000;
	int sample_num = 5000;
	assert(need.size() == mul * sample_num * 5);
	Edge* EE = read_stackoverflow_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();
	ofstream fout;
	fout.open("test_tiangles_co.txt", ios::out);
	holemerge hole(245816, 122908, 0, 221);
	int inum = 0;
	int sum1 = 0;
	int sum2 = 0;
	for (int itt = 0; itt < cnt; ++itt) {
		if (ed.count(EE[itt]))continue;
		ed.insert(EE[itt]);
		hole.insert(EE[itt]);
		edg.insert(EE[itt].dst);
		edg.insert(EE[itt].src);
		inum = ed.size();
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
	int beg = 530000;
	int inter = 610000 - 530000;
	int sample_num = 5000;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* EE = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&EE[i], &items[i], sizeof(Edge));
	std::cout << "The number of packet:" << cnt << std::endl;
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
	delete[] items;
}
void test_time_succ_co() {
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int beg = 530000;
	int inter = 610000 - 530000;
	int sample_num = 5000;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* EE = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&EE[i], &items[i], sizeof(Edge));
	std::cout << "The number of packet:" << cnt << std::endl;
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	ofstream fout;
	ofstream fr1;
	ofstream fr2;
	fout.open("test_time_succ_co.txt", ios::out);
	fr1.open("fr1_co.txt", ios::out);
	fr2.open("fr2_co.txt", ios::out);
	holemerge hole(28136, 14068, 0, 221);
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
	delete[] items;
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
	int L1 = 245816;
	int L2 = L1 / 2;
	count_type cnt;
	int mul = 3;
	int maxtopk = 4000;
	int rd = 10;
	key_type topk[10001] = { 0 };
	Edge* EE = read_stackoverflow_data(&cnt, 100000000);
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
		int num = 70 * TT;
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
		int num = 70 * TT;
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
#ifdef TEST
void test_big_del() {
	big_del = 0;
	BIG_DEL = 0;
	count_type cnt;
	Edge* EE = read_NotreDame_data(&cnt, 100000000);
	holemerge hole(bl1, bl2, 0, 221);
	Edge ee;
	int sum = 0;
	clock_t time_start = clock();
	for (int i = 0; i < cnt; ++i) {
		sum += hole.insert(EE[i]);
		if ((((i + 1) % 5) == 2) && i != 1)
			hole.insert(ee);
		if (((i + 1) % 5) == 0) {
			hole.del(EE[i]);
			++hole_del;
			ee = EE[i];
		}
	}
	clock_t time_end = clock();
	long time = (time_end - time_start);
	cout << "hole del num: " << hole_del << endl;
	cout << "S-CHT del num: " << big_del << endl;
	cout << "L-CHT del num: " << BIG_DEL << endl;
	cout << "S-CHT del rate: " << (double)big_del / (double)hole_del << endl;
	cout << "L-CHT del rate: " << (double)BIG_DEL / (double)hole_del << endl;
	cout << "average operate time: " << (double)time / (double)(cnt + hole_del);
	delete[] EE;
}
#endif
#ifdef TEST_KICK
void test_kick() {
	sblacklist.clear();
	count_type cnt;
	Edge* edge = read_NotreDame_data(&cnt, 100000000);
	holemerge hole(bl1, bl2, 0, 221);
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	cout << "S-CHT average kick times: " << (double)s_hole / (double)s_num << endl;
	cout << "S-CHT average T(i.e. kicktimes/2): " << ((double)s_hole / (double)s_num) / 2 << endl;
	cout << "L-CHT average kick times: " << (double)l_hole / (double)l_num << endl;
	cout << "L-CHT average T(i.e. kicktimes/2): " << ((double)l_hole / (double)l_num) / 2 << endl;
	delete[] edge;
}
void test_kick_2() {
	count_type cnt;
	Cuckoo* c0;
	Edge* EE = read_NotreDame_data(&cnt, 100000000);
	set<key_type> node; node.clear();
	for (int i = 0; i < cnt; ++i) {
		node.insert(EE[i].dst);
	}
	for (int L = 4; L <= 1024 * 16; L = L * 4) {
		sblacklist.clear();
		c0 = new Cuckoo(L, L / 2, us1, us2);
		s_hole = 0;
		l_hole = 0;
		s_num = 0;
		l_num = 0;
		unsigned long i = 0;
		unsigned long h = 0;
		for (auto it = node.begin(); it != node.end(); ) {
			if (c0->num < (c0->len1 + c0->len2) * bup * rate)
			{
				c0->insert(*it); ++it;
				++i;
				h += s_kick;
			}
			else break;
		}
		assert(h == s_hole); assert(i == s_num);
		cout << "L=" << L << " :" << endl;
		cout << "S-CHT average kick times: " << (double)s_hole / (double)s_num << endl;
		cout << "S-CHT average T(i.e. kicktimes/2): " << ((double)s_hole / (double)s_num) / 2 << endl;
	}
}
#endif
void test_CDF() {
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
	set<key_type> node; node.clear();
	for (int i = 0; i < cnt; ++i) {
		node.insert(edge[i].dst);
		node.insert(edge[i].src);
	}
	cout << "node num:  " << node.size() << endl;
}
void test_Circle() {
	count_type cnt;
	Edge* edge = read_NotreDame_data(&cnt, 100000000);
	for (int i = 0; i < cnt; ++i) {
		if (edge[i].dst == edge[i].src)cout << edge[i].dst << "   " << edge[i].src << endl;
	}
	cout << "OK!  " << endl;
}
void make_dataset() {
	ofstream fout;
	double node = 6000;
	double rrr = 0.9;
	double num = rrr * node;
	fout.open("CuckooGraphDataset.txt", ios::out);
	for (int i = 1; i <= node; ++i) {
		for (int j = 1; j <= num; ++j) {
			int u = i + j;
			if (u > node)u = u - node;
			fout << u << ',' << i << endl;
		}
	}
	fout.close();
}
void make_dataset_new() {
	ofstream fout;
	double node = 8000;
	double rrr = 0.9;
	double num = rrr * node;
	int r = 10;
	fout.open("CuckooGraphDatasetNewNew.txt", ios::out);
	for (int i = 1; i <= node; ++i) {
		for (int j = 1; j <= node; ++j) {
			if (j != i && ((rand() % (10))))
				fout << j << ',' << i << endl;
		}
	}
	fout.close();
}
void test_gss_time() {
	cout << "GSS:" << endl;
	count_type cnt;
	Edge* edge = read_NotreDame_data(&cnt, 100000000);
	GSS hole(1200, 16, 16, 2, 16, false);
	int sum = 0;
	clock_t t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(to_string(edge[i].src), to_string(edge[i].dst), 1);
	}
	clock_t t2 = clock();
	long t3 = t2 - t1;
	cout << "Insert average time: " << (double)t3 / (double)cnt << endl;
	t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		sum += hole.edgeQuery(to_string(edge[i].src), to_string(edge[i].dst));
	}
	t2 = clock();
	t3 = t2 - t1;
	cout << "Query average time: " << (double)t3 / (double)cnt << endl;
	cout << "sum: " << sum << endl;
	delete[] edge;
}
void test_Auxo_time() {
	cout << "Auxo:" << endl;
	count_type cnt;
	Edge* edge = read_NotreDame_data(&cnt, 100000000);
	int fpl = 16, cols = 4, candiNum = 16, width = 100;
	Auxo* auxo = new Auxo(width, cols, candiNum, fpl);
	int sum = 0;
	clock_t t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		auxo->insert(to_string(edge[i].src), to_string(edge[i].dst));
	}
	clock_t t2 = clock();
	long t3 = t2 - t1;
	cout << "Insert average time: " << (double)t3 / (double)cnt << endl;
	t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		sum += auxo->edgeQuery(to_string(edge[i].src), to_string(edge[i].dst));
	}
	t2 = clock();
	t3 = t2 - t1;
	cout << "Query average time: " << (double)t3 / (double)cnt << endl;
	cout << "sum: " << sum << endl;
	cout << "memory:  " << (double)(auxo->memoryAllocated2) / 131072 << " M\n";
	delete[] edge;
	delete auxo;
}
void test_tcm_time() {
	cout << "TCM:" << endl;
	count_type cnt;
	Edge* edge = read_NotreDame_data(&cnt, 100000000);
	TCM hole(19200, 19200, 9, false);
	int sum = 0;
	clock_t t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		string s1 = to_string(edge[i].src);
		string s2 = to_string(edge[i].dst);
		hole.insert((unsigned char*)(s1.c_str()), (unsigned char*)(s2.c_str()), 1, s1.length(), s2.length());
	}
	clock_t t2 = clock();
	long t3 = t2 - t1;
	cout << "Insert average time: " << (double)t3 / (double)cnt << endl;
	t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		string s1 = to_string(edge[i].src);
		string s2 = to_string(edge[i].dst);
		sum += hole.query((unsigned char*)(s1.c_str()), (unsigned char*)(s2.c_str()), s1.length(), s2.length());
	}
	t2 = clock();
	t3 = t2 - t1;
	cout << "Query average time: " << (double)t3 / (double)cnt << endl;
	cout << "sum: " << sum << endl;
	delete[] edge;
}
void test_time_2hop_succ() {
	chain* tempC = new chain();
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int beg = 20000000;
	int inter = 24000000 - 20000000;
	int sample_num = 300;
	Edge* EE = read_stackoverflow_data(&cnt, 100000000);
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	ofstream fout;
	ofstream fr1;
	ofstream fr2;
	fout.open("test_time_2hop_succ.txt", ios::out);
	fr1.open("fr1.txt", ios::out);
	fr2.open("fr2.txt", ios::out);
	Chandelier* wind;
	holemerge hole(bl1, bl2, 0, 221);
	wind = new Chandelier(csl);
	cout << "ok!" << endl;
	cout << "ok!" << endl;
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
					int hn = 0;
					hn = wind->query_successor(ms, precursor, hn);
					for (int yy = 0; yy < hn; ++yy) {
						sum1 += wind->query_successor(precursor[yy], successor, sum1);
					}
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
					int hn = 0;
					hn = hole.query_successor(ms, precursor, hn);
					for (int yy = 0; yy < hn; ++yy) {
						sum2 += hole.query_successor(precursor[yy], successor1, sum2);
					}
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
void test_time_2hop_succ_co() {
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int beg = 20000000;
	int inter = 24000000 - 20000000;
	int sample_num = 300;
	Edge* EE = read_stackoverflow_data(&cnt, 100000000);
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	ofstream fout;
	ofstream fr1;
	ofstream fr2;
	fout.open("test_time_2hop_succ_co.txt", ios::out);
	fr1.open("fr1_co.txt", ios::out);
	fr2.open("fr2_co.txt", ios::out);
	holemerge hole(245816, 122908, 0, 221);
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
					int hn = 0;
					hn = hole.query_successor(ms, precursor, hn);
					for (int yy = 0; yy < hn; ++yy) {
						sum2 += hole.query_successor(precursor[yy], successor1, sum2);
					}
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
void test_time_2hop_pre() {
	chain* tempC = new chain();
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int beg = 20000000;
	int inter = 24000000 - 20000000;
	int sample_num = 300;
	Edge* EE = read_stackoverflow_data(&cnt, 100000000);
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	ofstream fout;
	ofstream fr1;
	ofstream fr2;
	fout.open("test_time_2hop_pre.txt", ios::out);
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
					int hn = 0;
					hn = wind->query_precursor(ms, successor, hn);
					for (int yy = 0; yy < hn; ++yy) {
						sum1 += wind->query_precursor(successor[yy], precursor, sum1);
					}
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
					int hn = 0;
					hn = hole.query_successor(ms, successor, hn);
					for (int yy = 0; yy < hn; ++yy) {
						sum2 += hole.query_successor(successor[yy], precursor1, sum2);
					}
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
void test_time_2hop_pre_co() {
	int csl = 400;
	count_type cnt;
	int mul = 3;
	int beg = 20000000;
	int inter = 24000000 - 20000000;
	int sample_num = 300;
	Edge* EE = read_stackoverflow_data(&cnt, 100000000);
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	ofstream fout;
	ofstream fr1;
	ofstream fr2;
	fout.open("test_time_2hop_pre_co.txt", ios::out);
	fr1.open("fr1_co.txt", ios::out);
	fr2.open("fr2_co.txt", ios::out);
	holemerge hole(253920, 126960, 0, 221);
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
					int hn = 0;
					hn = hole.query_successor(ms, successor, hn);
					for (int yy = 0; yy < hn; ++yy) {
						sum2 += hole.query_successor(successor[yy], precursor1, sum2);
					}
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
void make_sparse_dataset() {
	ofstream fout;
	double node = 5000000;
	double num = 6;
	fout.open("Sparse.txt", ios::out);
	for (int i = 1; i <= node; ++i) {
		for (int j = 1; j <= num; ++j) {
			int u = i + j;
			if (u > node)u = u - node;
			fout << u << ',' << i << endl;
		}
	}
	fout.close();
}
#ifdef TEST_MEM
void test_Hole_mem_huge() {
	set<Edge, EdgeLess> ed;
	count_type cnt;
	for (int q = 0; q < 59; ++q) {
		TUPLES* items = read_data((folder + filenames[q]).c_str(), 100000000, &cnt);
		Edge* edge = new Edge[cnt];
		for (count_type i = 0; i < cnt; ++i)
			memcpy(&edge[i], &items[i], sizeof(Edge));
		ed.insert(edge, edge + cnt);
		delete[] items;
		delete[] edge;
	}
	int edsize = ed.size();
	cout << "edsize:  " << edsize << endl;
	int mout = (edsize / 1000000) + 1;
	holemerge hole(bl1, bl2, 0, 221);
#ifdef TEST_MEM 
	cuckoo_mem = sizeof(holemerge) + sizeof(sblacklist) + sbl * sizeof(wEdge) + bbl * sizeof(bSlot);
#endif 
	string fname = "./Hole_mem.csv";
	ofstream fout;
	fout.open(fname);
	int i = 0;
	int small = 0;
	int big = 0;
	for (auto it = ed.begin(); it != ed.end(); it++) {
		hole.insert(*it);
		++i;
		if (sblacklist.size() > sbl)++small;
		if (hole.blacklist.size() > bbl)++big;
		if (0 == ((i - 1) % mout))fout << i << "," << ((double)cuckoo_mem) / 1000 << endl;
	}
	cout << "small: " << small << endl;
	cout << "big: " << big << endl;
	fout.close();
#ifdef TEST_MEM
	cuckoo_mem = 0;
#endif
}
#endif  
#ifdef TEST_MEM
void test_wind_mem_huge() {
	set<Edge, EdgeLess> ed;
	count_type cnt;
	for (int q = 0; q < 59; ++q) {
		TUPLES* items = read_data((folder + filenames[q]).c_str(), 100000000, &cnt);
		Edge* edge = new Edge[cnt];
		for (count_type i = 0; i < cnt; ++i)
			memcpy(&edge[i], &items[i], sizeof(Edge));
		ed.insert(edge, edge + cnt);
		delete[] items;
		delete[] edge;
	}
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
	wind_mem = 0;
}
#endif  
void huge_hole_test_time_multi() {
	count_type cnt;
	int mul = 3;
	double insert[10];
	double query[10];
	for (int i = 0; i < 10; ++i) {
		insert[i] = 0;
		query[i] = 0;
	}
	Edge* EE = new Edge[1468822130];
	long long count = 0;
	for (int q = 0; q < 59; ++q) {
		TUPLES* items = read_data((folder + filenames[q]).c_str(), 100000000, &cnt);
		Edge* edge = new Edge[cnt];
		for (count_type i = 0; i < cnt; ++i)
			memcpy(&edge[i], &items[i], sizeof(Edge));
		memcpy(EE + count, edge, cnt * sizeof(Edge));
		count = count + cnt;
		delete[] items;
		delete[] edge;
	}
	int fin = 1400000;
	set<key_type> ed;
	for (count_type i = 0; i < fin * 1000; ++i)
		ed.insert(EE[i].src);
	int edsize = ed.size();
	ofstream fout;
	fout.open("hole_test_time_multi.txt", ios::out);
	for (int num = 600000; num <= 1400000; num += 200000) {
		cnt = num * 1000;
		fout << "DATA SIZE: " << cnt << endl;
		for (int i = 0; i < mul; ++i) {
			int l2 = (edsize / (3 * bup * brate)) + 1;
			if (l2 % 2)l2 = l2 + 1;
			int l1 = l2 * 2;
			cout << "l1:  " << l1 << endl;
			cout << "l2:  " << l2 << endl;
			sblacklist.clear();
			holemerge hole(bl1, bl2, 0, 221);
			int sum = 0;
			clock_t time_start = clock();
			for (int i = 0; i < cnt; ++i) {
				sum += hole.insert(EE[i]);
			}
			clock_t time_end = clock();
			long time = time_end - time_start;
			insert[i] = (double)time / (double)cnt;
			cout << "sum: " << sum << endl;
			sum = 0;
			clock_t time_start_2 = clock();
			for (int i = 0; i < cnt; ++i) {
				sum += hole.query(EE[i]);
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
void huge_wind_test_time_multi() {
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
	Edge* EE = new Edge[1468822130];
	long long count = 0;
	for (int q = 0; q < 59; ++q) {
		TUPLES* items = read_data((folder + filenames[q]).c_str(), 100000000, &cnt);
		Edge* edge = new Edge[cnt];
		for (count_type i = 0; i < cnt; ++i)
			memcpy(&edge[i], &items[i], sizeof(Edge));
		memcpy(EE + count, edge, cnt * sizeof(Edge));
		count = count + cnt;
		delete[] items;
		delete[] edge;
	}
	ofstream fout;
	fout.open("wind_test_time_multi.txt", ios::out);
	for (int num = 600000; num <= 1400000; num += 200000) {
		cnt = num * 1000;
		fout << "DATA SIZE: " << cnt << endl;
		for (int i = 0; i < mul; ++i) {
			Chandelier* wind;
			wind = new Chandelier(csl);
			int sum = 0;
			clock_t time_start = clock();
			for (int i = 0; i < cnt; ++i) {
				chain* c = new chain();
				c->src = EE[i].src;
				c->dst = EE[i].dst;
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
				int s = EE[i].src;
				int d = EE[i].dst;
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
	delete[] EE;
}
long long trans(Edge e) {
	long long ret = e.src;
	ret = ret << 32;
	ret = ret + e.dst;
	return ret;
}
void test_weight_hole_err2() {
	sblacklist.clear();
	count_type cnt;
	Edge* edge = read_stackoverflow_data(&cnt, 100000000);
	unordered_map< long long, count_type > mp;
	for (int l = 0; l < cnt; ++l) {
		if (mp.find(trans(edge[l])) == mp.end())
			mp[trans(edge[l])] = 1;
		else
			mp[trans(edge[l])] += 1;
	}
	holemerge hole(bl1, bl2, 0, 221);
	asum = 0;
	int ernum = 0;
	int ernum2 = 0;
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
	}
	cout << "begin query!" << endl;
	for (int i = 0; i < cnt; ++i) {
		if (mp[trans(edge[i])] != hole.query(edge[i])) {
			++ernum2;
		}
	}
	cout << "sum:  " << asum << endl;
	cout << "error num 2 :   " << ernum2 << endl;
	delete[] edge;
}
void hole_test_th_multi() {
	Edge edge;
	count_type cnt;
	int mul = 3;
	double insert[10];
	double query[10];
	for (int i = 0; i < 10; ++i) {
		insert[i] = 0;
		query[i] = 0;
	}
	cnt = load_data13_CAIDA();
	cout << "cnt: " << cnt << endl;
	int fin = 27000;
	set<key_type> ed;
	for (count_type i = 0; i < fin * 1000; ++i)
		ed.insert(asrc[i]);
	int edsize = ed.size();
	ofstream fout;
	fout.open("hole_test_th_multi.txt", ios::out);
	for (int num = 3000; num <= fin; num += 6000) {
		cnt = num * 1000;
		fout << "DATA SIZE: " << cnt << endl;
		for (int i = 0; i < mul; ++i) {
			int l2 = (edsize / (3 * bup * brate)) + 1;
			if (l2 % 2)l2 = l2 + 1;
			int l1 = l2 * 2;
			cout << "l1:  " << l1 << endl;
			cout << "l2:  " << l2 << endl;
			sblacklist.clear();
			holemerge hole(bl1, bl2, 0, 221);
			int sum = 0;
			clock_t time_start = clock();
			for (int i = 0; i < cnt; ++i) {
				edge.src = asrc[i];
				edge.dst = adst[i];
				sum += hole.insert(edge);
			}
			clock_t time_end = clock();
			long time = time_end - time_start;
			insert[i] = (double)cnt / (double)time;
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
			query[i] = (double)cnt / (double)time3;
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
		fout << "average_insert:     " << average_insert << "   MIPS" << endl;
		fout << "average_query:      " << average_query << "   MQPS" << endl;
	}
	fout.close();
}
void test_hole_test_bl() {
	int small = 0;
	int big = 0;
	sblacklist.clear();
	count_type cnt;
	Edge* edge = read_sina_data(&cnt);
	holemerge hole(bl1, bl2, 0, 221);
	int sum = 0;
	auto t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i]);
		if (sblacklist.size() > sbl)small++;
		if (hole.blacklist.size() > bbl)big++;
	}
	auto t2 = steady_clock::now();
	auto t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Insert throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	cout << "small:  " << small << endl;
	cout << "big:  " << big << endl;
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
void test_wind_th() {
	cout << "TEST WIN TH!" << endl;
	chain* tempC = new chain();
	int csl = 400;
	Chandelier* wind;
	wind = new Chandelier(csl);
	count_type cnt;
	Edge* edge = read_wiki_data(&cnt, 100000000);
	int sum = 0;
	auto t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		chain* c = new chain();
		c->src = edge[i].src;
		c->dst = edge[i].dst;
		c->pointer_chand = nullptr;
		wind->insert(c);
	}
	auto t2 = steady_clock::now();
	auto t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Insert throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	t1 = steady_clock::now();
	for (int i = 0; i < cnt; ++i) {
		tempC->src = edge[i].src;
		tempC->dst = edge[i].dst;
		sum = sum + wind->query_edge(tempC);
	}
	t2 = steady_clock::now();
	t3 = duration_cast<microseconds>(t2 - t1).count();
	cout << "Query throughput: " << cnt / (1.0 * t3) << " MIPs" << endl;
	cout << "sum: " << sum << endl;
	delete[] edge;
}
void test_weight_wind_err2() {
	count_type cnt;
	TUPLES* items = read_data((folder + filenames[0]).c_str(), 100000000, &cnt);
	std::cout << "The number of packet:" << cnt << std::endl;
	Edge* edge = new Edge[cnt];
	for (count_type i = 0; i < cnt; ++i)
		memcpy(&edge[i], &items[i], sizeof(Edge));
	unordered_map< long long, count_type > mp;
	for (int l = 0; l < cnt; ++l) {
		if (mp.find(trans(edge[l])) == mp.end())
			mp[trans(edge[l])] = 1;
		else
			mp[trans(edge[l])] += 1;
	}
	chain* tempC = new chain();
	int csl = 400;
	Chandelier* wind;
	wind = new Chandelier(csl);
	asum = 0;
	int ernum = 0;
	int ernum2 = 0;
	for (int i = 0; i < cnt; ++i) {
		chain* c = new chain();
		c->src = edge[i].src;
		c->dst = edge[i].dst;
		c->pointer_chand = nullptr;
		wind->insert(c);
	}
	cout << "begin query!" << endl;
	for (int i = 0; i < cnt; ++i) {
		tempC->src = edge[i].src;
		tempC->dst = edge[i].dst;
		if (mp[trans(edge[i])] != wind->query_edge(tempC)) {
			++ernum2;
		}
	}
	cout << "sum:  " << asum << endl;
	cout << "error num 2 :   " << ernum2 << endl;
	delete[] items;
	delete[] edge;
}
void test_sssp_hole() {
	holemerge hole(bl1, bl2, 0, 221);
	count_type cnt;
	Edge* edge = read_caida_data(&cnt, 100000000);
	int mul = 3;
	int maxtopk = 4000;
	int rd = 10;
	key_type topk[10001] = { 0 };
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	unordered_map<key_type, key_type> node;
	ofstream fout;
	ofstream fr1;
	ofstream fr2;
	ofstream fr3;
	fout.open("test_sssp_hole.txt", ios::out);
	fr1.open("fr1.txt", ios::out);
	fr2.open("fr2.txt", ios::out);
	fr3.open("fr3.txt", ios::out);
	for (int it = 0; it < cnt; ++it) {
		hole.insert(edge[it]);
		Edge EE;
		EE.src = edge[it].src;
		EE.dst = edge[it].dst;
		if (ed.count(EE))continue;
		ed.insert(EE);
		edg.insert(EE.dst);
		edg.insert(EE.src);
		if (node.find(EE.src) != node.end()) node[EE.src] = node[EE.src] + 1;
		else node.insert(pair<key_type, key_type>(EE.src, 1));
		if (node.find(EE.dst) != node.end()) node[EE.dst] = node[EE.dst] + 1;
		else node.insert(pair<key_type, key_type>(EE.dst, 1));
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
	unordered_map<key_type, uint32_t> dist;
	uint32_t INF = 0;
	INF = ~INF;
	cout << "INF:  " << INF << endl;
	for (uint32_t TT = 1; TT <= 8; ++TT) {
		uint32_t num = 500 * TT;
		fout << "num:     " << num << endl;
		double average;
		average = 0;
		for (uint32_t hh = 0; hh < mul; ++hh) {
			long time = 0;
			for (uint32_t i = 0; i < rd; ++i) {
				flag.clear();
				dist.clear();
				clock_t time_start = clock();
				key_type vs = topk[i];
				key_type k;
				uint32_t tmp;
				for (uint32_t it = 0; it < num; ++it) {
					flag.insert(pair<key_type, bool>(topk[it], false));
					uint32_t mmm;
					Edge qen;
					qen.src = vs;
					qen.dst = topk[it];
					uint32_t we = hole.query(qen);
					if (we)mmm = we;
					else mmm = INF;
					dist.insert(pair<key_type, uint32_t>(topk[it], mmm));
				}
				flag[vs] = true;
				dist[vs] = 0;
				for (uint32_t ii = 1; ii < num; ++ii) {
					uint32_t min = INF;
					bool fff = false;
					for (uint32_t it = 0; it < num; ++it) {
						if (flag[topk[it]] == false && (fff == false || dist[topk[it]] < min))
						{
							min = dist[topk[it]];
							k = topk[it];
							fff = true;
						}
					}
					flag[k] = true;
					if (min == INF)break;
					for (uint32_t it = 0; it < num; ++it) {
						Edge qen;
						qen.src = vs;
						qen.dst = topk[it];
						uint32_t we = hole.query(qen);
						if (we)tmp = min + we;
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
			fout << "average time: " << (double)time / (double)rd << endl;
			average = average + (double)time / (double)rd;
		}
		average = average / (double)mul;
		fout << "average average time: " << average << endl;
		cout << "end" << endl;
	}
	fout.close();
	fr1.close();
	fr2.close();
	fr3.close();
	delete[] edge;
}
void test_sssp_wind() {
	chain* tempC = new chain();
	int csl = 400;
	Chandelier* wind;
	wind = new Chandelier(csl);
	count_type cnt;
	Edge* edge = read_caida_data(&cnt, 100000000);
	int mul = 3;
	int maxtopk = 4000;
	int rd = 10;
	key_type topk[10001] = { 0 };
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	unordered_map<key_type, key_type> node;
	ofstream fout;
	ofstream fr1;
	ofstream fr2;
	ofstream fr3;
	fout.open("test_ssspwind.txt", ios::out);
	fr1.open("fr1.txt", ios::out);
	fr2.open("fr2.txt", ios::out);
	fr3.open("fr3.txt", ios::out);
	for (int it = 0; it < cnt; ++it) {
		chain* c = new chain();
		c->src = edge[it].src;
		c->dst = edge[it].dst;
		c->pointer_chand = nullptr;
		wind->insert(c);
		Edge EE;
		EE.src = edge[it].src;
		EE.dst = edge[it].dst;
		if (ed.count(EE))continue;
		ed.insert(EE);
		edg.insert(EE.dst);
		edg.insert(EE.src);
		if (node.find(EE.src) != node.end()) node[EE.src] = node[EE.src] + 1;
		else node.insert(pair<key_type, key_type>(EE.src, 1));
		if (node.find(EE.dst) != node.end()) node[EE.dst] = node[EE.dst] + 1;
		else node.insert(pair<key_type, key_type>(EE.dst, 1));
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
	unordered_map<key_type, uint32_t> dist;
	uint32_t INF = 0;
	INF = ~INF;
	cout << "INF:  " << INF << endl;
	for (uint32_t TT = 1; TT <= 8; ++TT) {
		uint32_t num = 500 * TT;
		fout << "num:     " << num << endl;
		double average;
		average = 0;
		for (uint32_t hh = 0; hh < mul; ++hh) {
			long time = 0;
			for (uint32_t i = 0; i < rd; ++i) {
				flag.clear();
				dist.clear();
				clock_t time_start = clock();
				key_type vs = topk[i];
				key_type k;
				uint32_t tmp;
				for (uint32_t it = 0; it < num; ++it) {
					flag.insert(pair<key_type, bool>(topk[it], false));
					uint32_t mmm;
					tempC->src = vs;
					tempC->dst = topk[it];
					tempC->pointer_chand = nullptr;
					uint32_t we = wind->query_edge(tempC);
					if (we)mmm = we;
					else mmm = INF;
					dist.insert(pair<key_type, uint32_t>(topk[it], mmm));
				}
				flag[vs] = true;
				dist[vs] = 0;
				for (uint32_t ii = 1; ii < num; ++ii) {
					uint32_t min = INF;
					bool fff = false;
					for (uint32_t it = 0; it < num; ++it) {
						if (flag[topk[it]] == false && (fff == false || dist[topk[it]] < min))
						{
							min = dist[topk[it]];
							k = topk[it];
							fff = true;
						}
					}
					flag[k] = true;
					if (min == INF)break;
					for (uint32_t it = 0; it < num; ++it) {
						tempC->src = vs;
						tempC->dst = topk[it];
						tempC->pointer_chand = nullptr;
						uint32_t we = wind->query_edge(tempC);
						if (we)tmp = min + we;
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
			fout << "average time: " << (double)time / (double)rd << endl;
			average = average + (double)time / (double)rd;
		}
		average = average / (double)mul;
		fout << "average average time: " << average << endl;
		cout << "end" << endl;
	}
	fout.close();
	fr1.close();
	fr2.close();
	fr3.close();
	delete[] edge;
	delete wind;
}
long hole_Bfs(holemerge* g, VertexType src)
{
	clock_t time_start = clock();
	uint32_t node_num = 0;
	vector<VertexType> bfs_node;
	std::unordered_map<VertexType, bool> visited;
	std::queue<VertexType> q;
	q.push(src);
	bfs_node.push_back(src);
	++node_num;
	visited[src] = true;
	while (q.size()) {
		VertexType top = q.front();
		q.pop();
		auto temp = g->GetNeighbors(top);
		if (temp.size() == 0) {
			continue;
		}
		for (const auto& edge : temp) {
			if (!visited[edge.first]) {
				q.push(edge.first);
				visited[edge.first] = true;
				bfs_node.push_back(edge.first);
				++node_num;
			}
		}
	}
	clock_t time_end = clock();
	ofstream fr1;
	fr1.open("hole_Bfs_fr1.txt", ios::app);
	for (auto it = bfs_node.begin(); it != bfs_node.end(); ++it)
		fr1 << *it << endl;
	fr1 << "node_num:   " << node_num << endl;
	fr1.close();
	long time = (time_end - time_start);
	return time;
}
void hole_test_bfs() {
	cout << "test CuckooGraph BFS begin!" << endl;
	holemerge hole(bl1, bl2, 0, 221);
	count_type cnt;
	Edge* edge = read_stackoverflow_data(&cnt, 100000000);
	int maxtopk = 1003;
	int R = 50;
	key_type topk[10001] = { 0 };
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	unordered_map<key_type, key_type> node;
	for (int it = 0; it < cnt; ++it) {
		hole.insert(edge[it]);
		Edge EE;
		EE.src = asrc[it];
		EE.dst = adst[it];
		if (ed.count(EE))continue;
		ed.insert(EE);
		edg.insert(EE.dst);
		edg.insert(EE.src);
		if (node.find(EE.src) != node.end()) node[EE.src] = node[EE.src] + 1;
		else node.insert(pair<key_type, key_type>(EE.src, 1));
		if (node.find(EE.dst) != node.end()) node[EE.dst] = node[EE.dst] + 1;
		else node.insert(pair<key_type, key_type>(EE.dst, 1));
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
	double average_time = 0;
	for (int i = 0; i < R; ++i) {
		long time = hole_Bfs(&hole, topk[i]);
		double ttt = (double)time;
		ttt = ttt / R;
		average_time = average_time + ttt;
	}
	cout << "average_time:    " << average_time << endl;
}
long wind_Bfs(Chandelier* g, VertexType src)
{
	clock_t time_start = clock();
	uint32_t node_num = 0;
	vector<VertexType> bfs_node;
	std::unordered_map<VertexType, bool> visited;
	std::queue<VertexType> q;
	q.push(src);
	bfs_node.push_back(src);
	++node_num;
	visited[src] = true;
	while (q.size()) {
		VertexType top = q.front();
		q.pop();
		auto temp = g->GetNeighbors(top);
		if (temp.size() == 0) {
			continue;
		}
		for (const auto& edge : temp) {
			if (!visited[edge.first]) {
				q.push(edge.first);
				visited[edge.first] = true;
				bfs_node.push_back(edge.first);
				++node_num;
			}
		}
	}
	clock_t time_end = clock();
	ofstream fr1;
	fr1.open("wind_Bfs_fr1.txt", ios::app);
	for (auto it = bfs_node.begin(); it != bfs_node.end(); ++it)
		fr1 << *it << endl;
	fr1 << "node_num:   " << node_num << endl;
	fr1.close();
	long time = (time_end - time_start);
	return time;
}
void wind_test_bfs() {
	cout << "test WindBell BFS begin!" << endl;
	chain* tempC = new chain();
	int csl = 400;
	Chandelier* wind;
	wind = new Chandelier(csl);
	count_type cnt;
	Edge* edge = read_stackoverflow_data(&cnt, 100000000);
	int maxtopk = 1003;
	int R = 50;
	key_type topk[10001] = { 0 };
	set<key_type> edg;
	set<Edge, EdgeLess> ed;
	unordered_map<key_type, key_type> node;
	for (int it = 0; it < cnt; ++it) {
		chain* c = new chain();
		c->src = edge[it].src;
		c->dst = edge[it].dst;
		c->pointer_chand = nullptr;
		wind->insert(c);
		Edge EE;
		EE.src = asrc[it];
		EE.dst = adst[it];
		if (ed.count(EE))continue;
		ed.insert(EE);
		edg.insert(EE.dst);
		edg.insert(EE.src);
		if (node.find(EE.src) != node.end()) node[EE.src] = node[EE.src] + 1;
		else node.insert(pair<key_type, key_type>(EE.src, 1));
		if (node.find(EE.dst) != node.end()) node[EE.dst] = node[EE.dst] + 1;
		else node.insert(pair<key_type, key_type>(EE.dst, 1));
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
	double average_time = 0;
	for (int i = 0; i < R; ++i) {
		long time = wind_Bfs(wind, topk[i]);
		double ttt = (double)time;
		ttt = ttt / R;
		average_time = average_time + ttt;
	}
	cout << "average_time:    " << average_time << endl;
}
void make_rand() {
	int sample_num = 10000;
	ofstream fr1;
	fr1.open("rand.txt", ios::out);
	for (int i = 0; i < sample_num; ++i)
		fr1 << rand() << endl;
	fr1.close();
}
void test_cukoograph_triangles() {
	holemerge hole(bl1, bl2, 0, 221);
	int mul = 3;
	int sample_num = 10000;
	int ran[33333] = { 0 };
	cout << "test cuckoograph triangles begin!" << endl;
	count_type cnt;
	read_caida_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();
	ofstream fout;
	fout.open("test_cuckoograph_tiangles.txt", ios::out);
	int inum = 0;
	int sum1 = 0;
	int sum2 = 0;
	ifstream infile;
	infile.open("./rand.txt");
	int intbig = 2147483647;
	for (int i = 0; i < sample_num; ++i) {
		char s[50];
		if (!infile.getline(s, intbig))assert(0);
		string nu(s);
		ran[i] = stoi(nu);
	}
	for (int it = 0; it < cnt; ++it) {
		Edge EE;
		EE.src = asrc[it];
		EE.dst = adst[it];
		hole.insert(EE);
		edg.insert(EE.dst);
		edg.insert(EE.src);
	}
	key_type* edge = new key_type[edg.size()];
	int number = 0;
	for (auto it = edg.begin(); it != edg.end(); ++it) {
		edge[number] = *it;
		++number;
	}
	double ours_average = 0;
	for (int mm = 0; mm < mul; ++mm) {
		clock_t time_start_2 = clock();
		for (int i = 0; i < sample_num; ++i) {
			sum2 += hole.query_triangles(edge[ran[i] % edg.size()]);
		}
		clock_t time_end_2 = clock();
		long time3 = time_end_2 - time_start_2;
		fout << "ours average time: " << (double)time3 / (double)sample_num << endl;
		ours_average += (double)time3 / (double)sample_num;
		cout << "ours end" << endl;
	}
	ours_average = ours_average / (double)mul;
	fout << "Ours Average Average time: " << ours_average << endl;
	cout << "sum2: " << sum2 << endl;
	fout.close();
	infile.close();
	delete[] edge;
}
void test_wind_triangles() {
	int csl = 400;
	Chandelier* wind;
	wind = new Chandelier(csl);
	int mul = 3;
	int sample_num = 10000;
	int ran[33333] = { 0 };
	cout << "test wind triangles begin!" << endl;
	count_type cnt;
	read_caida_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();
	ofstream fout;
	fout.open("test_wind_tiangles.txt", ios::out);
	int inum = 0;
	int sum1 = 0;
	int sum2 = 0;
	ifstream infile;
	infile.open("./rand.txt");
	int intbig = 2147483647;
	for (int i = 0; i < sample_num; ++i) {
		char s[50];
		if (!infile.getline(s, intbig))assert(0);
		string nu(s);
		ran[i] = stoi(nu);
	}
	for (int it = 0; it < cnt; ++it) {
		Edge EE;
		EE.src = asrc[it];
		EE.dst = adst[it];
		chain* c = new chain();
		c->src = EE.src;
		c->dst = EE.dst;
		c->pointer_chand = nullptr;
		wind->insert(c);
		edg.insert(EE.dst);
		edg.insert(EE.src);
	}
	key_type* edge = new key_type[edg.size()];
	int number = 0;
	for (auto it = edg.begin(); it != edg.end(); ++it) {
		edge[number] = *it;
		++number;
	}
	double ours_average = 0;
	for (int mm = 0; mm < mul; ++mm) {
		clock_t time_start_2 = clock();
		for (int i = 0; i < sample_num; ++i) {
			sum2 += wind->query_triangle_new(edge[ran[i] % edg.size()]);
		}
		clock_t time_end_2 = clock();
		long time3 = time_end_2 - time_start_2;
		fout << "wind average time: " << (double)time3 / (double)sample_num << endl;
		ours_average += (double)time3 / (double)sample_num;
		cout << "wind end" << endl;
	}
	ours_average = ours_average / (double)mul;
	fout << "Wind Average Average time: " << ours_average << endl;
	cout << "sum2: " << sum2 << endl;
	fout.close();
	infile.close();
	delete[] edge;
}
void PageRank_CuckooGraph()
{
	double d = 0.85;
	double e = 0.1;
	int maxIterations = 100;
	holemerge hole(bl1, bl2, 0, 221);
	cout << "test cuckoograph PR begin!" << endl;
	count_type cnt;
	read_stackoverflow_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();
	cnt = 30;
	for (int it = 0; it < cnt; ++it) {
		Edge EE;
		EE.src = asrc[it];
		EE.dst = adst[it];
		hole.insert(EE);
		edg.insert(EE.dst);
		edg.insert(EE.src);
	}
	int N = edg.size();
	cout << "insert ok!" << endl;
	clock_t time_start_2 = clock();
	vector<vector<double>> M(N, vector<double>(N, 0.0));
	for (int i = 0; i < N; i++)
	{
		key_type src = i + 1;
		auto temp = hole.GetNeighbors(src);
		if (temp.size() != 0)
		{
			for (const auto& EEE : temp) {
				++M[EEE.first - 1][i];
			}
			for (int j = 0; j < N; j++)
			{
				M[j][i] /= temp.size();
			}
		}
		else
		{
			for (const auto& EEE : temp)
			{
				M[EEE.first - 1][i] = 0;
			}
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			M[i][j] = d * M[i][j] + (1 - d) * (1.0 / N);
		}
	}
	vector<double> pagerank(N, 1.0);
	while (maxIterations)
	{
		vector<double> newpagerank(N, 0.0);
		double dif = 0.0;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				newpagerank[i] += M[i][j] * pagerank[j];
			}
			dif += abs(pagerank[i] - newpagerank[i]);
		}
		pagerank = newpagerank;
		if (dif < e)
		{
			break;
		}
		maxIterations--;
	}
	clock_t time_end_2 = clock();
	long time = time_end_2 - time_start_2;
	cout << "time:  " << time << endl;
	ofstream fr1;
	fr1.open("PR_fr1.txt", ios::out);
	for (int i = 0; i < N; i++)
	{
		fr1 << pagerank[i] << endl;
	}
	fr1.close();
}
void PageRank_CuckooGraph2()
{
	double d = 0.85;
	double e = 0.1;
	int maxIterations = 100;
	holemerge hole(bl1, bl2, 0, 221);
	cout << "test cuckoograph PR begin!" << endl;
	count_type cnt;
	read_stackoverflow_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();
	for (int it = 0; it < cnt; ++it) {
		Edge EE;
		EE.src = asrc[it];
		EE.dst = adst[it];
		hole.insert(EE);
		edg.insert(EE.dst);
		edg.insert(EE.src);
	}
	int N = edg.size();
	cout << "insert ok!" << endl;
	int* jj = new int[N];
	for (int j = 0; j < N; ++j) {
		key_type src = j + 1;
		auto temp = hole.GetNeighbors(src);
		jj[j] = temp.size();
	}
	cout << "GetNeighbors OK!" << endl;
	clock_t time_start_2 = clock();
	vector<double> pagerank(N, 1.0);
	while (maxIterations)
	{
		vector<double> newpagerank(N, 0.0);
		double dif = 0.0;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				double sz = jj[j];
				double M = 0;
				Edge e;
				e.src = j + 1;
				e.dst = i + 1;
				if (hole.query(e)) {
					++M;
					M = M / sz;
				}
				M = d * M + (1 - d) * (1.0 / N);
				newpagerank[i] += M * pagerank[j];
			}
			dif += abs(pagerank[i] - newpagerank[i]);
		}
		pagerank = newpagerank;
		if (dif < e)
		{
			break;
		}
		maxIterations--;
	}
	clock_t time_end_2 = clock();
	long time = time_end_2 - time_start_2;
	cout << "time:  " << time << endl;
	ofstream fr1;
	fr1.open("PR_fr2.txt", ios::out);
	for (int i = 0; i < N; i++)
	{
		fr1 << pagerank[i] << endl;
	}
	fr1.close();
	delete[] jj;
}
class CuckooGraph_tarjan {
public:
	holemerge hole;
	int index;
	vector<int> low;
	vector<bool> instack;
	int scc_count;
	stack<key_type> stk;
	vector<vector<key_type>  > scc_list;
	void tarjan(key_type v) {
		int i;
		int dfn = index;
		low[v] = index++;
		stk.push(v);
		instack[v] = true;
		auto temp = hole.GetNeighbors(v);
		for (const auto& EEE : temp) {
			key_type u = EEE.first;
			if (low[u] == -1) {
				tarjan(u);
				if (low[u] < low[v]) {
					low[v] = low[u];
				}
			}
			else if (instack[u]) {
				if (low[u] < low[v]) {
					low[v] = low[u];
				}
			}
		}
		if (low[v] == dfn) {
			key_type j;
			do {
				j = stk.top();
				stk.pop();
				instack[j] = false;
				scc_list[scc_count].push_back(j);
			} while (j != v);
			scc_count++;
			vector<key_type> v1;
			scc_list.push_back(v1);
		}
	}
	void tarjan_algorithm(int node_num) {
		index = 0;
		scc_count = 0;
		for (int i = 1; i <= node_num; i++) {
			if (low[i] == -1) {
				tarjan(i);
			}
		}
	}
	CuckooGraph_tarjan() : hole(bl1, bl2, 0, 221), index(0), scc_count(0)
	{
		low.clear();
		instack.clear();
		scc_list.clear();
		while (!stk.empty())stk.pop();
	}
	void test_CuckooGraph_tarjan() {
		count_type cnt;
		Edge* edge = read_caida_data(&cnt, 100000000);
		int mul = 3;
		int maxtopk = 4000;
		int rd = 10;
		key_type topk[10001] = { 0 };
		set<key_type> edg;
		set<Edge, EdgeLess> ed;
		unordered_map<key_type, key_type> node;
		ofstream fout;
		fout.open("test_CC.txt", ios::out);
		for (int it = 0; it < cnt; ++it) {
			Edge EE;
			EE.src = edge[it].src;
			EE.dst = edge[it].dst;
			if (ed.count(EE))continue;
			ed.insert(EE);
			edg.insert(EE.dst);
			edg.insert(EE.src);
			if (node.find(EE.src) != node.end()) node[EE.src] = node[EE.src] + 1;
			else node.insert(pair<key_type, key_type>(EE.src, 1));
			if (node.find(EE.dst) != node.end()) node[EE.dst] = node[EE.dst] + 1;
			else node.insert(pair<key_type, key_type>(EE.dst, 1));
		}
		unordered_map<key_type, key_type> mp;
		vector<Node_degree> nd;
		for (auto it = node.begin(); it != node.end(); ++it) {
			Node_degree dd;
			dd.node = it->first;
			dd.val = it->second;
			nd.push_back(dd);
		}
		make_heap(nd.begin(), nd.end(), Compare);
		low.push_back(-1);
		instack.push_back(0);
		vector<key_type> v3;
		scc_list.push_back(v3);
		for (int i = 0; i < maxtopk; ++i) {
			topk[i] = nd[0].node;
			pop_heap(nd.begin(), nd.end(), Compare);
			nd.pop_back();
			mp.insert({ topk[i],i + 1 });
			low.push_back(-1);
			instack.push_back(false);
		}
		for (int it = 0; it < cnt; ++it) {
			if (mp.count(asrc[it]) && mp.count(adst[it]))
			{
				Edge EE;
				EE.src = mp[asrc[it]];
				EE.dst = mp[adst[it]];
				hole.insert(EE);
			}
		}
		clock_t time_start_2 = clock();
		tarjan_algorithm(maxtopk);
		clock_t time_end_2 = clock();
		long time = time_end_2 - time_start_2;
		cout << "time:  " << time << endl;
		cout << "scc_count: " << scc_count << endl;
		for (int i = 0; i < scc_count; i++) {
			fout << "SCC " << i << ": ";
			for (int j = 0; j < scc_list[i].size(); j++) {
				fout << scc_list[i][j] << " ";
			}
			fout << endl;
		}
		fout.close();
		delete[] edge;
	}
};
class wind_tarjan {
public:
	Chandelier* wind;
	int index;
	vector<int> low;
	vector<bool> instack;
	int scc_count;
	stack<key_type> stk;
	vector<vector<key_type>  > scc_list;
	void tarjan(key_type v) {
		int i;
		int dfn = index;
		low[v] = index++;
		stk.push(v);
		instack[v] = true;
		auto temp = wind->GetNeighbors(v);
		for (const auto& EEE : temp) {
			key_type u = EEE.first;
			if (low[u] == -1) {
				tarjan(u);
				if (low[u] < low[v]) {
					low[v] = low[u];
				}
			}
			else if (instack[u]) {
				if (low[u] < low[v]) {
					low[v] = low[u];
				}
			}
		}
		if (low[v] == dfn) {
			key_type j;
			do {
				j = stk.top();
				stk.pop();
				instack[j] = false;
				scc_list[scc_count].push_back(j);
			} while (j != v);
			scc_count++;
			vector<key_type> v1;
			scc_list.push_back(v1);
		}
	}
	void tarjan_algorithm(int node_num) {
		index = 0;
		scc_count = 0;
		for (int i = 1; i <= node_num; i++) {
			if (low[i] == -1) {
				tarjan(i);
			}
		}
	}
	wind_tarjan() : index(0), scc_count(0)
	{
		wind = new Chandelier(400);
		low.clear();
		instack.clear();
		scc_list.clear();
		while (!stk.empty())stk.pop();
	}
	void test_wind_tarjan() {
		count_type cnt;
		Edge* edge = read_caida_data(&cnt, 100000000);
		int mul = 3;
		int maxtopk = 4000;
		int rd = 10;
		key_type topk[10001] = { 0 };
		set<key_type> edg;
		set<Edge, EdgeLess> ed;
		unordered_map<key_type, key_type> node;
		ofstream fout;
		fout.open("test_CC.txt", ios::out);
		for (int it = 0; it < cnt; ++it) {
			Edge EE;
			EE.src = edge[it].src;
			EE.dst = edge[it].dst;
			if (ed.count(EE))continue;
			ed.insert(EE);
			edg.insert(EE.dst);
			edg.insert(EE.src);
			if (node.find(EE.src) != node.end()) node[EE.src] = node[EE.src] + 1;
			else node.insert(pair<key_type, key_type>(EE.src, 1));
			if (node.find(EE.dst) != node.end()) node[EE.dst] = node[EE.dst] + 1;
			else node.insert(pair<key_type, key_type>(EE.dst, 1));
		}
		unordered_map<key_type, key_type> mp;
		vector<Node_degree> nd;
		for (auto it = node.begin(); it != node.end(); ++it) {
			Node_degree dd;
			dd.node = it->first;
			dd.val = it->second;
			nd.push_back(dd);
		}
		make_heap(nd.begin(), nd.end(), Compare);
		low.push_back(-1);
		instack.push_back(0);
		vector<key_type> v3;
		scc_list.push_back(v3);
		for (int i = 0; i < maxtopk; ++i) {
			topk[i] = nd[0].node;
			pop_heap(nd.begin(), nd.end(), Compare);
			nd.pop_back();
			mp.insert({ topk[i],i + 1 });
			low.push_back(-1);
			instack.push_back(false);
		}
		for (int it = 0; it < cnt; ++it) {
			if (mp.count(asrc[it]) && mp.count(adst[it]))
			{
				chain* c = new chain();
				c->src = mp[asrc[it]];
				c->dst = mp[adst[it]];
				c->pointer_chand = nullptr;
				wind->insert(c);
			}
		}
		clock_t time_start_2 = clock();
		tarjan_algorithm(maxtopk);
		clock_t time_end_2 = clock();
		long time = time_end_2 - time_start_2;
		cout << "time:  " << time << endl;
		cout << "scc_count: " << scc_count << endl;
		for (int i = 0; i < scc_count; i++) {
			fout << "SCC " << i << ": ";
			for (int j = 0; j < scc_list[i].size(); j++) {
				fout << scc_list[i][j] << " ";
			}
			fout << endl;
		}
		fout.close();
		delete[] edge;
	}
};
void PageRank_wind_final()
{
	Chandelier* wind;
	wind = new Chandelier(400);
	unordered_map<key_type, key_type> node;
	int mul = 3;
	int maxtopk = 1000;
	int rd = 10;
	key_type topk[10001] = { 0 };
	double d = 0.85;
	double e = 0.1;
	int maxIterations = 100;
	cout << "test wind PR begin!" << endl;
	count_type cnt;
	Edge* edge = read_caida_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();
	for (int it = 0; it < cnt; ++it) {
		Edge EE;
		EE.src = edge[it].src;
		EE.dst = edge[it].dst;
		if (ed.count(EE))continue;
		ed.insert(EE);
		edg.insert(EE.dst);
		edg.insert(EE.src);
		if (node.find(EE.src) != node.end()) node[EE.src] = node[EE.src] + 1;
		else node.insert(pair<key_type, key_type>(EE.src, 1));
		if (node.find(EE.dst) != node.end()) node[EE.dst] = node[EE.dst] + 1;
		else node.insert(pair<key_type, key_type>(EE.dst, 1));
	}
	unordered_map<key_type, key_type> mp;
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
		mp.insert({ topk[i],i + 1 });
	}
	for (int it = 0; it < cnt; ++it) {
		if (mp.count(asrc[it]) && mp.count(adst[it]))
		{
			chain* c = new chain();
			c->src = mp[asrc[it]];
			c->dst = mp[adst[it]];
			c->pointer_chand = nullptr;
			wind->insert(c);
		}
	}
	int N = maxtopk;
	cout << "insert ok!" << endl;
	clock_t time_start_2 = clock();
	vector<vector<double>> M(N, vector<double>(N, 0.0));
	for (int i = 0; i < N; i++)
	{
		key_type src = i + 1;
		auto temp = wind->GetNeighbors(src);
		if (temp.size() != 0)
		{
			for (const auto& EEE : temp) {
				++M[EEE.first - 1][i];
			}
			for (int j = 0; j < N; j++)
			{
				M[j][i] /= temp.size();
			}
		}
		else
		{
			for (const auto& EEE : temp)
			{
				M[EEE.first - 1][i] = 0;
			}
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			M[i][j] = d * M[i][j] + (1 - d) * (1.0 / N);
		}
	}
	vector<double> pagerank(N, 1.0);
	while (maxIterations)
	{
		vector<double> newpagerank(N, 0.0);
		double dif = 0.0;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				newpagerank[i] += M[i][j] * pagerank[j];
			}
			dif += abs(pagerank[i] - newpagerank[i]);
		}
		pagerank = newpagerank;
		if (dif < e)
		{
			break;
		}
		maxIterations--;
	}
	clock_t time_end_2 = clock();
	long time = time_end_2 - time_start_2;
	cout << "time:  " << time << endl;
	ofstream fr1;
	fr1.open("PR_fr_wind.txt", ios::out);
	for (int i = 0; i < N; i++)
	{
		fr1 << pagerank[i] << endl;
	}
	fr1.close();
	delete[] edge;
	delete wind;
}
void PageRank_CuckooGraph_final()
{
	unordered_map<key_type, key_type> node;
	int mul = 3;
	int maxtopk = 1000;
	int rd = 10;
	key_type topk[10001] = { 0 };
	double d = 0.85;
	double e = 0.1;
	int maxIterations = 100;
	holemerge hole(bl1, bl2, 0, 221);
	cout << "test cuckoograph PR begin!" << endl;
	count_type cnt;
	Edge* edge = read_caida_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();
	for (int it = 0; it < cnt; ++it) {
		Edge EE;
		EE.src = edge[it].src;
		EE.dst = edge[it].dst;
		if (ed.count(EE))continue;
		ed.insert(EE);
		edg.insert(EE.dst);
		edg.insert(EE.src);
		if (node.find(EE.src) != node.end()) node[EE.src] = node[EE.src] + 1;
		else node.insert(pair<key_type, key_type>(EE.src, 1));
		if (node.find(EE.dst) != node.end()) node[EE.dst] = node[EE.dst] + 1;
		else node.insert(pair<key_type, key_type>(EE.dst, 1));
	}
	unordered_map<key_type, key_type> mp;
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
		mp.insert({ topk[i],i + 1 });
	}
	for (int it = 0; it < cnt; ++it) {
		if (mp.count(asrc[it]) && mp.count(adst[it]))
		{
			Edge EE;
			EE.src = mp[asrc[it]];
			EE.dst = mp[adst[it]];
			hole.insert(EE);
		}
	}
	int N = maxtopk;
	cout << "insert ok!" << endl;
	clock_t time_start_2 = clock();
	vector<vector<double>> M(N, vector<double>(N, 0.0));
	for (int i = 0; i < N; i++)
	{
		key_type src = i + 1;
		auto temp = hole.GetNeighbors(src);
		if (temp.size() != 0)
		{
			for (const auto& EEE : temp) {
				++M[EEE.first - 1][i];
			}
			for (int j = 0; j < N; j++)
			{
				M[j][i] /= temp.size();
			}
		}
		else
		{
			for (const auto& EEE : temp)
			{
				M[EEE.first - 1][i] = 0;
			}
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			M[i][j] = d * M[i][j] + (1 - d) * (1.0 / N);
		}
	}
	vector<double> pagerank(N, 1.0);
	while (maxIterations)
	{
		vector<double> newpagerank(N, 0.0);
		double dif = 0.0;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				newpagerank[i] += M[i][j] * pagerank[j];
			}
			dif += abs(pagerank[i] - newpagerank[i]);
		}
		pagerank = newpagerank;
		if (dif < e)
		{
			break;
		}
		maxIterations--;
	}
	clock_t time_end_2 = clock();
	long time = time_end_2 - time_start_2;
	cout << "time:  " << time << endl;
	ofstream fr1;
	fr1.open("PR_fr_cuckoograph.txt", ios::out);
	for (int i = 0; i < N; i++)
	{
		fr1 << pagerank[i] << endl;
	}
	fr1.close();
	delete[] edge;
}
void ours_Lcc()
{
	unordered_map<key_type, key_type> node;
	key_type topk[5000001] = { 0 };
	holemerge hole(bl1, bl2, 0, 221);
	int maxtopk = 10000;
	count_type cnt;
	Edge* edge = read_caida_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();
	for (int it = 0; it < cnt; ++it) {
		Edge EE;
		EE.src = edge[it].src;
		EE.dst = edge[it].dst;
		if (ed.count(EE))continue;
		ed.insert(EE);
		edg.insert(EE.dst);
		edg.insert(EE.src);
		if (node.find(EE.src) != node.end()) node[EE.src] = node[EE.src] + 1;
		else node.insert(pair<key_type, key_type>(EE.src, 1));
		if (node.find(EE.dst) != node.end()) node[EE.dst] = node[EE.dst] + 1;
		else node.insert(pair<key_type, key_type>(EE.dst, 1));
	}
	unordered_map<key_type, key_type> mp;
	std::set<VertexType> vertex;
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
		mp.insert({ topk[i],i + 1 });
	}
	edg.clear(); ed.clear();
	int i = 0;
	for (int it = 0; it < cnt; ++it) {
		if (mp.count(edge[it].src) && mp.count(edge[it].dst))
		{
			Edge EE;
			EE.src = mp[edge[it].src];
			EE.dst = mp[edge[it].dst];
			asrc[i] = EE.src;
			adst[i] = EE.dst;
			vertex.insert(asrc[i]);
			vertex.insert(adst[i]);
			hole.insert(EE);
			++i;
		}
	}
	cnt = i;
	std::cout << "Test LCC:" << std::endl;
	auto time1 = std::chrono::system_clock::now();
	std::unordered_map<VertexType, std::set<VertexType>> inAndOutNeighbors;
	std::unordered_map<VertexType, uint64_t> numOfNeighbors;
	std::unordered_map<VertexType, double> lcc;
	for (int i = 0; i < cnt; i++) {
		VertexType tmpSrc = asrc[i], tmpDst = adst[i];
		if (!inAndOutNeighbors[tmpSrc].count(tmpDst)) {
			numOfNeighbors[tmpSrc]++;
			inAndOutNeighbors[tmpSrc].insert(tmpDst);
		}
		if (!inAndOutNeighbors[tmpDst].count(tmpSrc)) {
			numOfNeighbors[tmpDst]++;
			inAndOutNeighbors[tmpDst].insert(tmpSrc);
		}
	}
	std::vector<VertexType> V;
	for (const auto& v : vertex)
		V.push_back(v);
	std::cout << "Precomputation Over!\n";
#define DEBUG_LCC
	for (uint64_t i = 0; i < V.size(); i++) {
		VertexType v = V[i];
		uint64_t d = numOfNeighbors[v];
		if (d <= 1)
			continue;
		uint64_t t = 0;
		for (const auto& u : inAndOutNeighbors[v])
		{
			auto neighborsOfU = hole.GetNeighbors(u);
			sort(neighborsOfU.begin(), neighborsOfU.end());
			neighborsOfU.erase(unique(neighborsOfU.begin(), neighborsOfU.end()), neighborsOfU.end());
			for (const auto& temp : neighborsOfU)
			{
				VertexType w = temp.first;
				if (w == u)
					continue;
				if (inAndOutNeighbors[w].count(v))
				{
					t++;
				}
			}
		}
		{
			lcc[v] = (double)t / ((double)d * (d - 1));
		}
	}
	auto time2 = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1);
	std::cout << "LCC Time:" << elapsed.count() << "us" << '\n';
	for (const auto& v : vertex)
	{
		if (lcc[v] > 1.0) {
			std::cerr << "Warning! LCC > 1.0!" << std::endl;
		}
	}
	delete[] edge;
}
void wind_Lcc()
{
	Chandelier* wind;
	wind = new Chandelier(400);
	unordered_map<key_type, key_type> node;
	key_type topk[5000001] = { 0 };
	int maxtopk = 10000;
	count_type cnt;
	Edge* edge = read_caida_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();
	for (int it = 0; it < cnt; ++it) {
		Edge EE;
		EE.src = edge[it].src;
		EE.dst = edge[it].dst;
		if (ed.count(EE))continue;
		ed.insert(EE);
		edg.insert(EE.dst);
		edg.insert(EE.src);
		if (node.find(EE.src) != node.end()) node[EE.src] = node[EE.src] + 1;
		else node.insert(pair<key_type, key_type>(EE.src, 1));
		if (node.find(EE.dst) != node.end()) node[EE.dst] = node[EE.dst] + 1;
		else node.insert(pair<key_type, key_type>(EE.dst, 1));
	}
	unordered_map<key_type, key_type> mp;
	std::set<VertexType> vertex;
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
		mp.insert({ topk[i],i + 1 });
	}
	edg.clear(); ed.clear();
	int i = 0;
	for (int it = 0; it < cnt; ++it) {
		if (mp.count(edge[it].src) && mp.count(edge[it].dst))
		{
			Edge EE;
			EE.src = mp[edge[it].src];
			EE.dst = mp[edge[it].dst];
			asrc[i] = EE.src;
			adst[i] = EE.dst;
			vertex.insert(asrc[i]);
			vertex.insert(adst[i]);
			chain* c = new chain();
			c->src = mp[edge[it].src];
			c->dst = mp[edge[it].dst];
			c->pointer_chand = nullptr;
			wind->insert(c);
			++i;
		}
	}
	cnt = i;
	std::cout << "Test LCC:" << std::endl;
	auto time1 = std::chrono::system_clock::now();
	std::unordered_map<VertexType, std::set<VertexType>> inAndOutNeighbors;
	std::unordered_map<VertexType, uint64_t> numOfNeighbors;
	std::unordered_map<VertexType, double> lcc;
	for (int i = 0; i < cnt; i++) {
		VertexType tmpSrc = asrc[i], tmpDst = adst[i];
		if (!inAndOutNeighbors[tmpSrc].count(tmpDst)) {
			numOfNeighbors[tmpSrc]++;
			inAndOutNeighbors[tmpSrc].insert(tmpDst);
		}
		if (!inAndOutNeighbors[tmpDst].count(tmpSrc)) {
			numOfNeighbors[tmpDst]++;
			inAndOutNeighbors[tmpDst].insert(tmpSrc);
		}
	}
	std::vector<VertexType> V;
	for (const auto& v : vertex)
		V.push_back(v);
	std::cout << "Precomputation Over!\n";
#define DEBUG_LCC
	for (uint64_t i = 0; i < V.size(); i++) {
		VertexType v = V[i];
		uint64_t d = numOfNeighbors[v];
		if (d <= 1)
			continue;
		uint64_t t = 0;
		for (const auto& u : inAndOutNeighbors[v])
		{
			auto neighborsOfU = wind->GetNeighbors(u);
			sort(neighborsOfU.begin(), neighborsOfU.end());
			neighborsOfU.erase(unique(neighborsOfU.begin(), neighborsOfU.end()), neighborsOfU.end());
			for (const auto& temp : neighborsOfU)
			{
				VertexType w = temp.first;
				if (w == u)
					continue;
				if (inAndOutNeighbors[w].count(v))
				{
					t++;
				}
			}
		}
		{
			lcc[v] = (double)t / ((double)d * (d - 1));
		}
	}
	auto time2 = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1);
	std::cout << "LCC Time:" << elapsed.count() << "us" << '\n';
	for (const auto& v : vertex)
	{
		if (lcc[v] > 1.0) {
			std::cerr << "Warning! LCC > 1.0!" << std::endl;
		}
	}
	delete[] edge;
}
void ours_Bc()
{
	unordered_map<key_type, key_type> node;
	key_type topk[5000001] = { 0 };
	holemerge hole(bl1, bl2, 0, 221);
	int maxtopk = 1000;
	count_type cnt;
	Edge* edge = read_stackoverflow_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();
	for (int it = 0; it < cnt; ++it) {
		Edge EE;
		EE.src = edge[it].src;
		EE.dst = edge[it].dst;
		if (ed.count(EE))continue;
		ed.insert(EE);
		edg.insert(EE.dst);
		edg.insert(EE.src);
		if (node.find(EE.src) != node.end()) node[EE.src] = node[EE.src] + 1;
		else node.insert(pair<key_type, key_type>(EE.src, 1));
		if (node.find(EE.dst) != node.end()) node[EE.dst] = node[EE.dst] + 1;
		else node.insert(pair<key_type, key_type>(EE.dst, 1));
	}
	unordered_map<key_type, key_type> mp;
	std::set<VertexType> vertex;
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
		mp.insert({ topk[i],i + 1 });
	}
	edg.clear(); ed.clear();
	int i = 0;
	for (int it = 0; it < cnt; ++it) {
		if (mp.count(edge[it].src) && mp.count(edge[it].dst))
		{
			Edge EE;
			EE.src = mp[edge[it].src];
			EE.dst = mp[edge[it].dst];
			asrc[i] = EE.src;
			adst[i] = EE.dst;
			vertex.insert(asrc[i]);
			vertex.insert(adst[i]);
			hole.insert(EE);
			++i;
		}
	}
	cnt = i;
	std::vector<VertexType> V;
	for (const auto& v : vertex)
		V.push_back(v);
	std::cout << "Test BC:" << std::endl;
	auto time1 = std::chrono::system_clock::now();
	const uint64_t INF = 0x3f3f3f3f3f3f3f3f;
	std::unordered_map<VertexType, double > CB;
	for (int i = 0; i < V.size(); i++) {
		int s = V[i];
		using prev_type = std::vector<uint64_t>;
		std::unordered_map<VertexType, uint64_t> sigma;
		std::unordered_map<VertexType, double> delta;
		std::unordered_map<VertexType, uint64_t> dist;
		std::unordered_map<VertexType, prev_type> prev;
		sigma[s] = 1;
		dist[s] = 0;
		std::queue<VertexType> Q;
		std::stack<VertexType> S;
		Q.push(s);
		while (!Q.empty()) {
			VertexType u = Q.front();
			Q.pop();
			S.push(u);
			auto neighbors = hole.GetNeighbors(u);
			for (const auto& [v, useless] : neighbors) {
				if (dist[v] == 0) {
					dist[v] = dist[u] + 1;
					Q.push(v);
				}
				if (dist[v] == dist[u] + 1) {
					sigma[v] += sigma[u];
					prev[v].push_back(u);
				}
			}
		}
		while (!S.empty()) {
			VertexType v = S.top();
			S.pop();
			for (const auto& u : prev[v]) {
				delta[u] = delta[u] + (double)sigma[u] / sigma[v] * (1 + delta[v]);
				if (u != s) {
					{
						CB[v] += delta[v];
					}
				}
			}
		}
	}
	auto time2 = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1);
	std::cout << "BC Time: " << elapsed.count() << "us" << '\n';
	delete[] edge;
}
void wind_Bc()
{
	Chandelier* wind;
	wind = new Chandelier(400);
	unordered_map<key_type, key_type> node;
	key_type topk[5000001] = { 0 };
	int maxtopk = 1000;
	count_type cnt;
	Edge* edge = read_wiki_data(&cnt, 100000000);
	set<key_type> edg;
	edg.clear();
	set<Edge, EdgeLess> ed;
	ed.clear();
	for (int it = 0; it < cnt; ++it) {
		Edge EE;
		EE.src = edge[it].src;
		EE.dst = edge[it].dst;
		if (ed.count(EE))continue;
		ed.insert(EE);
		edg.insert(EE.dst);
		edg.insert(EE.src);
		if (node.find(EE.src) != node.end()) node[EE.src] = node[EE.src] + 1;
		else node.insert(pair<key_type, key_type>(EE.src, 1));
		if (node.find(EE.dst) != node.end()) node[EE.dst] = node[EE.dst] + 1;
		else node.insert(pair<key_type, key_type>(EE.dst, 1));
	}
	unordered_map<key_type, key_type> mp;
	std::set<VertexType> vertex;
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
		mp.insert({ topk[i],i + 1 });
	}
	edg.clear(); ed.clear();
	int i = 0;
	for (int it = 0; it < cnt; ++it) {
		if (mp.count(edge[it].src) && mp.count(edge[it].dst))
		{
			Edge EE;
			EE.src = mp[edge[it].src];
			EE.dst = mp[edge[it].dst];
			asrc[i] = EE.src;
			adst[i] = EE.dst;
			vertex.insert(asrc[i]);
			vertex.insert(adst[i]);
			chain* c = new chain();
			c->src = mp[edge[it].src];
			c->dst = mp[edge[it].dst];
			c->pointer_chand = nullptr;
			wind->insert(c);
			++i;
		}
	}
	cnt = i;
	std::vector<VertexType> V;
	for (const auto& v : vertex)
		V.push_back(v);
	std::cout << "Test BC:" << std::endl;
	auto time1 = std::chrono::system_clock::now();
	const uint64_t INF = 0x3f3f3f3f3f3f3f3f;
	std::unordered_map<VertexType, double > CB;
	for (int i = 0; i < V.size(); i++) {
		int s = V[i];
		using prev_type = std::vector<uint64_t>;
		std::unordered_map<VertexType, uint64_t> sigma;
		std::unordered_map<VertexType, double> delta;
		std::unordered_map<VertexType, uint64_t> dist;
		std::unordered_map<VertexType, prev_type> prev;
		sigma[s] = 1;
		dist[s] = 0;
		std::queue<VertexType> Q;
		std::stack<VertexType> S;
		Q.push(s);
		while (!Q.empty()) {
			VertexType u = Q.front();
			Q.pop();
			S.push(u);
			auto neighbors = wind->GetNeighbors(u);
			for (const auto& [v, useless] : neighbors) {
				if (dist[v] == 0) {
					dist[v] = dist[u] + 1;
					Q.push(v);
				}
				if (dist[v] == dist[u] + 1) {
					sigma[v] += sigma[u];
					prev[v].push_back(u);
				}
			}
		}
		while (!S.empty()) {
			VertexType v = S.top();
			S.pop();
			for (const auto& u : prev[v]) {
				delta[u] = delta[u] + (double)sigma[u] / sigma[v] * (1 + delta[v]);
				if (u != s) {
					{
						CB[v] += delta[v];
					}
				}
			}
		}
	}
	auto time2 = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1);
	std::cout << "BC Time: " << elapsed.count() << "us" << '\n';
	delete[] edge;
	delete wind;
}
int main()
{
	srand(time(NULL));
	test_hole_th();
	test_wind_th();
	exit(0);
}
