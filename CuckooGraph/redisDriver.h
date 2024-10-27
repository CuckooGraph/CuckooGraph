#pragma once
#ifndef REDISDRIVER
#define REDISDRIVER

#include <hiredis/hiredis.h>
#include <string>

class RedisDriver
{
public:
    RedisDriver(void);
    ~RedisDriver(void);
    bool Insert(int src, int dst); // return true if success
    bool Query(int src, int dst, bool& edgeExist); // return true if success; edgeExist is true if <src, dst> exists;
    bool Del(int src, int dst); // return true if success

private:
    redisContext* ctx;
    const std::string KEY = "TEMP"; // useless
};

#endif