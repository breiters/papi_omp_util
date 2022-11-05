SHELL := /bin/bash

TARGET:=libpapiutil.so
SOURCES:=$(wildcard ./src/*.c)
HEADERS:=$(wildcard include/*.h)
OBJECTS:=$(SOURCES:.c=.o)

INC_DIRS:=include

# CFLAGS ?= -std=c99 -fPIC -Wall -Wextra -march=native -ggdb3 -fopenmp
# CFLAGS ?= -fPIC -fopenmp
CFLAGS ?= -fPIC -fopenmp
CFLAGS += -O2 -DNDEBUG

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INC_DIRS) -c $^ -o $@

$(TARGET): $(OBJECTS)
	$(CC) $< -o $@ -shared $(LDFLAGS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJECTS)
