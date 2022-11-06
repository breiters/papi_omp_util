#include "papi_util.h"

#define _GNU_SOURCE

#ifndef CLS
#define CLS 64
#endif

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void init(int N, double *restrict a, double *restrict b,
          double *restrict c, double *restrict d)
{
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; ++i) {
        a[i] = 0.0;
        b[i] = i;
        c[i] = i;
        d[i] = i;
    }
}

// Schoenauer-Triad Kernel
void triad(int N, int REP, double *restrict a, double *restrict b,
           double *restrict c, double *restrict d)
{
#pragma omp parallel
    for (int r = 0; r < REP; ++r) {
#pragma omp for schedule(static)
        for (int i = 0; i < N; ++i) {
            a[i] = b[i] + c[i] * d[i];
        }
        // avoid optimizing the loop with -O3
        if (a[0] > b[0])
            printf("%f%f\n", a[1], b[2]);
    }
}

// Actual Benchmark
double bench(int N)
{
    double *a = NULL, *b = NULL, *c = NULL, *d = NULL;
    size_t nbytes = N * sizeof(double);
    int REP = 1;

    a = aligned_alloc(CLS, nbytes);
    b = aligned_alloc(CLS, nbytes);
    c = aligned_alloc(CLS, nbytes);
    d = aligned_alloc(CLS, nbytes);

    if (!a || !b || !c || !d) {
        perror("aligned_alloc");
        exit(EXIT_FAILURE);
    }

    init(N, a, b, c, d);

    char region_name[256];
    snprintf(region_name, 255, "Triad N=%d", N);

    double time;
    PAPI_UTIL_START(region_name);
    do {
        time = omp_get_wtime();
        triad(N, REP, a, b, c, d);
        time = omp_get_wtime() - time;
        REP *= 2;
    } while (time < 0.5);
    PAPI_UTIL_FINISH();

    REP /= 2;

    free(a);
    free(b);
    free(c);
    free(d);

    // return average time of the kernel
    return time / REP;
}

void usage()
{
    fprintf(stderr,
            "options:\n"
            "-g specify performance group file\n"
            "-m enable multiplexing\n"
            "-t display stats on per-thread basis\n");
}

int main(int argc, char **argv)
{
    int opt;
    struct papi_util_opt options = {.event_file = NULL,
                                    .print_csv = 1,
                                    .print_threads = 0,
                                    .print_summary = 1,
                                    .print_region = 1,
                                    .component = 0,
                                    .multiplex = 0,
                                    .output = stdout};

    while ((opt = getopt(argc, argv, "mtc:g:")) != -1) {
        switch (opt) {

        case 'g':
            options.event_file = optarg;
            break;

        case 't':
            options.print_threads = 1;
            break;

        case 'm':
            options.multiplex = 1;
            break;

        default:
            usage();
            exit(EXIT_FAILURE);
            break;
        }
    }

    PAPI_UTIL_SETUP(&options);

    double time, gflop, throughput;

    for (int n = 1 << 10; n <= 1 << 25; n <<= 1) {
        time = bench(n);

        gflop = 2.0 * n * 1.0e-9;
        double flops = gflop / time;

        // 3 read + 1 write (+1 write allocate if no streaming stores)
        throughput = 5.0 * n * sizeof(double) * 1.0e-9 / time;
        // throughput = 4.0 * n * sizeof(double) * 1.0e-9 / time;

        printf("Software Caclulation N=%d:\n"
               "\t%.3f GFLOP/s\n"
               "\t%.3f GB/s\n",
               n, flops, throughput);
    }

    PAPI_UTIL_FINALIZE();

    exit(EXIT_SUCCESS);
}
