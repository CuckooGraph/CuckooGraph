#ifndef _LayerSucPre_H
#define _LayerSucPre_H
#include "../LayerSuc/LayerSuc.h"

class LayerSucPre : public LayerSuc {
protected:
	vector<node> precursorAdjacencyList;
public:
	LayerSucPre(uint32_t granularity, uint32_t width, uint32_t depth, uint32_t fingerprintLength, uint32_t row_addrs = 4, uint32_t column_addrs = 4);
	LayerSucPre(const LayerSucPre* layer);
	LayerSucPre(const LayerSucPre* layer, int level);

	virtual ~LayerSucPre();
	void bucketCounting();


protected:
	void insertPreBuffer(uint32_t k1, uint32_t k2, weight_type weight);

};
void LayerSucPre::bucketCounting() {
	LayerSuc::bucketCounting();
	cout << "---------------------------------------" << endl;
	cout << "LayerSucPre bucketCounting(): print precursorAdjacencyList..." << endl;
	int64_t total_preBuffer = this->precursorAdjacencyList.size();
	cout << "precursorAdjacencyList.size() = " << total_preBuffer << endl;
	cout << "---------------------------------------" << endl;
	return;
}

LayerSucPre::LayerSucPre(uint32_t granularity, uint32_t width, uint32_t depth, uint32_t fingerprintLength, uint32_t row_addrs, uint32_t column_addrs) : LayerSuc(granularity, width, depth, fingerprintLength, row_addrs, column_addrs) {
	cout << "LayerSucPre::LayerSucPre(granularity: " << granularity
		<< ", width: " << width << ", depth: " << depth << ", fplen: " << fingerprintLength
		<< ", row_addrs: " << row_addrs << ", column_addrs: " << column_addrs << ")" << endl;
}
LayerSucPre::LayerSucPre(const LayerSucPre* layer) : LayerSuc(layer) {
	cout << "LayerSucPre::LayerSucPre(*layer)" << endl;

	this->precursorAdjacencyList.resize(layer->precursorAdjacencyList.size());
	this->precursorAdjacencyList.assign(layer->precursorAdjacencyList.begin(), layer->precursorAdjacencyList.end());
}

LayerSucPre::LayerSucPre(const LayerSucPre* layer, int level) : LayerSuc(layer, level) {
	cout << "LayerSucPre::LayerSucPre(*layer, level: " << level << ")" << endl;
}

LayerSucPre::~LayerSucPre() {
	cout << "LayerSucPre::~LayerSucPre()" << endl;
	vector<node>().swap(precursorAdjacencyList);
}

void LayerSucPre::insertPreBuffer(uint32_t k1, uint32_t k2, weight_type weight) {
	vector<node>::iterator it2 = find_if(precursorAdjacencyList.begin(), precursorAdjacencyList.end(), findx(k2));
	if (it2 != precursorAdjacencyList.end()) {
		it2->weight += weight;
	}
	else {
		node newnode;
		newnode.key = k2;
		newnode.weight = weight;
		precursorAdjacencyList.push_back(newnode);
	}
}


#endif	 	 // _LayerSucPre_H