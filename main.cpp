#include "redis_tool.h"
using namespace stc;

int main() {
    RedisTool redis;
    redis.setString("3", "4");
    printf("%s", redis.getString("3").c_str());
    return 0;
}
