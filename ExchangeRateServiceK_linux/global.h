//
// Created by root on 5/24/17.
//

#ifndef EXCHANGERATESERVICEK_LINUX_GLOBAL_H
#define EXCHANGERATESERVICEK_LINUX_GLOBAL_H

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
static const char *OPERATION[] = {"GBPUSD_K"};

/**
 * db info
 */
static const char *DB_HOST = "47.94.165.17";
static const char *DB_PORT = "5432";
static const char *DB_NAME = "postgres";
static const char *DB_UID = "postgres";
static const char *DB_PWD = "123456";

/**
 * capture days
 */
static const int CAPTURE_DAYS = 20;

#endif //EXCHANGERATESERVICEK_LINUX_GLOBAL_H
