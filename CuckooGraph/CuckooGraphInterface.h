#pragma once
#ifndef CUCKOOGRAPHINTERFACE
#define CUCKOOGRAPHINTERFACE

#include <stdint.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

struct CuckooGraphTypeObject
{
    void *Graph;
    void *VertexCount;
};

struct CuckooGraphTypeObject *createCuckooGraphTypeObject(void);
// void InsertInternal(struct CuckooGraphTypeObject *o, int ele1, int ele2);
void CuckooGraphTypeInsert(struct CuckooGraphTypeObject *o, int ele1, int ele2);
void CuckooGraphTypeReleaseObjectInternal(struct CuckooGraphTypeObject *o);
void CuckooGraphTypeReleaseObject(struct CuckooGraphTypeObject *o);
int QueryInternal(struct CuckooGraphTypeObject *o, int src, int dst);
void DelInternal(struct CuckooGraphTypeObject *o, int src, int dst);
void GetNeighborsInternal(struct CuckooGraphTypeObject *o, int src, int **p_addr, int *p_len);
void GetAllVertices(struct CuckooGraphTypeObject *o, int **addr, int *len);
// void CreateUnorderedMap(struct CuckooGraphTypeObject *o);
void UpdateUnorderedMap(struct CuckooGraphTypeObject *o, int v); 

#ifdef __cplusplus
}
#endif // __cplusplus
#endif //CUCKOOGRAPHINTERFACE