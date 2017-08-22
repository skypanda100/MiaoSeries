//
// Created by root on 5/24/17.
//
#include "analyst.h"

static const char *K_PATTERN_FMT = ".*<td.*\"first left bold noWrap\">%d年%d月%d日</td>[\t\r\n ]*"
                                "<td[^\r\n]*>([0-9\\.]+)</td>[\t\r\n ]*"
                                "<td[^\r\n]*>([0-9\\.]+)</td>[\t\r\n ]*"
                                "<td[^\r\n]*>([0-9\\.]+)</td>[\t\r\n ]*"
                                "<td[^\r\n]*>([0-9\\.]+)</td>.*";

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
    strncat(dst_time, src_time + 4, 2);
    strcat(dst_time, "-");
    strncat(dst_time, src_time + 6, 2);
    strcat(dst_time, " 00:00:00");
}

static int is_data_exist(PGconn *conn, char *time)
{
    char db_query[512] = {0};
    PGresult *res = NULL;
    const char *db_query_params[1] = {NULL};
    int rs_count = 0;

    strcpy(db_query, "select count(*) from gbpusd_k "
            "where date = $1::timestamp");
    db_query_params[0] = time;

    if (util_db_query(conn
            , &res
            , db_query
            , db_query_params
            , sizeof(db_query_params) / sizeof(char *)) > 0)
    {
        char *count = PQgetvalue(res, 0, 0);
        rs_count = atoi(count);
    }

    util_db_free(res);

    return rs_count;
}

static void save_data(PGconn *conn, int save_or_update
        , const char *time, const char *close
        , const char *open, const char *high
        , const char *low)
{
    char db_save[128] = {0};
    const char *db_save_params[5] = {NULL};

    if(save_or_update == 0)
    {
        strcpy(db_save, "insert into gbpusd_k values ($1, $2, $3, $4, $5)");
    }
    else
    {
        strcpy(db_save, "update gbpusd_k set close=$2, open=$3, high=$4, low=$5 where date=$1");
    }

    db_save_params[0] = time;
    db_save_params[1] = close;
    db_save_params[2] = open;
    db_save_params[3] = high;
    db_save_params[4] = low;
    printf("%s\n", db_save);

    util_db_insert(conn, db_save, db_save_params, sizeof(db_save_params) / sizeof(char *));
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

    for(int i = 0;i < CAPTURE_DAYS;i++)
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
        char time_format[19 + 1] = {0};

        util_make_time_by_offset(now_date, offset_date, i * -86400);
        strncpy(offset_year, offset_date, 4);
        strncpy(offset_month, offset_date + 4, 2);
        strncpy(offset_day, offset_date + 6, 2);
        sprintf(k_pattern, K_PATTERN_FMT, atoi(offset_year), atoi(offset_month), atoi(offset_day));

        ret = find_k(k_pattern, pchunk->memory, close, open, high, low);
        if(ret == 0)
        {
            format_time(offset_date, time_format);
            if(is_data_exist(conn, time_format) > 0)
            {
                save_data(conn, 1, time_format, close, open, high, low);
            }
            else
            {
                save_data(conn, 0, time_format, close, open, high, low);
            }
        }
    }

    util_db_disconnect(conn);
}
