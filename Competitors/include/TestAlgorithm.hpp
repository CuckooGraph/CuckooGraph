#pragma once
#ifndef TESTALGORITHM
#define TESTALGORITHM

#include <queue>
#include <vector>
#include <unordered_map>
#include <set>
#include <iostream>
#include <fstream>
#include <time.h>
#include <algorithm>
#include <map>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <set>
#include <chrono>
#include <assert.h>

#include "GeneralGraph.hpp"

class mEdge
{
public:
	key_type src;
	key_type dst;
};

class mEdgeLess
{
public:
	bool operator()(const mEdge &e1, const mEdge &e2) const
	{
		if (e1.src == e2.src)
			return e1.dst < e2.dst;
		return e1.src < e2.src;
	}
};

typedef struct Node_degree
{
	key_type node;
	key_type val;
} Node_degree;

bool Compare(const Node_degree &a, const Node_degree &b);
/*bool Compare(const Node_degree& a, const Node_degree& b) {
	if (a.val == b.val)return a.node < b.node;
	return a.val < b.val;
}*/

long Bfs(GeneralGraph *g, VertexType src);
void test_bfs(GeneralGraph *g, std::string path);
void test_sssp_new(GeneralGraph *g, std::string path);
int query_triangles(GeneralGraph *g, VertexType src);
void test_triangles(GeneralGraph *g, std::string path);
void PageRank(GeneralGraph *g, std::string path);
void Lcc(GeneralGraph *g);
void Bc(GeneralGraph *g);

class CuckooGraph_tarjan
{
public:
	GeneralGraph *g;
	int index;
	vector<int> low;
	vector<bool> instack;
	int scc_count;
	stack<key_type> stk;
	vector<vector<key_type>> scc_list;

	void tarjan(key_type v)
	{
		int i;
		int dfn = index;
		low[v] = index++;
		stk.push(v);
		instack[v] = true;
		auto temp = g->GetNeighbors(v);
		for (const auto &EEE : temp)
		{
			key_type u = EEE.first;
			if (low[u] == -1)
			{
				tarjan(u);
				if (low[u] < low[v])
				{
					low[v] = low[u];
				}
			}
			else if (instack[u])
			{
				if (low[u] < low[v])
				{
					low[v] = low[u];
				}
			}
		}
		if (low[v] == dfn)
		{
			key_type j;
			do
			{
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

	void tarjan_algorithm(int node_num)
	{
		index = 0;
		scc_count = 0;
		for (int i = 1; i <= node_num; i++)
		{
			if (low[i] == -1)
			{
				tarjan(i);
			}
		}
	}

	CuckooGraph_tarjan(GeneralGraph *_g, std::string path) : index(0), scc_count(0)
	{
		g = _g;
		g->LoadData(path);
		low.clear();
		instack.clear();
		scc_list.clear();
		while (!stk.empty())
			stk.pop();
	}

	void test_CuckooGraph_tarjan()
	{
		count_type cnt = g->cnt;
		int mul = 3;
		int maxtopk = 4000;
		int rd = 10;
		key_type topk[10001] = {0};
		set<key_type> edg;
		set<mEdge, mEdgeLess> ed;
		unordered_map<key_type, key_type> node;

		ofstream fout;
		fout.open("test_CC.txt", ios::out);

		for (int it = 0; it < cnt; ++it)
		{

			mEdge EE;
			EE.src = g->asrc[it];
			EE.dst = g->adst[it];
			if (ed.count(EE))
				continue;
			ed.insert(EE);
			edg.insert(EE.dst);
			edg.insert(EE.src);
			if (node.find(EE.src) != node.end())
				node[EE.src] = node[EE.src] + 1;
			else
				node.insert(pair<key_type, key_type>(EE.src, 1));
			if (node.find(EE.dst) != node.end())
				node[EE.dst] = node[EE.dst] + 1;
			else
				node.insert(pair<key_type, key_type>(EE.dst, 1));
		}
		unordered_map<key_type, key_type> mp;

		vector<Node_degree> nd;
		for (auto it = node.begin(); it != node.end(); ++it)
		{
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

		for (int i = 0; i < maxtopk; ++i)
		{
			topk[i] = nd[0].node;
			pop_heap(nd.begin(), nd.end(), Compare);
			nd.pop_back();
			mp.insert({topk[i], i + 1});
			low.push_back(-1);
			instack.push_back(false);
		}

		for (int it = 0; it < cnt; ++it)
		{
			if (mp.count(g->asrc[it]) && mp.count(g->adst[it]))
			{

				WeightType we = g->GetWeightOfEdge(mp[g->asrc[it]], mp[g->adst[it]]);
				g->ModifyEdge(mp[g->asrc[it]], mp[g->adst[it]], we + 1);
			}
		}
		clock_t time_start_2 = clock();
		tarjan_algorithm(maxtopk);
		clock_t time_end_2 = clock();
		long time = time_end_2 - time_start_2;
		cout << "time:  " << time << endl;
		cout << "scc_count: " << scc_count << endl;
		for (int i = 0; i < scc_count; i++)
		{
			fout << "SCC " << i << ": ";
			for (int j = 0; j < scc_list[i].size(); j++)
			{
				fout << scc_list[i][j] << " ";
			}
			fout << endl;
		}
		fout.close();
	}
};
#endif +