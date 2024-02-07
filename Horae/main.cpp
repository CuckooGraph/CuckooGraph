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
#include "headers/QueryFunction.h"
#include "Util.h"


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
#define minc1 8
#define minc2 4
#define cthres 0.35
#define bminc1 8
#define bminc2 4
#define bcthres 0.35
#define sbl 128
#define bbl 128


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

const string folder = "./";
const string filenames[] = { "130000.dat" };





int load_data13_CAIDA(int length = 100000000)
{
	ifstream input("./130000.dat", ios::in | ios::binary);
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

Edge* read_CuckooGraph_data(count_type* cnt, const count_type length = 100000000) {
	Edge* items = new Edge[length];
	Edge* it = items;

	int intbig = 2147483647;

	ifstream infile;
	infile.open("./CuckooGraphDatasetNewNew.txt");
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


void test_Horae_time() {
	cout << "Horae:" << endl;
	uint32_t granularityLength = 61200, gl = 1, slot = 2, fingerprintLen = 7;
	uint32_t row_addrs = 4, col_addrs = 4;
	uint32_t width = 6500;
	uint32_t depth = 6500;
	bool kick = false, cache_align = false;

	count_type cnt;

	Edge* edge = read_NotreDame_data(&cnt, 100000000);

	Horae hole(0, granularityLength, gl, width, depth, fingerprintLen, cache_align, kick, row_addrs, col_addrs);

	int sum = 0;
	cout << "start insert" << endl;
	clock_t t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		hole.insert(edge[i].src, edge[i].dst, 1, 0);
	}
	clock_t t2 = clock();
	long t3 = t2 - t1;
	cout << "Insert average time: " << (double)t3 / (double)cnt << endl;
	cout << "start query" << endl;
	t1 = clock();
	for (int i = 0; i < cnt; ++i) {
		sum += hole.edgeQuery(edge[i].src, edge[i].dst, 0, 0);
	}
	t2 = clock();
	t3 = t2 - t1;
	cout << "Query average time: " << (double)t3 / (double)cnt << endl;
	cout << "sum: " << sum << endl;
	delete[] edge;
}

void caida_test_time_multi() {
	uint32_t granularityLength = 61200, gl = 1, slot = 2, fingerprintLen = 7;
	uint32_t row_addrs = 4, col_addrs = 4;
	uint32_t width = 6500;
	uint32_t depth = 6500;
	bool kick = false, cache_align = false;
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
	fout.open("caida_test_time_multi.txt", ios::out);

	for (int num = 3000; num <= fin; num += 6000) {
		cnt = num * 1000;
		fout << "DATA SIZE: " << cnt << endl;
		for (int i = 0; i < mul; ++i) {
			Horae hole(0, granularityLength, gl, width, depth, fingerprintLen, cache_align, kick, row_addrs, col_addrs);
			int sum = 0;
			clock_t time_start = clock();
			for (int i = 0; i < cnt; ++i) {
				hole.insert(asrc[i], adst[i], 1, 0);
			}
			clock_t time_end = clock();
			long time = time_end - time_start;
			insert[i] = (double)time / (double)cnt;
			cout << "sum: " << sum << endl;
			sum = 0;
			clock_t time_start_2 = clock();
			for (int i = 0; i < cnt; ++i) {
				sum += hole.edgeQuery(asrc[i], adst[i], 0, 0);
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
}

void notredame_test_time_multi() {
	uint32_t granularityLength = 61200, gl = 1, slot = 2, fingerprintLen = 7;
	uint32_t row_addrs = 4, col_addrs = 4;
	uint32_t width = 6500;
	uint32_t depth = 6500;
	bool kick = false, cache_align = false;
	Edge edge;
	count_type cnt;
	int mul = 3;
	double insert[10];
	double query[10];
	for (int i = 0; i < 10; ++i) {
		insert[i] = 0;
		query[i] = 0;
	}
	Edge* EE = read_NotreDame_data(&cnt, 100000000);
	cout << "cnt: " << cnt << endl;


	int fin = 1400;
	set<key_type> ed;
	for (count_type i = 0; i < fin * 1000; ++i)
		ed.insert(asrc[i]);
	int edsize = ed.size();

	ofstream fout;
	fout.open("notredame_test_time_multi.txt", ios::out);

	for (int num = 600; num <= fin; num += 200) {
		cnt = num * 1000;
		fout << "DATA SIZE: " << cnt << endl;
		for (int i = 0; i < mul; ++i) {
			Horae hole(0, granularityLength, gl, width, depth, fingerprintLen, cache_align, kick, row_addrs, col_addrs);
			int sum = 0;
			clock_t time_start = clock();
			for (int i = 0; i < cnt; ++i) {
				hole.insert(asrc[i], adst[i], 1, 0);
			}
			clock_t time_end = clock();
			long time = time_end - time_start;
			insert[i] = (double)time / (double)cnt;
			cout << "sum: " << sum << endl;
			sum = 0;
			clock_t time_start_2 = clock();
			for (int i = 0; i < cnt; ++i) {
				sum += hole.edgeQuery(asrc[i], adst[i], 0, 0);
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

void stackoverflow_test_time_multi() {
	uint32_t granularityLength = 61200, gl = 1, slot = 2, fingerprintLen = 7;
	uint32_t row_addrs = 4, col_addrs = 4;
	uint32_t width = 6500;
	uint32_t depth = 6500;
	bool kick = false, cache_align = false;
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
	fout.open("stackoverflow_test_time_multi.txt", ios::out);

	for (int num = 35000; num <= fin; num += 7000) {
		cnt = num * 1000;
		fout << "DATA SIZE: " << cnt << endl;
		for (int i = 0; i < mul; ++i) {
			Horae hole(0, granularityLength, gl, width, depth, fingerprintLen, cache_align, kick, row_addrs, col_addrs);
			int sum = 0;
			clock_t time_start = clock();
			for (int i = 0; i < cnt; ++i) {
				hole.insert(asrc[i], adst[i], 1, 0);
			}
			clock_t time_end = clock();
			long time = time_end - time_start;
			insert[i] = (double)time / (double)cnt;
			cout << "sum: " << sum << endl;
			sum = 0;
			clock_t time_start_2 = clock();
			for (int i = 0; i < cnt; ++i) {
				sum += hole.edgeQuery(asrc[i], adst[i], 0, 0);
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

void wiki_test_time_multi() {
	uint32_t granularityLength = 61200, gl = 1, slot = 2, fingerprintLen = 7;
	uint32_t row_addrs = 4, col_addrs = 4;
	uint32_t width = 6500;
	uint32_t depth = 6500;
	bool kick = false, cache_align = false;
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
	fout.open("wiki_test_time_multi.txt", ios::out);

	for (int num = 12000; num <= fin; num += 3000) {
		cnt = num * 1000;
		fout << "DATA SIZE: " << cnt << endl;
		for (int i = 0; i < mul; ++i) {
			Horae hole(0, granularityLength, gl, width, depth, fingerprintLen, cache_align, kick, row_addrs, col_addrs);
			int sum = 0;
			clock_t time_start = clock();
			for (int i = 0; i < cnt; ++i) {
				hole.insert(asrc[i], adst[i], 1, 0);
			}
			clock_t time_end = clock();
			long time = time_end - time_start;
			insert[i] = (double)time / (double)cnt;
			cout << "sum: " << sum << endl;
			sum = 0;
			clock_t time_start_2 = clock();
			for (int i = 0; i < cnt; ++i) {
				sum += hole.edgeQuery(asrc[i], adst[i], 0, 0);
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
int main()
{
	srand(time(NULL));
	//caida_test_time_multi();
	//notredame_test_time_multi();
	//wiki_test_time_multi();
	//stackoverflow_test_time_multi();
	exit(0);
}


