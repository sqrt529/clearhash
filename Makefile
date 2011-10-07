CC = gcc
CCFLAGS = -std=gnu99 -lcrypt

all: clearhash.c
	$(CC) $(CCFLAGS) -o clearhash clearhash.c
