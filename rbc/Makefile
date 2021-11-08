SOURCE = RBC.c
CC = gcc
FLAGS=-std=gnu11 -Wall
LFLAGS= -lpthread -lm -lrt

all: RBC


RBC:
	$(CC) RBC.c controller/*.c -o RBC.exe $(LFLAGS) $(FLAGS)

test: RBC
	./RBC.exe $(RBC_PORT)
