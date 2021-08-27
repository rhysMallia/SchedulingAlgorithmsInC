CC=gcc
CFLAGS= -g -Wall 

hellomake: hellomake.o hellofunc.o 
	$(CC) -o hellomake hellomake.o hellofunc.o
