#include "redis_tool.h"
using namespace stc;

int main() {
    RedisTool redis;
    list<string> val_list{"sda", "sss", "oso"};
    list<string> temp;
    redis.appendList("897", "haha");
    redis.getList("897", temp);

    for(auto i : temp)
        printf("%s\n", i.c_str());

    return 0;
}
