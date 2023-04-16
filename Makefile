CC=gcc
CFLAGS=-g -static -std=c11 -Wall -pedantic -I. -lm
LDFLAGS=-L/usr/lib
OBJS=spracker.o tga.o spr.o

all: spracker

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

spracker: $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

.PHONY: clean

clean:
	$(RM) *.o spracker spracker.com
