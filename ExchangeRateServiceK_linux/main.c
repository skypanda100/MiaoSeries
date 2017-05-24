#include <unistd.h>
#include <stdlib.h>
#include "global.h"
#include "spider.h"
#include "analyst.h"

int main(void)
{
    daemon(0, 0);

    int ret = 0;
    struct Chunk chunk;

    for(;;)
    {
        chunk.memory = malloc(1);
        chunk.size = 0;

        ret = spider_execute(&chunk);
        if(ret == 0)
        {
            analyst_execute(&chunk);
        }

        free(chunk.memory);

        sleep(3600);
    }
}
