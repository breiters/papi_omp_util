#include "../src/papi_util.h"

#define _GNU_SOURCE

#ifndef CLS
#define CLS 64
#endif

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Schoenauer-Triad Kernel
void triad(int N, double *restrict a, double *restrict b,
           double *restrict c, double *restrict d)
{
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; ++i) {
        a[i] = b[i] + c[i] * d[i];
    }
    // avoid optimizing the loop with -O3
    if (a[0] > b[0])
        printf("%f%f\n", a[1], b[2]);
}

// Actual Benchmark
double bench(int N)
{
    double time;
    double *a = NULL, *b = NULL, *c = NULL, *d = NULL;
    size_t nbytes = N * sizeof(double);
    int REP = 0;

    a = aligned_alloc(CLS, nbytes);
    b = aligned_alloc(CLS, nbytes);
    c = aligned_alloc(CLS, nbytes);
    d = aligned_alloc(CLS, nbytes);

    if (!a || !b || !c || !d) {
        perror("aligned_alloc");
        exit(EXIT_FAILURE);
    }

    char region_name[256];
    snprintf(region_name, 255, "Triad N=%d", N);

    PAPI_UTIL_START(region_name);
    double t_start = omp_get_wtime();
    do {
        triad(N, a, b, c, d);
        time = omp_get_wtime() - t_start;
        REP++;
    } while (time < 1.0);
    PAPI_UTIL_FINISH(time);

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
