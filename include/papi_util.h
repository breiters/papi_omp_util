/**
 * @file papi_util.h
 * @author Sergej Breiter (sergej.breiter@gmx.de)
 * @brief
 * @version 0.1
 * @date 2022-11-05
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

struct _IO_FILE;
typedef struct _IO_FILE FILE;

struct papi_util_opt {
    const char *event_file;
    int print_csv;
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
#define PAPI_UTIL_FINISH() PAPI_UTIL_finish()
#define PAPI_UTIL_FINALIZE() PAPI_UTIL_finalize()
#else
#define PAPI_UTIL_SETUP(o)
#define PAPI_UTIL_START(r)
#define PAPI_UTIL_FINISH()
#define PAPI_UTIL_FINALIZE()
#endif /* PAPI_UTIL_USEPAPI */

/**
 *
 */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
extern void PAPI_UTIL_setup(const struct papi_util_opt *opt);
extern void PAPI_UTIL_start(const char *region_name);
extern void PAPI_UTIL_finish(void);
extern void PAPI_UTIL_finalize(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */
