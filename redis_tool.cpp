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

string RedisTool::getString(const string& key) {
    string str_res;
    if(!m_redis || m_redis->err){
        printf("redis init error");
        init();
        return str_res;
    }
    //请求
    redisReply * reply;
    reply = static_cast<redisReply *>(redisCommand(m_redis, "get %s", key.c_str()));
    //校验返回值
    if(reply){
        if(reply->len > 0){
            str_res = reply->str;
        }
    }else {
        redisFree(m_redis);
        m_redis = nullptr;
        printf("get string error >> reply = null\n");
    }
    //回收返回值
    freeReplyObject(reply);
    return str_res;
}
