/**
 * @file papi_util.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <omp.h>
#include <papi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct papi_util_opt {
    const char *event_file;
    int print_threads;
    int print_summary;
    int print_region;
    int component;
    int multiplex;
    FILE *output;
};

#if PAPI_UTIL_USEPAPI
#define PAPI_UTIL_SETUP(o) PAPI_UTIL_setup(o)
#define PAPI_UTIL_START(r) PAPI_UTIL_start(r)
#define PAPI_UTIL_FINISH(t) PAPI_UTIL_finish((t))
#define PAPI_UTIL_FINALIZE() PAPI_UTIL_finalize()
#else
#define PAPI_UTIL_SETUP(o)
#define PAPI_UTIL_START(r)
#define PAPI_UTIL_FINISH(t)
#define PAPI_UTIL_FINALIZE()
#endif /* PAPI_UTIL_USEPAPI */

/**
 *
 */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
extern void PAPI_UTIL_setup(struct papi_util_opt *opt);
extern void PAPI_UTIL_start(char *region_name);
extern void PAPI_UTIL_finish(double time);
extern void PAPI_UTIL_finalize(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */
