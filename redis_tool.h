//
// Created by shotacon on 23-6-19.
//

#ifndef REDISTOOL_REDIS_TOOL_H
#define REDISTOOL_REDIS_TOOL_H

#include <string>
#define _DEF_REDIS_IP "127.0.0.1"
#define _DEF_REDIS_POST 6379

#include "hiredis/hiredis.h"
#include "string.h"
using std::string;

namespace stc{
    class RedisTool{
    public:
        RedisTool();
        ~RedisTool();

        //string
        bool setString(const string& key, const string& val);
        string getString(const string& key);

    private:
        void init();


        static string m_redis_ip;
        static short m_redis_port;
        redisContext* m_redis;
    };
}


#endif //REDISTOOL_REDIS_TOOL_H
