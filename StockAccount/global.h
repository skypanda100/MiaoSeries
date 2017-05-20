//
// Created by zhengdongtian on 17-5-20.
//

#ifndef STOCKACCOUNT_GLOBAL_H
#define STOCKACCOUNT_GLOBAL_H

#include <stdlib.h>

/**
 * html
 */
struct Chunk
{
    char *memory;
    size_t size;
};

/**
 * table and rate
 */
static const char *OPERATION[] = {"GBPUSD", "GBP/USD", "USDJPY", "USD/JPY", "CNHJPY", "CNH/JPY"};

/**
 * db info
 */
static const char *DB_HOST = "127.0.0.1";
static const char *DB_PORT = "15432";
static const char *DB_NAME = "postgres";
static const char *DB_UID = "postgres";
static const char *DB_PWD = "123456";

/**
 * target url
 */
static const char *TARGET_URL = "http://www.chinaclear.cn/cms-search/view.action?action=china&viewType=&dateStr=&channelIdStr=";

#endif //STOCKACCOUNT_GLOBAL_H
