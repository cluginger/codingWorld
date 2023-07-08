PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CC := gcc
CFLAGS := -Werror -Wall
LDFLAGS := -lrt -lpthread -g
OBJS  := client server

all: client server

%: %.c
	$(CC) $(CFLAGS) $@.c util.c -o $@ $(LDFLAGS)
run:
	./client

clean:
	rm -fr ASS5 $(OBJS)
	
