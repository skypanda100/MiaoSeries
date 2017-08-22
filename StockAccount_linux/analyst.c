//
// Created by zhengdongtian on 17-5-20.
//

#include "analyst.h"

static const char *TIME_PATTERN = ".*<h2 class=\"fl\">一周投资者情况统计表（([0-9]{4}\\.[0-9]{2}\\.[0-9]{2})-([0-9]{4}\\.[0-9]{2}\\.[0-9]{2})）</h2>.*";
static const char *ACCOUNT_PATTERN = ".*新增投资者数量.*</td>[    ]*<td.*>.*<span.*>([0-9\\.\\,]*)</span>.*</td>"
        ".*自然人.*</td>[    ]*<td.*>.*<span.*>([0-9\\.\\,]*)</span>.*</td>"
        ".*非自然人.*</td>[    ]*<td.*>.*<span.*>([0-9\\.\\,]*)</span>.*</td>"
        ".*期末投资者数量.*</td>[    ]*<td.*>.*<span.*>([0-9\\.\\,]*)</span>.*</td>"
        ".*、[   ]*自然人";

static int find_time(const char *content, char *f_time, char *t_time)
{
    int ret = 0;
    char *time_array[2];

    time_array[0] = f_time;
    time_array[1] = t_time;
    ret = util_find_string_by_regex(TIME_PATTERN, content, time_array, sizeof(time_array) / sizeof(char *));

    return ret;
}

static int find_account(const char *content, char *new_account, char *natural_account, char *none_natural_account, char *final_account)
{
    int ret = 0;
    char *account_array[4];

    account_array[0] = new_account;
    account_array[1] = natural_account;
    account_array[2] = none_natural_account;
    account_array[3] = final_account;
    ret = util_find_string_by_regex(ACCOUNT_PATTERN, content, account_array, sizeof(account_array) / sizeof(char *));

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

static int is_data_exist(PGconn *conn, char *f_time, char *t_time)
{
    char db_query[512] = {0};
    PGresult *res = NULL;
    const char *db_query_params[2] = {NULL};
    int rs_count = 0;

    strcpy(db_query, "select count(*) from stock_account "
            "where f_time = $1::timestamp and t_time = $2::timestamp");
    db_query_params[0] = f_time;
    db_query_params[1] = t_time;

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

int analyst_execute(const struct Chunk *pchunk)
{
    PGconn *conn = NULL;
    int ret = 0;
    char f_time[10 + 1] = {0};
    char f_time_fmt[19 + 1] = {0};
    char t_time[10 + 1] = {0};
    char t_time_fmt[19 + 1] = {0};
    char new_account[12] = {0};
    char natural_account[12] = {0};
    char none_natural_account[12] = {0};
    char final_account[12] = {0};

    if(util_db_connect(&conn) != 0)
    {
        util_db_disconnect(conn);
        return 1;
    }

    ret = find_time(pchunk->memory, f_time, t_time);
    if(ret == 0)
    {
        format_time(f_time, f_time_fmt);
        format_time(t_time, t_time_fmt);

        if(is_data_exist(conn, f_time_fmt, t_time_fmt) > 0)
        {
            util_db_disconnect(conn);
            return 1;
        }

        ret = find_account(pchunk->memory, new_account, natural_account, none_natural_account, final_account);
        if(ret == 0)
        {
            char temp[12] = {0};
            for(int i = 0;i < sizeof(new_account) / sizeof(char);i++)
            {
                char c = new_account[i];
                if(c != ',')
                {
                    temp[strlen(temp)] = c;
                }
            }
            memcpy(new_account, temp, 12);
            memset(temp, 0, 12);

            for(int i = 0;i < sizeof(natural_account) / sizeof(char);i++)
            {
                char c = natural_account[i];
                if(c != ',')
                {
                    temp[strlen(temp)] = c;
                }
            }
            memcpy(natural_account, temp, 12);
            memset(temp, 0, 12);

            for(int i = 0;i < sizeof(none_natural_account) / sizeof(char);i++)
            {
                char c = none_natural_account[i];
                if(c != ',')
                {
                    temp[strlen(temp)] = c;
                }
            }
            memcpy(none_natural_account, temp, 12);
            memset(temp, 0, 12);

            for(int i = 0;i < sizeof(final_account) / sizeof(char);i++)
            {
                char c = final_account[i];
                if(c != ',')
                {
                    temp[strlen(temp)] = c;
                }
            }
            memcpy(final_account, temp, 12);
            memset(temp, 0, 12);

            save_data(conn, f_time_fmt, t_time_fmt, new_account, natural_account, none_natural_account, final_account);
            ret = 0;
        }
    }
    util_db_disconnect(conn);

    return ret;
}
