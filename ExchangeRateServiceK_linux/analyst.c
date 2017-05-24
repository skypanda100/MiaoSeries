//
// Created by root on 5/24/17.
//
#include "analyst.h"

static const char *K_PATTERN_FMT = ".*<td.*\"first left bold noWrap\">%d年%d月%d日</td>[\t\r\n ]*"
                                "<td.*>([0-9\\.]+)</td>[\t\r\n ]*"
                                "<td.*>([0-9\\.]+)</td>[\t\r\n ]*"
                                "<td.*>([0-9\\.]+)</td>[\t\r\n ]*"
                                "<td.*>([0-9\\.]+)</td>.*";

static int find_k(const char *pattern, const char *content, char *close, char *open, char *high, char *low)
{
    int ret = 0;
    char *k_array[4];

    k_array[0] = close;
    k_array[1] = open;
    k_array[2] = high;
    k_array[3] = low;
    ret = util_find_string_by_regex(pattern, content, k_array, sizeof(k_array) / sizeof(char *));

    return ret;
}

static void format_time(const char *src_time, char *dst_time)
{
    strncpy(dst_time, src_time, 4);
    strcat(dst_time, "-");
    strncat(dst_time, src_time + 5, 2);
    strcat(dst_time, "-");
    strncat(dst_time, src_time + 8, 2);
    strcat(dst_time, " 00:00:00");
}

static void save_data(PGconn *conn
        , const char *f_time, const char *t_time
        , const char *new_account, const char *natural_account
        , const char *none_natural_account, const char *final_account)
{
    char db_insert[128] = {0};
    const char *db_insert_params[6] = {NULL};

    strcpy(db_insert, "insert into stock_account values ($1, $2, $3, $4, $5, $6)");
    db_insert_params[0] = f_time;
    db_insert_params[1] = t_time;
    db_insert_params[2] = new_account;
    db_insert_params[3] = natural_account;
    db_insert_params[4] = none_natural_account;
    db_insert_params[5] = final_account;
    printf("%s\n", db_insert);

    util_db_insert(conn, db_insert, db_insert_params, sizeof(db_insert_params) / sizeof(char *));
}

void analyst_execute(const struct Chunk *pchunk)
{
    PGconn *conn = NULL;

    if(util_db_connect(&conn) != 0)
    {
        util_db_disconnect(conn);
        return;
    }

    struct tm *src_t = NULL;
    time_t src_timet = 0L;

    src_timet = time((time_t *)NULL);
    src_t = localtime(&src_timet);

    int now_year = src_t->tm_year + 1900;
    int now_month = src_t->tm_mon + 1;
    int now_day = src_t->tm_mday;

    char now_date[14 + 1] = {0};
    sprintf(now_date, "%d%02d%02d000000", now_year, now_month, now_day);

    printf("------------------------\n");
    for(int i = 0;i < 10;i++)
    {
        int ret = 0;
        char offset_date[14 + 1] = {0};
        char offset_year[4 + 1] = {0};
        char offset_month[2 + 1] = {0};
        char offset_day[2 + 1] = {0};
        char k_pattern[1024] = {0};
        char close[12] = {0};
        char open[12] = {0};
        char high[12] = {0};
        char low[12] = {0};

        util_make_time_by_offset(now_date, offset_date, i * -86400);
        strncpy(offset_year, offset_date, 4);
        strncpy(offset_month, offset_date + 4, 2);
        strncpy(offset_day, offset_date + 6, 2);
        sprintf(k_pattern, K_PATTERN_FMT, atoi(offset_year), atoi(offset_month), atoi(offset_day));

        ret = find_k(k_pattern, pchunk->memory, close, open, high, low);
        if(ret == 0)
        {
            printf("%s\t%s\t%s\t%s\t%s\n",offset_date, close, open, high, low);
        }
        else
        {
            printf("%s\n",offset_date);
        }
    }

    util_db_disconnect(conn);
}
