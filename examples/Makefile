SHELL := /bin/bash

SRC := triad.c
OBJ := $(patsubst %.c, %.o, $(SRC))
BIN := triad

CC ?= gcc

# get systems cacheline size:
CLS := $(shell getconf LEVEL1_DCACHE_LINESIZE)

# defines
USEPAPI := 1

DEFS := -DCLS=$(CLS) -DPAPI_UTIL_USEPAPI=$(USEPAPI)

CFLAGS ?= -Ofast -march=native -std=gnu11 -Wall -Wextra -fopenmp

# linking
LDFLAGS = -lm -lpthread -lpapi
ifneq (,$(PAPI_LIBDIR))
LDFLAGS += -L$(PAPI_LIBDIR)
endif

# include
INCL ?= -I../include
ifneq (,$(PAPI_INC))
INCL += -I$(PAPI_INC)
endif

.DEFAULT: all
all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) \
	$(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ \
	$(INCL) $(DEFS)

remake: clean all

.PHONY: clean
clean:
	$(RM) $(BIN) $(OBJ)
