SHELL := /bin/bash

TARGET:=papiutil.o
SOURCES:=$(wildcard ./src/*.c)
HEADERS:=$(wildcard include/*.h)
OBJECTS:=$(SOURCES:.c=.o)

INC_DIRS:=include

CFLAGS ?= -fopenmp
CFLAGS += -O2

all: $(OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -I$(INC_DIRS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OBJECTS) #$(TARGET)
