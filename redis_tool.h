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
#include "list"
using std::string;
using std::list;


namespace stc{
    class RedisTool{
    public:
        RedisTool();
        ~RedisTool();

        bool isExiist(const string& key);
        void delKey(const string& key);
        //string
        bool setString(const string& key, const string& val);
        bool getString(const string& key, string& val);//传出参数val，返回值是否存在
        //list
        bool setList(const string& key, const list<string>& val_list);
        bool appendList(const string& key, const string& val);
        bool getList(const string& key, list<string>& val_list);
        //Hash
        bool setHash(const string& key, const string& field, const string& val);
        bool getHash(const string& key, const string& field, string& val);
        bool delHashKey(const string& key, const string& field);
        bool getHashKey(const string& key, list<string>& field_list);//传出field列表
    private:
        void init();


        static string m_redis_ip;
        static short m_redis_port;
        redisContext* m_redis;
    };
}


#endif //REDISTOOL_REDIS_TOOL_H
