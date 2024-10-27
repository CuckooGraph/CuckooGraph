#include "CuckooGraphInterface.h"
#include "Cuckoo.h"
#include <set>

using namespace std;

extern class holemerge;
extern int bl1, bl2;
using SUI = set<unsigned int>;

struct CuckooGraphTypeObject *createCuckooGraphTypeObject(void)
{
    struct CuckooGraphTypeObject *ptr = new CuckooGraphTypeObject;
    holemerge *temp1 = new holemerge(bl1, bl2, 0, 221);
    ptr->Graph = temp1;
    SUI *temp2 = new SUI;
    ptr->Set = temp2;
    return ptr;
}

// void InsertInternal(struct CuckooGraphTypeObject *o, uint32_t ele1, uint32_t ele2)
// {
//     SUI *ptr1 = (SUI*)o->Set;
//     ptr1->insert(ele);
//     holemerge *ptr2 = (holemerge *)o->Graph;
//     ptr2->
// }

void CuckooGraphTypeInsert(struct CuckooGraphTypeObject *o, uint32_t ele1, uint32_t ele2)
{
    SUI *ptr1 = (SUI*)o->Set;
    ptr1->insert(ele1);
    ptr1->insert(ele2);


    holemerge *ptr2 = (holemerge *)o->Graph;
    ptr2->insert({ele1, ele2});
}