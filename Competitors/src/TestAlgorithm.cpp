#include "TestAlgorithm.hpp"
#include "GeneralGraph.hpp"
#include <queue>
#include <vector>
#include <unordered_map>
#include<set>
#include<iostream>
#include <fstream>
#include<time.h>
#include<algorithm>
#include<map>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <set>
#include <chrono>
#include<assert.h>
#include <algorithm>

#include <omp.h>
using namespace std;


bool Compare(const Node_degree& a, const Node_degree& b) {
    if (a.val == b.val)return a.node < b.node;
    return a.val < b.val;
}

int query_triangles(GeneralGraph *g, VertexType src) {
        int cnt = 0;
        auto temp = g->GetNeighbors(src);
        mEdge e;
        e.dst = src;
       for (const auto &edge : temp) {
            auto temp2 = g->GetNeighbors(edge.first);
       for (const auto &edge2 : temp2) {
                e.src = edge2.first;
                if ((edge2.first != edge.first) && (edge.first != src) && (edge2.first != src)) {
                   if( g->GetWeightOfEdge(e.src,e.dst))++cnt;
                }
            }
        }
        return cnt;
}

void test_triangles(GeneralGraph* g,std::string path) {
int mul=3;
    int sample_num=10000;
    int ran[33333]={0};
    cout<<"test triangles begin!"<<endl;
    g->LoadData(path);
    int cnt = g->cnt;
    set<key_type> edg;
    edg.clear();
    set<mEdge, mEdgeLess> ed;
    ed.clear();

    ofstream fout;
    fout.open("test_tiangles.txt", ios::out);

    int inum = 0;
    int sum1 = 0;
    int sum2 = 0;
    
    ifstream infile;
    infile.open("./rand.txt");
    int intbig = 2147483647;
    for(int i=0;i<sample_num;++i){
        char s[50];
        if (!infile.getline(s, intbig))assert(0);
        string nu(s);
        ran[i]=stoi(nu);
    }
    
     for (int it = 0; it < cnt; ++it) {
        //g->InsertVertex(g->asrc[it]);     //sortledton and livegraph need, spruce and terrace do not need
        //g->InsertVertex(g->adst[it]);     //sortledton and livegraph need, spruce and terrace do not need
        WeightType we = g->GetWeightOfEdge(g->asrc[it], g->adst[it]);
        g->ModifyEdge(g->asrc[it], g->adst[it], we + 1);

        mEdge EE;
        EE.src = g->asrc[it];
        EE.dst = g->adst[it];
        edg.insert(EE.dst);
        edg.insert(EE.src);
    }
    
    key_type* edge = new key_type[edg.size()];
    int number=0;
    for(auto it =edg.begin();it!=edg.end();++it) {
       edge[number]=*it;
       ++number;
       }
    
    double ours_average = 0;
    for (int mm = 0; mm < mul; ++mm) {
                clock_t time_start_2 = clock();
                for (int i = 0; i < sample_num; ++i) {
                    sum2 += query_triangles(g,edge[ran[i]%edg.size()]);
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

long Bfs(GeneralGraph *g, VertexType src)
{
    clock_t time_start = clock();
    uint32_t node_num=0;
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
        for (const auto &edge : temp) {
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
    fr1.open("fr1.txt", ios::app);
    for (auto it = bfs_node.begin(); it != bfs_node.end(); ++it)
       fr1<<*it<<endl;
    fr1<<"node_num:   "<<node_num<<endl;
    fr1.close();
    long time = (time_end - time_start);
    return time;
}

void test_bfs(GeneralGraph* g,std::string path){
cout<<"test BFS begin!"<<endl;
        g->LoadData(path);
        int cnt = g->cnt;
        int maxtopk = 1003;
    int R=50;
    key_type topk[10001] = { 0 };    
    set<key_type> edg;
    set<mEdge, mEdgeLess> ed;
    unordered_map<key_type, key_type> node;
    
  //  for (int i = 0; i < g->cnt; ++i) g->InsertEdge(g->asrc[i], g->adst[i], 0); //terrace do not need, others do not need
    
    for (int it = 0; it < cnt; ++it) {
       // g->InsertVertex(g->asrc[it]);     //sortledton and livegraph need, spruce and terrace do not need
       // g->InsertVertex(g->adst[it]);     //sortledton and livegraph need, spruce and terrace do not need
        WeightType we = g->GetWeightOfEdge(g->asrc[it], g->adst[it]);
        g->ModifyEdge(g->asrc[it], g->adst[it], we + 1);

        mEdge EE;
        EE.src = g->asrc[it];
        EE.dst = g->adst[it];
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
    
    double average_time =0;
    for(int i=0;i<R;++i){
    long time = Bfs(g,topk[i]);
    double ttt = (double)time;
    ttt=ttt/R;
    average_time=average_time+ttt;
    }   
    
    cout<<"average_time:    "<<average_time<<endl;
}

void test_sssp_new(GeneralGraph* g,std::string path) {
    cout << "okk!" << endl;
    g->LoadData(path);
    cout << "ok2" << endl;
    assert(g->asrc.size() == g->adst.size());
    int cnt = g->cnt;
    int mul = 3;
    int maxtopk = 4000;
    int rd = 10;
    key_type topk[10001] = { 0 };
    set<key_type> edg;
    set<mEdge, mEdgeLess> ed;
    unordered_map<key_type, key_type> node;

    ofstream fout;
    ofstream fr1;
    ofstream fr2;
    ofstream fr3;
    fout.open("test_sssp.txt", ios::out);
    fr1.open("fr1.txt", ios::out);
    fr2.open("fr2.txt", ios::out);
    fr3.open("fr3.txt", ios::out);



    for (int it = 0; it < cnt; ++it) {
       // g->InsertVertex(g->asrc[it]);     //sortledton and livegraph need, spruce and terrace do not need
       // g->InsertVertex(g->adst[it]);     //sortledton and livegraph need, spruce and terrace do not need
        WeightType we = g->GetWeightOfEdge(g->asrc[it], g->adst[it]);
        g->ModifyEdge(g->asrc[it], g->adst[it], we + 1);

        mEdge EE;
        EE.src = g->asrc[it];
        EE.dst = g->adst[it];
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
    cout << "INF:  " <<INF << endl;


 for (uint32_t TT = 1; TT <= 1; ++TT) {
        uint32_t num = 3000 * TT;
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
                    uint32_t we = g->GetWeightOfEdge(vs, topk[it]);
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
                        uint32_t we = g->GetWeightOfEdge(vs, topk[it]);
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
}

void PageRank(GeneralGraph* g,std::string path)
{	
g->LoadData(path);count_type cnt=g->cnt;
	unordered_map<key_type, key_type> node;
	int mul = 3;
	int maxtopk = 1000;
	int rd = 10;
	key_type topk[10001] = { 0 };
	double d = 0.85;
	double e = 0.1;
	int maxIterations = 100;
	set<key_type> edg;
	edg.clear();
	set<mEdge, mEdgeLess> ed;
	ed.clear();


	for (int it = 0; it < cnt; ++it) {
		

		mEdge EE;
		EE.src = g->asrc[it];
		EE.dst = g->adst[it];
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
		// if(mp.count(g->asrc[it])) g->InsertVertex(mp[g->asrc[it]]);//sortledton and livegraph need, spruce and terrace do not need   real
     //   if(mp.count(g->adst[it])) g->InsertVertex(mp[g->adst[it]]);//sortledton and livegraph need, spruce and terrace do not need   real
			if (mp.count(g->asrc[it]) && mp.count(g->adst[it]))
			{
        WeightType we = g->GetWeightOfEdge(mp[g->asrc[it]],mp[ g->adst[it]]);
        g->ModifyEdge(mp[g->asrc[it]],mp[ g->adst[it]], we + 1);
			}
	}

	int N = maxtopk;

	clock_t time_start_2 = clock();
	vector<vector<double>> M(N, vector<double>(N, 0.0));

	for (int i = 0; i < N; i++)
	{
		key_type src = i + 1;
		auto temp = g->GetNeighbors(src);
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
	fr1.open("PR_fr.txt", ios::out);
	for (int i = 0; i < N; i++)
	{
		fr1 << pagerank[i] << endl;
	}
	fr1.close();
}


void Lcc(GeneralGraph *g)
{
    try {
    std::cout << "Test LCC:" << std::endl;
    auto time1 = std::chrono::system_clock::now();
    std::unordered_map<VertexType, std::set<VertexType>> inAndOutNeighbors;
    std::unordered_map<VertexType, uint64_t> numOfNeighbors;
    std::unordered_map<VertexType, double> lcc;


    for (int i = 0; i < g->cnt; i++) {
        VertexType tmpSrc = g->asrc[i], tmpDst = g->adst[i];
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
    for (const auto &v: g->vertex)
        V.push_back(v);
    std::cout << "Precomputation Over!\n";

    omp_set_num_threads(32);
    #define DEBUG_LCC

    
    for (uint64_t i = 0; i < V.size(); i++) {

        VertexType v = V[i];
        uint64_t d = numOfNeighbors[v];

        if (d <= 1)
            continue;

        uint64_t t = 0;
        for (const auto& u: inAndOutNeighbors[v])
        {
            auto neighborsOfU = g->GetNeighbors(u);
            sort(neighborsOfU.begin(), neighborsOfU.end());
            neighborsOfU.erase(unique(neighborsOfU.begin(), neighborsOfU.end()), neighborsOfU.end());
            for (const auto& temp: neighborsOfU )
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
    std::cout << "LCC Time:" << elapsed.count() <<"us" << '\n';

    for (const auto& v: g->vertex)
    {
        if (lcc[v] > 1.0) {
            std::cerr << "Warning! LCC > 1.0!" << std::endl;
        }
    }

    } catch (IllegalOperation str) {
        std::cout << str.what << std::endl;
    }
}


















// https://en.wikipedia.org/wiki/Brandes%27_algorithm#cite_note-Brandes2001-1
void Bc(GeneralGraph *g)
{
    std::vector<VertexType> V;
    for (const auto &v: g->vertex)
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
            auto neighbors = g->GetNeighbors(u);
            for (const auto& [v, useless]: neighbors) {
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

            for (const auto& u: prev[v]) {
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
}