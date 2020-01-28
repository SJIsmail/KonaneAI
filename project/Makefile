# Author: Jonathan Ismail, Declan Simkins, Rebecca Walton
# Project: Konane (Project 2)
# Course: CMPT 355
# Date: Mar. 25, 19
# File: Makefile
# Description: Makefile
CC=gcc
CFLAGS=-g -Wall -std=c99 -pedantic -D_POSIX_C_SOURCE=200809
LDFLAGS=
LDLIBS=-lm -lpthread

.PHONY: all clean

all: konane.exe

konane.exe: konane.o model.o parse.o node.o moves.o minimax.o hashtable.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

konane.o: konane.c
	$(CC) $(CFLAGS) -c -o $@ $<

model.o: model.c model.h
	$(CC) $(CFLAGS) -c -o $@ $<

parse.o: parse.c parse.h
	$(CC) $(CFLAGS) -c -o $@ $<

node.o: node.c node.h
	$(CC) $(CFLAGS) -c -o $@ $<

moves.o: moves.c moves.h
	$(CC) $(CFLAGS) -c -o $@ $<

minimax.o: minimax.c minimax.h
	$(CC) $(CFLAGS) -c -o $@ $<

hashtable.o: hashtable.c hashtable.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) konane.exe *.o *~
