//
// Created by zhengdongtian on 17-5-20.
//
#include "util.h"

int util_find_string_by_regex(const char *pattern, const char *content, char **result, int match_count)
{
    match_count += 1;
    int ret = 0;
    regex_t reg;
    regmatch_t pmatch[match_count];
    ret = regcomp(&reg, pattern, REG_EXTENDED);
    if (ret == 0)
    {
        ret = regexec(&reg, content, match_count, pmatch, 0);
        if(ret == 0)
        {
            for(int i = 1;i < match_count && pmatch[i].rm_so != -1;i++)
            {
                int match_len = pmatch[i].rm_eo - pmatch[i].rm_so;
                strncpy(result[i - 1], content + pmatch[i].rm_so, match_len);
            }
        }
        else if(ret == REG_NOMATCH)
        {
            printf("No match\n");
            ret = -1;
        }
        else
        {
            char err[100];
            regerror(ret,&reg,err,100);
            printf("error:%s\n",err);
            ret = -1;
        }

    }
    else
    {
        printf("regcomp error\n");
        ret = -1;
    }

    regfree(&reg);

    return ret;
}

void util_make_time_by_offset(const char *src_time, char *dst_time, size_t offset_sec)
{
    struct tm *src_t = NULL;
    time_t src_timet = 0L;
    struct tm *dst_t = NULL;
    time_t dst_timet = 0L;

    src_timet = time((time_t *)NULL);
    src_t = localtime(&src_timet);

    char year[5];
    strncpy(year, src_time, 4);
    year[4] = 0;
    src_t->tm_year = atoi(year) - 1900;

    char month[3];
    strncpy(month, src_time + 4, 2);
    month[2] = 0;
    src_t->tm_mon = atoi(month) - 1;

    char day[3];
    strncpy(day, src_time + 6, 2);
    day[2] = 0;
    src_t->tm_mday = atoi(day);

    char hour[3];
    strncpy(hour, src_time + 8, 2);
    hour[2] = 0;
    src_t->tm_hour = atoi(hour);

    char minute[3];
    strncpy(minute, src_time + 10, 2);
    minute[2] = 0;
    src_t->tm_min = atoi(minute);

    char second[3];
    strncpy(second, src_time + 12, 2);
    second[2] = 0;
    src_t->tm_sec = atoi(second);

    src_timet = mktime(src_t);
    dst_timet = src_timet + offset_sec;
    dst_t = localtime(&dst_timet);
    sprintf(dst_time, "%d%02d%02d%02d%02d%02d"
            , dst_t->tm_year + 1900
            , dst_t->tm_mon + 1
            , dst_t->tm_mday
            , dst_t->tm_hour
            , dst_t->tm_min
            , dst_t->tm_sec);
}

struct tm * util_make_time(const char *src_time)
{
    struct tm *src_t = NULL;
    time_t src_timet = 0L;

    src_timet = time((time_t *)NULL);
    src_t = localtime(&src_timet);

    char year[5];
    strncpy(year, src_time, 4);
    src_t->tm_year = atoi(year) - 1900;

    char month[3];
    strncpy(month, src_time + 4, 2);
    src_t->tm_mon = atoi(month) - 1;

    char day[3];
    strncpy(day, src_time + 6, 2);
    src_t->tm_mday = atoi(day);

    char hour[3];
    strncpy(hour, src_time + 8, 2);
    src_t->tm_hour = atoi(hour);

    char minute[3];
    strncpy(minute, src_time + 10, 2);
    src_t->tm_min = atoi(minute);

    char second[3];
    strncpy(second, src_time + 12, 2);
    src_t->tm_sec = atoi(second);

    return src_t;
}

int util_db_connect(PGconn **conn)
{
    int ret = 0;

    *conn = PQsetdbLogin(DB_HOST
            , DB_PORT
            , NULL
            , NULL
            , DB_NAME
            , DB_UID
            , DB_PWD);
    if (PQstatus(*conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s",
                PQerrorMessage(*conn));
        PQfinish(*conn);
        ret = -1;
    }

    return ret;
}

void util_db_disconnect(PGconn *conn)
{
    if(conn != NULL)
    {
        PQfinish(conn);
    }
}

void util_db_free(PGresult *res)
{
    if(res != NULL)
    {
        PQclear(res);
    }
}

int util_db_query(PGconn *conn, PGresult **res, const char *query, const char *const *params, int param_count)
{
    int ret = 0;

    *res = PQexecParams(conn,
                        query,
                        param_count,
                        NULL,
                        params,
                        NULL,
                        NULL,
                        0);
    if (PQresultStatus(*res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        return -1;
    }

    ret = PQntuples(*res);

    return ret;
}

int util_db_insert(PGconn *conn, const char *insert, const char *const *params, int param_count)
{
    int ret = 0;
    PGresult *res;

    res = PQexecParams(conn,
                       insert,
                       param_count,
                       NULL,
                       params,
                       NULL,
                       NULL,
                       0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        util_db_free(res);
        return -1;
    }

    util_db_free(res);

    return ret;
}
