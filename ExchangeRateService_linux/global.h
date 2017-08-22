//
// Created by root on 4/7/17.
//

#ifndef EXCHANGERATESERVICE_GLOBAL_H
#define EXCHANGERATESERVICE_GLOBAL_H

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
static const char *DB_HOST = "192.168.1.3";
static const char *DB_PORT = "15432";
static const char *DB_NAME = "postgres";
static const char *DB_UID = "postgres";
static const char *DB_PWD = "123456";

#endif //EXCHANGERATESERVICE_GLOBAL_H
