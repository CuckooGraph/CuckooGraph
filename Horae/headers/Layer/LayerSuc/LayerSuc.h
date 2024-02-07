#ifndef _LayerSuc_H
#define _LayerSuc_H
#include "../Layer.h"

class LayerSuc : public Layer {
protected:
	vector<vector<node>> successorAdjacencyList;
public:
	LayerSuc(uint32_t granularity, uint32_t width, uint32_t depth, uint32_t fingerprintLength, uint32_t row_addrs = 4, uint32_t column_addrs = 4);
	LayerSuc(const LayerSuc* layer);
	LayerSuc(const LayerSuc* layer, int level);

	virtual ~LayerSuc();
	void bucketCounting();


protected:
	void insertSucBuffer(uint32_t k1, uint32_t k2, weight_type weight);
	weight_type edgeQuerySucBuffer(uint32_t k1, uint32_t k2);
	weight_type nodeQuerySucBuffer(uint32_t k, int type);

};
void LayerSuc::bucketCounting() {
	Layer::bucketCounting();
	cout << "---------------------------------------" << endl;
	cout << "LayerSuc bucketCounting(): print successorAdjacencyList..." << endl;
	cout << "successorAdjacencyList.size() = " << successorAdjacencyList.size() << endl;
	cout << "successorAdjacencyList.capacity() = " << successorAdjacencyList.capacity() << endl;

	int64_t total_sucBuffer = 0;
	int64_t total_cap = 0, total_size = 0;
	for (int64_t i = 0; i < this->successorAdjacencyList.size(); i++) {
		total_cap += this->successorAdjacencyList[i].capacity();
		total_size += this->successorAdjacencyList[i].size();
	}
	cout << "total_size = " << total_size << endl;
	cout << "total_cap = " << total_cap << endl;
	for (int64_t i = 0; i < this->successorAdjacencyList.size(); i++) {
		total_sucBuffer += this->successorAdjacencyList[i].size();
	}
	cout << "total_sucBuffer = " << total_sucBuffer << endl;
	cout << "---------------------------------------" << endl;
	return;
}

LayerSuc::LayerSuc(uint32_t granularity, uint32_t width, uint32_t depth, uint32_t fingerprintLength, uint32_t row_addrs, uint32_t column_addrs) : Layer(granularity, width, depth, fingerprintLength, row_addrs, column_addrs) {
	cout << "LayerSuc::LayerSuc(granularity: " << granularity
		<< ", width: " << width << ", depth: " << depth << ", fplen: " << fingerprintLength
		<< ", row_addrs: " << row_addrs << ", column_addrs: " << column_addrs << ")" << endl;
}
LayerSuc::LayerSuc(const LayerSuc* layer) : Layer(layer) {
	cout << "LayerSuc::LayerSuc(*layer)" << endl;

	this->successorAdjacencyList.resize(layer->successorAdjacencyList.size());
	for (uint32_t i = 0; i < layer->successorAdjacencyList.size(); i++) {
		this->successorAdjacencyList[i].assign(layer->successorAdjacencyList[i].begin(), layer->successorAdjacencyList[i].end());
	}
}

LayerSuc::LayerSuc(const LayerSuc* layer, int level) : Layer(layer, level) {
	cout << "LayerSuc::LayerSuc(*layer, level: " << level << ")" << endl;
}

LayerSuc::~LayerSuc() {
	cout << "LayerSuc::~LayerSuc()" << endl;
	vector<vector<node>>().swap(successorAdjacencyList);
}

void LayerSuc::insertSucBuffer(uint32_t k1, uint32_t k2, weight_type weight) {
	vector<vector<node> >::iterator it = find_if(successorAdjacencyList.begin(), successorAdjacencyList.end(), findv(k1));
	if (it != successorAdjacencyList.end()) {
		vector<node>::iterator iter = find_if(it->begin(), it->end(), findx(k2));
		if (iter != it->end()) {
			iter->weight += weight;
		}
		else {
			node tmpnode;
			tmpnode.key = k2;
			tmpnode.weight = weight;
			it->push_back(tmpnode);
		}
	}
	else {
		node tmpnode;
		tmpnode.key = k1;
		tmpnode.weight = 0;
		vector<node> vc;
		vc.push_back(tmpnode);
		if (k1 != k2) {
			node newnode;
			newnode.key = k2;
			newnode.weight = weight;
			vc.push_back(newnode);
		}
		else {
			vc[0].weight += weight;
		}
		successorAdjacencyList.push_back(vc);
	}
}
weight_type LayerSuc::edgeQuerySucBuffer(uint32_t k1, uint32_t k2) {
	vector<vector<node> >::iterator it = find_if(successorAdjacencyList.begin(), successorAdjacencyList.end(), findv(k1));
	if (it != successorAdjacencyList.end()) {
		vector<node>::iterator iter = find_if(it->begin(), it->end(), findx(k2));
		if (iter != it->end()) {
			return iter->weight;
		}
	}
	return 0;
}
weight_type LayerSuc::nodeQuerySucBuffer(uint32_t k, int type) {
	weight_type weight = 0;
	if (type == 0) {
		vector<vector<node> >::iterator it = find_if(successorAdjacencyList.begin(), successorAdjacencyList.end(), findv(k));
		if (it != successorAdjacencyList.end()) {
			vector<node>::iterator iter;
			for (iter = it->begin(); iter != it->end(); iter++) {
				weight += iter->weight;
			}
		}
	}
	else if (type == 1) {
		for (vector<vector<node> >::iterator it = successorAdjacencyList.begin(); it != successorAdjacencyList.end(); it++) {
			for (vector<node>::iterator iter = it->begin(); iter != it->end(); iter++) {
				if (iter->key == k) {
					weight += iter->weight;
				}
			}
		}
	}
	return weight;
}

#endif	  	// _LayerSuc_H