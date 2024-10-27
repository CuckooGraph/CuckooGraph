/* This file implements a new module native data type called "CuckooGraphType".
 * The data structure implemented is a very simple ordered linked list of
 * 64 bit integers, in order to have something that is real world enough, but
 * at the same time, extremely simple to understand, to show how the API
 * works, how a new data type is created, and how to write basic methods
 * for RDB loading, saving and AOF rewriting.
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright (c) 2016-Present, Redis Ltd.
 * All rights reserved.
 *
 * Licensed under your choice of the Redis Source Available License 2.0
 * (RSALv2) or the Server Side Public License v1 (SSPLv1).
 */

#include "redismodule.h"
#include "CuckooGraphInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

static RedisModuleType* CuckooGraphType;

#ifdef __cplusplus
extern "C" {
#endif

    /* ========================== Internal data structure  =======================
     * This is just a linked list of 64 bit integers where elements are inserted
     * in-place, so it's ordered. There is no pop/push operation but just insert
     * because it is enough to show the implementation of new data types without
     * making things complex. */

     // struct CuckooGraphTypeNode
     // {
     //     int64_t value;
     //     struct CuckooGraphTypeNode *next;
     // };


     // struct CuckooGraphTypeObject *createCuckooGraphTypeObject(void)
     // {
     //     struct CuckooGraphTypeObject *o;
     //     o = RedisModule_Alloc(sizeof(*o));
     //     o->head = NULL;
     //     o->len = 0;
     //     return o;
     // }

     // void CuckooGraphTypeInsert(struct CuckooGraphTypeObject *o, uint32_t ele1, uint32_t ele2)
     // {
     //     InsertInternal(o, ele);
     // }

     // void CuckooGraphTypeReleaseObject(struct CuckooGraphTypeObject *o)
     // {
     //     struct CuckooGraphTypeNode *cur, *next;
     //     cur = o->head;
     //     while (cur)
     //     {
     //         next = cur->next;
     //         RedisModule_Free(cur);
     //         cur = next;
     //     }
     //     RedisModule_Free(o);
     // }

     /* ========================= "CuckooGraphType" type commands ======================= */

     /* CuckooGraphType.INSERT key src dst */
    int CuckooGraphTypeInsert_RedisCommand(RedisModuleCtx* ctx, RedisModuleString** argv, int argc)
    {
        RedisModule_AutoMemory(ctx); /* Use automatic memory management. */

        if (argc != 4)
            return RedisModule_WrongArity(ctx);
        RedisModuleKey* key = RedisModule_OpenKey(ctx, argv[1],
            REDISMODULE_READ | REDISMODULE_WRITE);
        int type = RedisModule_KeyType(key);
        if (type != REDISMODULE_KEYTYPE_EMPTY &&
            RedisModule_ModuleTypeGetType(key) != CuckooGraphType)
        {
            return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
        }

        long long src, dst;
        if ((RedisModule_StringToLongLong(argv[2], &src) != REDISMODULE_OK))
        {
            return RedisModule_ReplyWithError(ctx, "ERR invalid src: must be a signed 64 bit integer");
        }

        if ((RedisModule_StringToLongLong(argv[3], &dst) != REDISMODULE_OK))
        {
            return RedisModule_ReplyWithError(ctx, "ERR invalid dst: must be a signed 64 bit integer");
        }

        /* Create an empty value object if the key is currently empty. */
        struct CuckooGraphTypeObject* hto;
        if (type == REDISMODULE_KEYTYPE_EMPTY)
        {
            hto = createCuckooGraphTypeObject();
            RedisModule_ModuleTypeSetValue(key, CuckooGraphType, hto);
        }
        else
        {
            hto = (struct CuckooGraphTypeObject*)RedisModule_ModuleTypeGetValue(key);
        }

        /* Insert the new element. */
        CuckooGraphTypeInsert(hto, src, dst);
        // RedisModule_SignalKeyAsReady(ctx, argv[1]);
        // exit(114514);
        // RedisModule_ReplyWithLongLong(ctx, 2021);
        RedisModule_ReplyWithString(ctx, RedisModule_CreateString(ctx, "Insertion Over", 14));
        // RedisModule_ReplicateVerbatim(ctx);
        return REDISMODULE_OK;
    }

    // /* CuckooGraphType.RANGE key first count */
    // int CuckooGraphTypeRange_RedisCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
    // {
    //     RedisModule_AutoMemory(ctx); /* Use automatic memory management. */

    //     if (argc != 4)
    //         return RedisModule_WrongArity(ctx);
    //     RedisModuleKey *key = RedisModule_OpenKey(ctx, argv[1],
    //                                               REDISMODULE_READ | REDISMODULE_WRITE);
    //     int type = RedisModule_KeyType(key);
    //     if (type != REDISMODULE_KEYTYPE_EMPTY &&
    //         RedisModule_ModuleTypeGetType(key) != CuckooGraphType)
    //     {
    //         return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
    //     }

    //     long long first, count;
    //     if (RedisModule_StringToLongLong(argv[2], &first) != REDISMODULE_OK ||
    //         RedisModule_StringToLongLong(argv[3], &count) != REDISMODULE_OK ||
    //         first < 0 || count < 0)
    //     {
    //         return RedisModule_ReplyWithError(ctx,
    //                                           "ERR invalid first or count parameters");
    //     }

    //     struct CuckooGraphTypeObject *hto = RedisModule_ModuleTypeGetValue(key);
    //     struct CuckooGraphTypeNode *node = hto ? hto->head : NULL;
    //     RedisModule_ReplyWithArray(ctx, REDISMODULE_POSTPONED_LEN);
    //     long long arraylen = 0;
    //     while (node && count--)
    //     {
    //         RedisModule_ReplyWithLongLong(ctx, node->value);
    //         arraylen++;
    //         node = node->next;
    //     }
    //     RedisModule_ReplySetArrayLength(ctx, arraylen);
    //     return REDISMODULE_OK;
    // }


    /* CuckooGraphType.Query key src */
    int CuckooGraphTypeGetNeighbors_RedisCommand(RedisModuleCtx* ctx, RedisModuleString** argv, int argc)
    {
        RedisModule_AutoMemory(ctx); /* Use automatic memory management. */

        if (argc != 3)
            return RedisModule_WrongArity(ctx);
        RedisModuleKey* key = RedisModule_OpenKey(ctx, argv[1],
            REDISMODULE_READ | REDISMODULE_WRITE);
        int type = RedisModule_KeyType(key);
        if (type != REDISMODULE_KEYTYPE_EMPTY &&
            RedisModule_ModuleTypeGetType(key) != CuckooGraphType)
        {
            return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
        }

        long long src;
        if (RedisModule_StringToLongLong(argv[2], &src) != REDISMODULE_OK)
        {
            return RedisModule_ReplyWithError(ctx, "ERR invalid src: must be a signed 64 bit integer");
        }

        /* Create an empty value object if the key is currently empty. */
        struct CuckooGraphTypeObject* hto;
        if (type == REDISMODULE_KEYTYPE_EMPTY)
        {
            hto = createCuckooGraphTypeObject();
            RedisModule_ModuleTypeSetValue(key, CuckooGraphType, hto);
        }
        else
        {
            hto = (struct CuckooGraphTypeObject*)RedisModule_ModuleTypeGetValue(key);
        }


        hto = RedisModule_ModuleTypeGetValue(key);

        int* addr, len;
        RedisModule_ReplyWithArray(ctx, REDISMODULE_POSTPONED_LEN);
        GetNeighborsInternal(hto, src, &addr, &len);
        // long long arraylen = 0;
        // while (node && count--)
        // {
        //     RedisModule_ReplyWithLongLong(ctx, node->value);
        //     arraylen++;
        //     node = node->next;
        // }

        for (int i = 0; i < len; i++)
        {
            RedisModule_ReplyWithLongLong(ctx, (long long)addr[i]);
        }
        free(addr);
        RedisModule_ReplySetArrayLength(ctx, len);
        return REDISMODULE_OK;
    }


    /* CuckooGraphType.Query key src dst */
    int CuckooGraphTypeQuery_RedisCommand(RedisModuleCtx* ctx, RedisModuleString** argv, int argc)
    {
        RedisModule_AutoMemory(ctx); /* Use automatic memory management. */

        if (argc != 4)
            return RedisModule_WrongArity(ctx);
        RedisModuleKey* key = RedisModule_OpenKey(ctx, argv[1],
            REDISMODULE_READ | REDISMODULE_WRITE);
        int type = RedisModule_KeyType(key);
        if (type != REDISMODULE_KEYTYPE_EMPTY &&
            RedisModule_ModuleTypeGetType(key) != CuckooGraphType)
        {
            return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
        }

        long long src, dst;
        if ((RedisModule_StringToLongLong(argv[2], &src) != REDISMODULE_OK))
        {
            return RedisModule_ReplyWithError(ctx, "ERR invalid src: must be a signed 64 bit integer");
        }

        if ((RedisModule_StringToLongLong(argv[3], &dst) != REDISMODULE_OK))
        {
            return RedisModule_ReplyWithError(ctx, "ERR invalid dst: must be a signed 64 bit integer");
        }

        /* Create an empty value object if the key is currently empty. */
        struct CuckooGraphTypeObject* hto;
        if (type == REDISMODULE_KEYTYPE_EMPTY)
        {
            hto = createCuckooGraphTypeObject();
            RedisModule_ModuleTypeSetValue(key, CuckooGraphType, hto);
        }
        else
        {
            hto = (struct CuckooGraphTypeObject*)RedisModule_ModuleTypeGetValue(key);
        }

        hto = RedisModule_ModuleTypeGetValue(key);
        // RedisModule_ReplyWithLongLong(ctx, hto ? hto->len : 0);
        int ans = QueryInternal(hto, src, dst);
        //RedisModule_ReplyWithBool(ctx, ans);
        RedisModule_ReplyWithLongLong(ctx, ans);
        return REDISMODULE_OK;
    }

    /* CuckooGraphType.Del key src dst */
    int CuckooGraphTypeDel_RedisCommand(RedisModuleCtx* ctx, RedisModuleString** argv, int argc)
    {
        RedisModule_AutoMemory(ctx); /* Use automatic memory management. */

        if (argc != 4)
            return RedisModule_WrongArity(ctx);
        RedisModuleKey* key = RedisModule_OpenKey(ctx, argv[1],
            REDISMODULE_READ | REDISMODULE_WRITE);
        int type = RedisModule_KeyType(key);
        if (type != REDISMODULE_KEYTYPE_EMPTY &&
            RedisModule_ModuleTypeGetType(key) != CuckooGraphType)
        {
            return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
        }

        long long src, dst;
        if ((RedisModule_StringToLongLong(argv[2], &src) != REDISMODULE_OK))
        {
            return RedisModule_ReplyWithError(ctx, "ERR invalid src: must be a signed 64 bit integer");
        }

        if ((RedisModule_StringToLongLong(argv[3], &dst) != REDISMODULE_OK))
        {
            return RedisModule_ReplyWithError(ctx, "ERR invalid dst: must be a signed 64 bit integer");
        }

        /* Create an empty value object if the key is currently empty. */
        struct CuckooGraphTypeObject* hto;
        if (type == REDISMODULE_KEYTYPE_EMPTY)
        {
            hto = createCuckooGraphTypeObject();
            RedisModule_ModuleTypeSetValue(key, CuckooGraphType, hto);
        }
        else
        {
            hto = (struct CuckooGraphTypeObject*)RedisModule_ModuleTypeGetValue(key);
        }

        hto = RedisModule_ModuleTypeGetValue(key);
        DelInternal(hto, src, dst);
        RedisModule_ReplyWithString(ctx, RedisModule_CreateString(ctx, "Delete over", 11));
        return REDISMODULE_OK;
    }


    // /* ====================== Example of a blocking command ==================== */

    // /* Reply callback for blocking command CuckooGraphType.BRANGE, this will get
    //  * called when the key we blocked for is ready: we need to check if we
    //  * can really serve the client, and reply OK or ERR accordingly. */
    // int CuckooGraphBlock_Reply(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
    // {
    //     REDISMODULE_NOT_USED(argv);
    //     REDISMODULE_NOT_USED(argc);

    //     RedisModuleString *keyname = RedisModule_GetBlockedClientReadyKey(ctx);
    //     RedisModuleKey *key = RedisModule_OpenKey(ctx, keyname, REDISMODULE_READ);
    //     int type = RedisModule_KeyType(key);
    //     if (type != REDISMODULE_KEYTYPE_MODULE ||
    //         RedisModule_ModuleTypeGetType(key) != CuckooGraphType)
    //     {
    //         RedisModule_CloseKey(key);
    //         return REDISMODULE_ERR;
    //     }

    //     /* In case the key is able to serve our blocked client, let's directly
    //      * use our original command implementation to make this example simpler. */
    //     RedisModule_CloseKey(key);
    //     return CuckooGraphTypeRange_RedisCommand(ctx, argv, argc - 1);
    // }

    // /* Timeout callback for blocking command CuckooGraphType.BRANGE */
    // int CuckooGraphBlock_Timeout(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
    // {
    //     REDISMODULE_NOT_USED(argv);
    //     REDISMODULE_NOT_USED(argc);
    //     return RedisModule_ReplyWithSimpleString(ctx, "Request timedout");
    // }

    // /* Private data freeing callback for CuckooGraphType.BRANGE command. */
    // void CuckooGraphBlock_FreeData(RedisModuleCtx *ctx, void *privdata)
    // {
    //     REDISMODULE_NOT_USED(ctx);
    //     RedisModule_Free(privdata);
    // }

    /* CuckooGraphType.BRANGE key first count timeout -- This is a blocking version of
     * the RANGE operation, in order to show how to use the API
     * RedisModule_BlockClientOnKeys(). */
     // int CuckooGraphTypeBRange_RedisCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
     // {
     //     if (argc != 5)
     //         return RedisModule_WrongArity(ctx);
     //     RedisModule_AutoMemory(ctx); /* Use automatic memory management. */
     //     RedisModuleKey *key = RedisModule_OpenKey(ctx, argv[1],
     //                                               REDISMODULE_READ | REDISMODULE_WRITE);
     //     int type = RedisModule_KeyType(key);
     //     if (type != REDISMODULE_KEYTYPE_EMPTY &&
     //         RedisModule_ModuleTypeGetType(key) != CuckooGraphType)
     //     {
     //         return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
     //     }

     //     /* Parse the timeout before even trying to serve the client synchronously,
     //      * so that we always fail ASAP on syntax errors. */
     //     long long timeout;
     //     if (RedisModule_StringToLongLong(argv[4], &timeout) != REDISMODULE_OK)
     //     {
     //         return RedisModule_ReplyWithError(ctx,
     //                                           "ERR invalid timeout parameter");
     //     }

     //     /* Can we serve the reply synchronously? */
     //     if (type != REDISMODULE_KEYTYPE_EMPTY)
     //     {
     //         return CuckooGraphTypeRange_RedisCommand(ctx, argv, argc - 1);
     //     }

     //     /* Otherwise let's block on the key. */
     //     void *privdata = RedisModule_Alloc(100);
     //     RedisModule_BlockClientOnKeys(ctx, CuckooGraphBlock_Reply, CuckooGraphBlock_Timeout, CuckooGraphBlock_FreeData, timeout, argv + 1, 1, privdata);
     //     return REDISMODULE_OK;
     // }

     /* ========================== "CuckooGraphType" type methods ======================= */

     // void *CuckooGraphTypeRdbLoad(RedisModuleIO *rdb, int encver)
     // {
     //     if (encver != 0)
     //     {
     //         /* RedisModule_Log("warning","Can't load data with version %d", encver);*/
     //         return NULL;
     //     }
     //     uint64_t elements = RedisModule_LoadUnsigned(rdb);
     //     struct CuckooGraphTypeObject *hto = createCuckooGraphTypeObject();
     //     while (elements--)
     //     {
     //         int64_t ele = RedisModule_LoadSigned(rdb);
     //         CuckooGraphTypeInsert(hto, ele);
     //     }
     //     return hto;
     // }

    void* CuckooGraphTypeRdbLoad(RedisModuleIO* rdb, int encver)
    {
        // exit(1);
        if (encver != 0)
        {
            /* RedisModule_Log("warning","Can't load data with version %d", encver);*/
            return NULL;
        }

        struct CuckooGraphTypeObject* hto = createCuckooGraphTypeObject();
        // return hto;
        // CreateUnorderedMap(hto);
        uint64_t numOfVertex = RedisModule_LoadUnsigned(rdb);
        for (int i = 0; i < numOfVertex; i++)
        {
            int src = RedisModule_LoadUnsigned(rdb);
            int lenSrc = RedisModule_LoadUnsigned(rdb);
            UpdateUnorderedMap(hto, src);
            for (int j = 0; j < lenSrc; j++)
            {
                int dst = RedisModule_LoadUnsigned(rdb);
                CuckooGraphTypeInsert(hto, src, dst);
                UpdateUnorderedMap(hto, dst);
            }
        }
        return hto;
    }

    void CuckooGraphTypeRdbSave(RedisModuleIO* rdb, void* value)
    {
        struct CuckooGraphTypeObject* hto = value;

        // struct CuckooGraphTypeNode *node = hto->head;
        // RedisModule_SaveUnsigned(rdb, hto->len);
        // RedisModule_SaveUnsigned(rdb, hto->Graph);
        // RedisModule_SaveUnsigned(rdb, hto->VertexCount);
        // exit(1);
        int* addr, len;
        GetAllVertices(hto, &addr, &len);
        RedisModule_SaveUnsigned(rdb, len);

        // while (node)
        // {
        //     RedisModule_SaveSigned(rdb, node->value);
        //     node = node->next;
        // }
        for (int i = 0; i < len; i++)
        {
            int src = addr[i];
            int* addrSrc, lenSrc;
            GetNeighborsInternal(hto, src, &addrSrc, &lenSrc);
            RedisModule_SaveUnsigned(rdb, src);
            RedisModule_SaveUnsigned(rdb, lenSrc);
            for (int j = 0; j < lenSrc; j++)
                RedisModule_SaveUnsigned(rdb, addrSrc[j]);
            free(addrSrc);
        }
        free(addr);
    }

    // void CuckooGraphTypeAofRewrite(RedisModuleIO *aof, RedisModuleString *key, void *value)
    // {
    //     struct CuckooGraphTypeObject *hto = value;
    //     struct CuckooGraphTypeNode *node = hto->head;
    //     while (node)
    //     {
    //         RedisModule_EmitAOF(aof, "CuckooGraphType.INSERT", "sl", key, node->value);
    //         node = node->next;
    //     }
    // }

    void CuckooGraphTypeAofRewrite(RedisModuleIO* aof, RedisModuleString* key, void* value)
    {
        struct CuckooGraphTypeObject* hto = value;
        // struct CuckooGraphTypeNode *node = hto->head;

        int* addr, len;
        // exit(1);
        GetAllVertices(hto, &addr, &len);
        // while (node)
        // {
        //     RedisModule_EmitAOF(aof, "CuckooGraphType.INSERT", "sl", key, node->value);
        //     node = node->next;
        // }

        for (int i = 0; i < len; i++)
        {
            int* addrVertex, lenVertex;
            GetNeighborsInternal(hto, addr[i], &addrVertex, &lenVertex);
            int src = addr[i];
            for (int j = 0; j < lenVertex; j++)
            {
                int dst = addrVertex[j];
                // /* CuckooGraphType.INSERT key src dst */
                // int CuckooGraphTypeInsert_RedisCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
                RedisModule_EmitAOF(aof, "CuckooGraphType.INSERT", "sll", key, src, dst);
            }
            free(addrVertex);
        }
        free(addr);

    }

    /* The goal of this function is to return the amount of memory used by
     * the CuckooGraphType value. */
     // size_t CuckooGraphTypeMemUsage(const void *value)
     // {
     //     const struct CuckooGraphTypeObject *hto = value;
     //     struct CuckooGraphTypeNode *node = hto->head;
     //     return sizeof(*hto) + sizeof(*node) * hto->len;
     // }

    void CuckooGraphTypeFree(void* value)
    {
        //return;
        CuckooGraphTypeReleaseObjectInternal(value);
        RedisModule_Free(value);
    }

    // void CuckooGraphTypeDigest(RedisModuleDigest *md, void *value)
    // {
    //     struct CuckooGraphTypeObject *hto = value;
    //     struct CuckooGraphTypeNode *node = hto->head;
    //     while (node)
    //     {
    //         RedisModule_DigestAddLongLong(md, node->value);
    //         node = node->next;
    //     }
    //     RedisModule_DigestEndSequence(md);
    // }


    size_t CuckooGraphTypeMemUsage(const void* value)
    {
        return 0;
    }

    void CuckooGraphTypeDigest(RedisModuleDigest* md, void* value)
    {
        struct CuckooGraphTypeObject* hto = value;
        // struct CuckooGraphTypeNode *node = hto->head;
        // while (node)
        // {
        //     RedisModule_DigestAddLongLong(md, node->value);
        //     node = node->next;
        // }
        RedisModule_DigestAddLongLong(md, 0);
        RedisModule_DigestEndSequence(md);
    }

    /* This function must be present on each Redis module. It is used in order to
     * register the commands into the Redis server. */
    int RedisModule_OnLoad(RedisModuleCtx* ctx, RedisModuleString** argv, int argc)
    {
        REDISMODULE_NOT_USED(argv);
        REDISMODULE_NOT_USED(argc);
        if (RedisModule_Init(ctx, "CuckGrphT", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR)
            return REDISMODULE_ERR;

        RedisModuleTypeMethods tm = {
            .version = REDISMODULE_TYPE_METHOD_VERSION,
            .rdb_load = CuckooGraphTypeRdbLoad,
            .rdb_save = CuckooGraphTypeRdbSave,
            .aof_rewrite = CuckooGraphTypeAofRewrite,
            .mem_usage = CuckooGraphTypeMemUsage,
            .free = CuckooGraphTypeFree,
            .digest = CuckooGraphTypeDigest };

        CuckooGraphType = RedisModule_CreateDataType(ctx, "CuckGrphT", 0, &tm);
        //     if (CuckooGraphType == NULL)
        //         return REDISMODULE_ERR;
        //    exit(1451);
        if (RedisModule_CreateCommand(ctx, "CuckGrphT.insert",
            CuckooGraphTypeInsert_RedisCommand, "write", 1, 1, 1) == REDISMODULE_ERR)
            return REDISMODULE_ERR;


        if (RedisModule_CreateCommand(ctx, "CuckGrphT.getneighbors",
            CuckooGraphTypeGetNeighbors_RedisCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR)
            return REDISMODULE_ERR;

        if (RedisModule_CreateCommand(ctx, "CuckGrphT.del",
            CuckooGraphTypeDel_RedisCommand, "write", 1, 1, 1) == REDISMODULE_ERR)
            return REDISMODULE_ERR;

        if (RedisModule_CreateCommand(ctx, "CuckGrphT.query",
            CuckooGraphTypeQuery_RedisCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR)
            return REDISMODULE_ERR;

        // if (RedisModule_CreateCommand(ctx, "CuckooGraphType.range",
        //                               CuckooGraphTypeRange_RedisCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR)
        //     return REDISMODULE_ERR;

        // if (RedisModule_CreateCommand(ctx, "CuckooGraphType.len",
        //                               CuckooGraphTypeLen_RedisCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR)
        //     return REDISMODULE_ERR;

        // if (RedisModule_CreateCommand(ctx, "CuckooGraphType.brange",
        //                               CuckooGraphTypeBRange_RedisCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR)
        //     return REDISMODULE_ERR;


        return REDISMODULE_OK;
    }

#ifdef __cplusplus
}
#endif