//
// Created by root on 5/24/17.
//

#ifndef EXCHANGERATESERVICEK_LINUX_UTIL_H
#define EXCHANGERATESERVICEK_LINUX_UTIL_H
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <regex.h>
#include <libpq-fe.h>
#include "global.h"

int util_find_string_by_regex(const char *, const char *, char **, int);

void util_make_time_by_offset(const char *, char *, size_t);

struct tm * util_make_time(const char *);

int util_db_connect(PGconn **);

void util_db_disconnect(PGconn *);

void util_db_free(PGresult *res);

int util_db_query(PGconn *, PGresult **, const char *, const char *const *, int);

int util_db_insert(PGconn *, const char *, const char *const *, int);

#endif //EXCHANGERATESERVICEK_LINUX_UTIL_H
