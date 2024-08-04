#include "querysupportstruct.h"
using namespace std;
edgeval::edgeval(string _from,string _to,unsigned int _val){
    from=_from;to=_to;val=_val;
}

bool edgeval::operator < (const edgeval& b)const
{
    return val<b.val;
}

void heap::heapSubTree(int k,int n){
    while((2*k+1)<=n){
        int temp=2*k+1;
        if((2*k+2)<=n&&(*num[2*k+2])<(*num[2*k+1])){
            temp=2*k+2;
        }
        if((*num[temp])<(*num[k])){
            edgeval *t=num[temp];
            num[temp]=num[k];
            num[k]=t;
            k=temp;
        }
        else break;
    }
}

void heap::initialHeap(int l){
    s=l;
    num=new edgeval*[s];
    for(int i=0;i<s;i++)num[i]=new edgeval("null","null",0);
    for(int j=(s-2)/2;j>=0;j--){
        heapSubTree(j,s-1);
    }
}

int heap::find(string s1,string s2){
    for(int i=0;i<s;i++){
        if(num[i]->from==s1&&num[i]->to==s2){
            return num[i]->val;
        }
    }
    return -1;
}

heap::~heap(){
    for(int i=0;i<s;i++){
        delete num[i];
    }
}

void heap::pop(){
    delete num[0];
    num[0]=num[s-1];
    s--;
    heapSubTree(0,s-1);
}

void heap::push(string s1,string s2,unsigned int weight){
    num[s]=new edgeval(s1,s2,weight);s++;
    int temp=(s-2)/2,child=s-1;
    while(temp>=0){
        if((*num[child])<(*num[temp])){
            edgeval *t=num[child];
            num[child]=num[temp];
            num[temp]=t;
            child=temp;
            temp=(temp-1)/2;
        }
        else break;
    }
}

bool heap:: increase(string s1,string s2,unsigned int weight){
    for(int i=0;i<s;i++){
        if(num[i]->from==s1&&num[i]->to==s2){
            int now=i;
            num[now]->val=weight;
            heapSubTree(now,s-1);
            return true;
        }
    }
    return false;
}

unsigned int heap:: top(){
    return num[0]->val;
}

void heap::topres(string &s1,string &s2){
    s1=num[0]->from;
    s2=num[0]->to;
}

bool heap::empty(){
    if(!s)return true;
    return false;
}

int heap::size(){
    return s;
}

Blizzard_MPQ::Blizzard_MPQ(){
    ul seed =0x00100001, index1 =0, index2 =0, i;
    for(index1 = 0; index1 < 0x100; ++index1){
        for(index2 = index1, i = 0; i < 5; ++i, index2 += 0x100){
          ul temp1, temp2;
          seed = (seed * 125 + 3) % 0x2AAAAB;
          temp1 = (seed & 0xFFFF) << 0x10;
          seed = (seed * 125 + 3) % 0x2AAAAB;
          temp2 = (seed & 0xFFFF);
          MagicTable[index2] = ( temp1 | temp2 );
        }
    }
}

ul Blizzard_MPQ::MPQHash(const char* str, ul HashType){
      uc *key = (uc*)str;
      ul seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
      int ch;
      while(*key != 0){
        ch = toupper(*key++);
        seed1 = MagicTable[(HashType <<8) + ch] ^ (seed1 + seed2);
        seed2 = ch + seed1 + seed2 + (seed2 << 5) +3;
      }
      return seed1;
}


Gbasket::Gbasket(uint64* _srcFig,uint64*_dstFig,uint64* _idx,weightSize* _weight){
    srcFig=_srcFig;dstFig=_dstFig;
    idx=_idx;
    weight=_weight;
}

Gbasket::~Gbasket(){
    delete [] dstFig;
    delete [] srcFig;
    delete [] idx;
    delete [] weight;
}

basketNode::basketNode(){
    for(int i=0;i<2;i++){
        next[i]=NULL;
    }
    value=NULL;
    bound=0;
    level=0;
}

basketNode::~basketNode(){
    delete value;
}

ul calHash(ul tmp_g,ul tmp_s,int k,int w){
    ul shifter = tmp_g;
    for (int v = 0; v < tmp_s; v++)
        shifter = (shifter*timer + prime) % bigger_p;
    ul tmp_h = k;
    ul left=0,right=(shifter/w)+1;
    while(left<right){
        ul mid=(left+right)>>1;
        if(tmp_h+w*mid<shifter){
            left=mid+1;
        }
        else right=mid;
    }
    tmp_h+=w*left;
    tmp_h -= shifter;
    
    return tmp_h;
}




