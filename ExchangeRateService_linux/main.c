#include <unistd.h>
#include <stdlib.h>
#include "global.h"
#include "spider.h"
#include "analyst.h"

static int is_time_to_execute()
{
    struct tm *src_t = NULL;
    time_t src_timet = 0L;
    int minute = 0;
    int hour = 0;
    int week_day = 0;

    src_timet = time((time_t *)NULL);
    src_t = localtime(&src_timet);
    minute = src_t->tm_min;
    hour = src_t->tm_hour;
    week_day = src_t->tm_wday;

    /*
     * 从星期一早上六点到星期六早上六点执行抓取动作
     */
    if((week_day >= 2 && week_day <= 5)
       || (week_day == 1 && hour >= 6)
       || (week_day == 6 && hour < 6))
    {
        if(minute % 5 == 0)
        {
            return 1;
        }
    }

    return 0;
}

int main(void)
{
    daemon(0, 0);

    int ret = 0;
    struct Chunk chunk;

    for(;;)
    {
        sleep(60);

        if(is_time_to_execute() > 0)
        {
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

            free(chunk.memory);
        }
    }
}

/*
int main(void)
{
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

    free(chunk.memory);
}*/
