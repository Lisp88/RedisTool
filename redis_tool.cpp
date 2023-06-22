//
// Created by shotacon on 23-6-19.
//

#include "redis_tool.h"
using namespace stc;

std::string RedisTool::m_redis_ip = _DEF_REDIS_IP;
short RedisTool::m_redis_port = _DEF_REDIS_POST;

stc::RedisTool::RedisTool() {
    init();
}

void stc::RedisTool::init() {
    //连接redis
    struct timeval timeout = {1,  500000};//1.5s 秒和微秒
    m_redis = redisConnectWithTimeout(m_redis_ip.c_str(), m_redis_port, timeout);
    //检测
    if(m_redis->err){
        printf("connect error << %s \n", m_redis->errstr);
    }else
        printf("connect success!\n");
}

RedisTool::~RedisTool() {
    if(m_redis){
        redisFree(m_redis);
        printf("free connect\n");
    }
}

bool RedisTool::setString(const string& key, const string& val) {
    if(!m_redis || m_redis->err){
        printf("redis init error");
        init();
        return false;
    }
    //请求
    redisReply * reply;
    reply = static_cast<redisReply *>(redisCommand(m_redis, "set %s %s", key.c_str(), val.c_str()));
    //返回值
    bool flag = false;
    if(reply){
        if(strcmp(reply->str, "OK") == 0){
            flag = true;
        }else{
            flag = false;
            printf("set string error >> reply != ok\n");
        }
    }else {
        redisFree(m_redis);
        m_redis = nullptr;
        flag = false;
        printf("set string error >> reply = null\n");
    }
    //回收返回值
    freeReplyObject(reply);
    return flag;
}

bool RedisTool::getString(const string& key, string& val) {
    if(!m_redis || m_redis->err){
        printf("redis init error");
        init();
        return false;
    }
    bool flag = false;
    //请求
    redisReply * reply;
    reply = static_cast<redisReply *>(redisCommand(m_redis, "get %s", key.c_str()));
    //校验返回值
    if(reply){
        if(reply->len > 0){
            val = reply->str;
            flag = true;
        }
    }else {
        redisFree(m_redis);
        m_redis = nullptr;
        printf("get string error >> reply = null\n");
    }
    //回收返回值
    freeReplyObject(reply);
    return flag;
}

bool RedisTool::isExiist(const string &key) {
    if(!m_redis || m_redis->err){
        printf("redis init error");
        init();
        return false;
    }
    bool flag = false;
    //请求
    redisReply * reply;
    reply = static_cast<redisReply *>(redisCommand(m_redis, "get %s", key.c_str()));
    //校验返回值
    if(reply){
        if(reply->len > 0){
            flag = true;
        }
    }else {
        redisFree(m_redis);
        m_redis = nullptr;
        printf("get string error >> reply = null\n");
    }
    //回收返回值
    freeReplyObject(reply);
    return flag;
}

void RedisTool::delKey(const string &key) {
    if(!m_redis || m_redis->err){
        printf("redis init error");
        init();
        return;
    }
    //请求
    redisReply * reply;
    reply = static_cast<redisReply *>(redisCommand(m_redis, "del %s", key.c_str()));
    //校验返回值
    if(reply){
    }else {
        redisFree(m_redis);
        m_redis = nullptr;
        printf("del key error >> reply = null\n");
    }
    //回收返回值
    freeReplyObject(reply);
}

bool RedisTool::setList(const string &key, const list<string> &val) {
    if(!m_redis || m_redis->err){
        printf("redis init error");
        init();
        return false;
    }
    //请求
    redisReply * reply;
    delKey(key);
    int count = 0;
    for(auto it = val.begin(); it != val.end(); ++it){
        reply = static_cast<redisReply *>(redisCommand(m_redis, "rpush %s %s", key.c_str(), it->c_str()));
        //校验返回值
        if(reply){
            if(reply->integer > 0) {
                ++count;
            }
        }else {
            redisFree(m_redis);
            m_redis = nullptr;
            printf("set list error >> reply = null\n");
            return false;
        }
        //回收返回值
        freeReplyObject(reply);
    }
    if(count == val.size()) return true;
    return false;
}

bool RedisTool::appendList(const string &key, const string &val) {
    if(!m_redis || m_redis->err){
        printf("redis init error");
        init();
        return false;
    }
    bool flag = false;
    //请求
    redisReply * reply;
    reply = static_cast<redisReply *>(redisCommand(m_redis, "rpush %s %s", key.c_str(), val.c_str()));
    //校验返回值
    if(reply){
        if(reply->integer > 0) {
            flag = true;
        }
    }else {
        redisFree(m_redis);
        m_redis = nullptr;
        printf("append list error >> reply = null\n");
    }
    //回收返回值
    freeReplyObject(reply);
    return flag;
}

bool RedisTool::getList(const string &key, list<string> &val_list) {
    if(!m_redis || m_redis->err){
        printf("redis init error");
        init();
        return false;
    }
    bool flag = false;
    //请求
    redisReply * reply;
    reply = static_cast<redisReply *>(redisCommand(m_redis, "lrange %s 0 -1", key.c_str()));
    //校验返回值
    if(reply){
        if(reply->type == REDIS_REPLY_ARRAY){
            for(unsigned i = 0; i < reply->elements; ++i){
                val_list.emplace_back(reply->element[i]->str);
            }
            flag = true;
        }
    }else {
        redisFree(m_redis);
        m_redis = nullptr;
        printf("set list error >> reply = null\n");
    }
    //回收返回值
    freeReplyObject(reply);
    return flag;
}

bool RedisTool::setHash(const string &key, const string &field, const string &val) {
    if(!m_redis || m_redis->err){
        printf("redis init error");
        init();
        return false;
    }
    bool flag = false;
    //请求
    redisReply * reply;
    reply = static_cast<redisReply *>(redisCommand(m_redis, "hset %s %s %s", key.c_str(), field.c_str(), val.c_str()));
    //校验返回值
    if(reply){
        if(reply->len > 0){
            flag = true;
        }
    }else {
        redisFree(m_redis);
        m_redis = nullptr;
        printf("set hash error >> reply = null\n");
    }
    //回收返回值
    freeReplyObject(reply);
    return flag;
}

bool RedisTool::getHash(const string &key, const string &field, string &val) {
    if (!m_redis || m_redis->err) {
        printf("redis init error");
        init();
        return false;
    }
    bool flag = false;
    //请求
    redisReply *reply;
    reply = static_cast<redisReply *>(redisCommand(m_redis, "hget %s %s", key.c_str(), field.c_str()));
    //校验返回值
    if (reply) {
        if (reply->len > 0) {
            flag = true;
            val = reply->str;
        }
    } else {
        redisFree(m_redis);
        m_redis = nullptr;
        printf("get hash error >> reply = null\n");
    }
    //回收返回值
    freeReplyObject(reply);
    return flag;
}

bool RedisTool::delHashKey(const string &key, const string &field) {
    if (!m_redis || m_redis->err) {
        printf("redis init error");
        init();
        return false;
    }
    bool flag = false;
    //请求
    redisReply *reply;
    reply = static_cast<redisReply *>(redisCommand(m_redis, "hdel %s %s", key.c_str(), field.c_str()));
    //校验返回值
    if (reply) {
        if (reply->len > 0) {
            flag = true;
        }
    } else {
        redisFree(m_redis);
        m_redis = nullptr;
        printf("del hash field error >> reply = null\n");
    }
    //回收返回值
    freeReplyObject(reply);
    return flag;
}

bool RedisTool::getHashKey(const string &key, list<string> &field_list) {
    if (!m_redis || m_redis->err) {
        printf("redis init error");
        init();
        return false;
    }
    bool flag = false;
    //请求
    redisReply *reply;
    reply = static_cast<redisReply *>(redisCommand(m_redis, "hkeys %s", key.c_str()));
    //校验返回值
    if (reply) {
        if (reply->type == REDIS_REPLY_ARRAY) {
            flag = true;
            for(unsigned i = 0; i < reply->elements; ++i){
                field_list.emplace_back(reply->element[i]->str);
            }
        }
    } else {
        redisFree(m_redis);
        m_redis = nullptr;
        printf("get hash fields error >> reply = null\n");
    }
    //回收返回值
    freeReplyObject(reply);
    return flag;
}
