#include "redisDriver.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>

RedisDriver::RedisDriver(void)
{
    system("rm /var/lib/redis/dump.rdb; rm ./dump.rdb; redis-cli shutdown; redis-server --loadmodule ./cg.so --daemonize yes");
    sleep(5);
    ctx = redisConnect("127.0.0.1", 6379);
    system("redis-cli CONFIG SET save \"\"; redis-cli CONFIG SET appendonly no");
    if (ctx == nullptr || ctx->err) {
        if (ctx) {
            std::cerr << "fail: " << ctx->errstr << std::endl;
            redisFree(ctx);
        }
        else {
            std::cerr << "fail to allocate redis context" << std::endl;
        }
    }
}

RedisDriver::~RedisDriver(void)
{
    redisFree(this->ctx);
}

bool RedisDriver::Insert(int src, int dst)
{
    redisReply* reply = (redisReply*)redisCommand(this->ctx, "CUCKGRPHT.INSERT %s %d %d", this->KEY, src, dst);
    if (reply->type == REDIS_REPLY_ERROR) {
        std::cerr << "Insertion failed: " << reply->str << std::endl;
        freeReplyObject(reply);
        return false;
    }
    freeReplyObject(reply);
    return true;
}

bool RedisDriver::Del(int src, int dst)
{
    redisReply* reply = (redisReply*)redisCommand(this->ctx, "CUCKGRPHT.DEL %s %d %d", this->KEY, src, dst);
    if (reply->type == REDIS_REPLY_ERROR) {
        std::cerr << "Deletion failed: " << reply->str << std::endl;
        freeReplyObject(reply);
        return false;
    }
    freeReplyObject(reply);
    return true;
}

bool RedisDriver::Query(int src, int dst, bool& edgeExist)
{
    redisReply* reply = (redisReply*)redisCommand(this->ctx, "CUCKGRPHT.QUERY %s %d %d", this->KEY, src, dst);
    if (reply->type == REDIS_REPLY_ERROR) {
        std::cerr << "Query failed: " << reply->str << std::endl;
        freeReplyObject(reply);
        return false;
    }
    edgeExist = reply->integer == 1;
    freeReplyObject(reply);
    return true;
}
