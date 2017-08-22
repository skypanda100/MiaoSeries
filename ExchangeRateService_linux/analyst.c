//
// Created by root on 4/7/17.
//
#include "analyst.h"

static const char *TIME_PATTERN = ".*\"updatedAt\": \"([0-9]{14})\",.*";
static const char *RATE_PATTERN = ".*\"cd\": \"%s\",\\s+\"ccycd\": \"[0-9]+\",\\s+\"b\": \"([0-9]+\\.[0-9]+)\",.*";

static int find_time(const char *content, char *time)
{
    int ret = 0;
    char *time_array[1];

    time_array[0] = time;
    ret = util_find_string_by_regex(TIME_PATTERN, content, time_array, sizeof(time_array) / sizeof(char *));

    return ret;
}

static int find_rate(const char *content, char **rate, int rate_count)
{
    int ret = 0;
    char *rate_array[1];

    for(int i = 0;i < rate_count;i++)
    {
        rate_array[0] = rate[i];
        char rate_pattern[256];

        memset(rate_pattern, 0, sizeof(rate_pattern) / sizeof(char));
        sprintf(rate_pattern, RATE_PATTERN, OPERATION[i * 2 + 1]);

        ret = util_find_string_by_regex(rate_pattern, content, rate_array, sizeof(rate_array) / sizeof(char *));
        if(ret != 0)
        {
            break;
        }
    }

    return ret;
}

static void make_offset_time(const char *src_time, char *dst_time)
{
    char minute_c[2 + 1];
    int minute = 0;
    int minute_1 = 0;
    int minute_2 = 0;
    int offset_minute = 0;

    strncpy(minute_c, src_time + 10, 2);
    minute_c[2] = 0;
    minute = atoi(minute_c);
    minute_1 = minute / 5;
    minute_2 = minute % 5;
    if(minute_2 >= 0 && minute_2 < 3){
        minute_2 = 0;
    }else{
        minute_2 = 5;
    }
    offset_minute = minute_1 * 5 + minute_2 - minute;
    util_make_time_by_offset(src_time, dst_time, offset_minute * 60 - 3600);
}

static void format_offset_time(const char *src_time, char *dst_time)
{
    strncpy(dst_time, src_time, 4);
    strcat(dst_time, "-");
    strncat(dst_time, src_time + 4, 2);
    strcat(dst_time, "-");
    strncat(dst_time, src_time + 6, 2);
    strcat(dst_time, " ");
    strncat(dst_time, src_time + 8, 2);
    strcat(dst_time, ":");
    strncat(dst_time, src_time + 10, 2);
    strcat(dst_time, ":");
    strcat(dst_time, "00");
}

static double calculate_from_db(PGconn *conn, const char *table, const char *timet, int interval, int count)
{
    char db_query[512];
    char interval_c[10];
    char count_c[10];
    PGresult *res = NULL;
    const char *db_query_params[3];
    int db_query_params_count = 3;
    double money = 0.0;
    int rs_count = 0;

    memset(db_query, 0, sizeof(db_query) / sizeof(char));
    memset(interval_c, 0, sizeof(interval_c) / sizeof(char));
    memset(count_c, 0, sizeof(count_c) / sizeof(char));

    sprintf(interval_c, "%d", interval);
    sprintf(count_c, "%d", count);

    switch(interval){
        case 5:
        case 15:
        case 30:
        {
            sprintf(db_query, "select ma1 from %s "
                    "where timet <= $1::timestamp and mod(cast(extract(Minutes from timet) as int), $2::int4) = 0 "
                    "order by timet desc "
                    "limit $3::int4", table);
            db_query_params[0] = timet;
            db_query_params[1] = interval_c;
            db_query_params[2] = count_c;
            db_query_params_count = 3;
        }
            break;
        case 60:
        {
            sprintf(db_query, "select ma1 from %s "
                    "where timet <= $1::timestamp and extract(Minutes from timet) = 55 "
                    "order by timet desc "
                    "limit $2::int4", table);
            db_query_params[0] = timet;
            db_query_params[1] = count_c;
            db_query_params_count = 2;
        }
            break;
        case 240:
        {
            sprintf(db_query, "select ma1 from %s "
                    "where timet <= $1::timestamp "
                    "and mod(cast(extract(Hours from timet) as int) + 1 - 6, 4) = 0 "
                    "and extract(Minutes from timet) = 55 "
                    "order by timet desc "
                    "limit $2::int4", table);
            db_query_params[0] = timet;
            db_query_params[1] = count_c;
            db_query_params_count = 2;
        }
            break;
        case 1440:
        {
            sprintf(db_query, "select ma1 from %s "
                    "where timet <= $1::timestamp "
                    "and extract(Hours from timet) = 5 "
                    "and extract(Minutes from timet) = 55 "
                    "order by timet desc "
                    "limit $2::int4", table);
            db_query_params[0] = timet;
            db_query_params[1] = count_c;
            db_query_params_count = 2;
        }
            break;
        case 10080:
        {
            sprintf(db_query, "select ma1 from %s "
                    "where timet <= $1::timestamp "
                    "and extract(DOW from timet) = 6 "
                    "and extract(Hours from timet) = 5 "
                    "and extract(Minutes from timet) = 55 "
                    "order by timet desc "
                    "limit $2::int4", table);
            db_query_params[0] = timet;
            db_query_params[1] = count_c;
            db_query_params_count = 2;
        }
            break;
    }

    rs_count = util_db_query(conn
            , &res
            , db_query
            , db_query_params
            , db_query_params_count);

    for(int i = 0;i < rs_count;i++)
    {
        char *ma1_c = PQgetvalue(res, i, 0);
        money += atof(ma1_c);
    }

    if(count == rs_count){
        money = money / count;
    }else{
        money = 0.0;
    }

    util_db_free(res);

    return money;
}

static void save_to_db_by_no_interval(PGconn *conn, const char *table, const char *ma1, const char *timet)
{
    char db_insert[128];
    const char *db_insert_params[2];

    memset(db_insert, 0, sizeof(db_insert) / sizeof(char));
    sprintf(db_insert, "insert into %s(ma1, timet) values ($1, $2)", table);
    db_insert_params[0] = ma1;
    db_insert_params[1] = timet;
    util_db_insert(conn, db_insert, db_insert_params, sizeof(db_insert_params) / sizeof(char *));
}

static void save_to_db_by_interval(PGconn *conn, const char *table, const char *timet, int interval)
{
    char db_insert[128];
    const char *db_insert_params[5];
    char ma14_c[10];
    char ma60_c[10];
    char ma99_c[10];
    char ma888_c[10];

    memset(db_insert, 0, sizeof(db_insert) / sizeof(char));
    memset(ma14_c, 0, sizeof(ma14_c) / sizeof(char));
    memset(ma60_c, 0, sizeof(ma60_c) / sizeof(char));
    memset(ma99_c, 0, sizeof(ma99_c) / sizeof(char));
    memset(ma888_c, 0, sizeof(ma888_c) / sizeof(char));

    sprintf(db_insert, "insert into %s_%d(ma14, ma60, ma99, ma888, timet) values ($1, $2, $3, $4, $5)", table, interval);
    double ma14 = calculate_from_db(conn, table, timet, interval, 14);
    double ma60 = calculate_from_db(conn, table, timet, interval, 60);
    double ma99 = calculate_from_db(conn, table, timet, interval, 99);
    double ma888 = calculate_from_db(conn, table, timet, interval, 888);
    sprintf(ma14_c, "%f", ma14);
    sprintf(ma60_c, "%f", ma60);
    sprintf(ma99_c, "%f", ma99);
    sprintf(ma888_c, "%f", ma888);
    db_insert_params[0] = ma14_c;
    db_insert_params[1] = ma60_c;
    db_insert_params[2] = ma99_c;
    db_insert_params[3] = ma888_c;
    db_insert_params[4] = timet;

    util_db_insert(conn, db_insert, db_insert_params, sizeof(db_insert_params) / sizeof(char *));
}

static void save_to_db(const char *time, char *const *rate_array, int rate_count)
{
    PGconn *conn = NULL;
    char offset_time[14 + 1];
    char offset_time_fmt[19 + 1];
    struct tm *tm = NULL;
    int minute = 0;
    int hour = 0;
    int week_day = 0;

    memset(offset_time, 0, sizeof(offset_time) / sizeof(char));
    memset(offset_time_fmt, 0, sizeof(offset_time_fmt) / sizeof(char));

    if(util_db_connect(&conn) != 0)
    {
        util_db_disconnect(conn);
        return;
    }

    make_offset_time(time, offset_time);
    format_offset_time(offset_time, offset_time_fmt);
    tm = util_make_time(offset_time);
    minute = tm->tm_min;
    hour = tm->tm_hour;
    week_day = tm->tm_wday;

    for(int i = 0;i < rate_count;i++)
    {
        //is data exist
        char db_exist[128];
        const char *db_exist_params[1];
        PGresult *res = NULL;

        memset(db_exist, 0, sizeof(db_exist) / sizeof(char));
        sprintf(db_exist
                , "select * from %s where timet = $1::timestamp"
                , OPERATION[i * 2]);
        db_exist_params[0] = offset_time_fmt;

        if(util_db_query(conn
                , &res
                , db_exist
                , db_exist_params
                , sizeof(db_exist_params) / sizeof(char *)) > 0)
        {
            util_db_free(res);
            continue;
        }
        util_db_free(res);

        //insert data
        save_to_db_by_no_interval(conn, OPERATION[i * 2], rate_array[i], offset_time_fmt);

        if(minute % 5 == 0)
        {
            save_to_db_by_interval(conn, OPERATION[i * 2], offset_time_fmt, 5);
        }

        if(minute % 15 == 0)
        {
            save_to_db_by_interval(conn, OPERATION[i * 2], offset_time_fmt, 15);
        }

        if(minute % 30 == 0)
        {
            save_to_db_by_interval(conn, OPERATION[i * 2], offset_time_fmt, 30);
        }

        if(minute == 55)
        {
            save_to_db_by_interval(conn, OPERATION[i * 2], offset_time_fmt, 60);
        }

        if((hour + 1 - 6) % 4 == 0 && minute == 55)
        {
            save_to_db_by_interval(conn, OPERATION[i * 2], offset_time_fmt, 240);
        }

        if(hour == 5 && minute == 55)
        {
            save_to_db_by_interval(conn, OPERATION[i * 2], offset_time_fmt, 1440);
        }

        if(week_day == 6 && hour == 5 && minute == 55)
        {
            save_to_db_by_interval(conn, OPERATION[i * 2], offset_time_fmt, 10080);
        }

        printf("%d\t%s\t%s\n", week_day, OPERATION[i * 2], rate_array[i]);
    }

    util_db_disconnect(conn);
}

int analyst_execute(const struct Chunk *pchunk)
{
    int ret = 0;
    char time[14 + 1];

    memset(time, 0, sizeof(time) / sizeof(char));

    ret = find_time(pchunk->memory, time);
    if(ret == 0)
    {
        int op_count = sizeof(OPERATION) / sizeof(char *) / 2;
        char *rate_array[op_count];

        for(int i = 0;i < op_count;i++)
        {
            char *rate = (char *)malloc(20 * sizeof(char));
            memset(rate, 0, 20 * sizeof(char));
            rate_array[i] = rate;
        }

        ret = find_rate(pchunk->memory, rate_array, op_count);
        if(ret == 0)
        {
            save_to_db(time, rate_array, op_count);
        }

        for(int i = 0;i < op_count;i++)
        {
            free(rate_array[i]);
            rate_array[i] = NULL;
        }
    }

    return ret;
}

