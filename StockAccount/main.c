#include <unistd.h>
#include <stdlib.h>
#include "global.h"
#include "spider.h"
#include "analyst.h"

int main() {
    int ret = 0;
    struct Chunk chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    ret = spider_execute(&chunk);

    if(ret == 0)
    {
        ret = analyst_execute(&chunk);
        if(ret != 0)
        {
            printf("analyst_execute error\n");
        }
    }

    return 0;
}