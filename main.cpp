#include "redis_tool.h"
using namespace stc;

int main() {
    RedisTool redis;
    list<string> a;
    redis.getHashKey("k", a);
    printf("%s", a.begin()->c_str());
    return 0;
}
