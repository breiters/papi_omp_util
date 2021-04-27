# PAPI OpenMP Utility

## Description

This is a C-based utility for easy and fast microbenchmarking of OpenMP Kernels with PAPI.

You can specify performance events that should be measured and derived metrics that should be calculated from those events. The events as well as the formulas to calculate the derived metrics will be read from a file.

## Basic Usage

```c
#define PAPI_UTIL_USEPAPI

struct papi_util_opt opt = {.event_file = "event_file_name",
                            .print_threads = 0,
                            .print_summary = 1,
                            .print_region = 1,
                            .component = 0,
                            .multiplex = 0,
                            .output = stdout};

PAPI_UTIL_SETUP(&opt);

PAPI_UTIL_START("region name");
double time = omp_get_wtime();
/* some code that should be measured */
time = omp_get_wtime() - time;
PAPI_UTIL_STOP(time);

PAPI_UTIL_FINISH();
```

## Performance Event File

### Basic Layout of Performance Event File

```
[list of performance events]

formulas:

[list of formulas (derived metrics)]
```

### Example for Performance Event File

```
# this is a comment
PAPI_DP_OPS
PAPI_VEC_DP

formulas:

# metric name [unit] = expression

DP Flops [GFLOP/s] = 1.0e-9 * (PAPI_DP_OPS/time)
Vector Ratio   [%] = (PAPI_VEC_DP/PAPI_DP_OPS) * 100
```

### Grammar for Derived Metrics
```
 expression = (expression)
 expression = (expression binop expression)
 expression = literal
 binop = { + | - | * | / }
 literal = eventname | float | time
```

### Example

See example for [Schönauer-Triad](./examples/triad.c).

## Limitations

## Bugs