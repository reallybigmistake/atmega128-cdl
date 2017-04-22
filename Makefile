CC=gcc
objects = main.o lib.o
VPATH=cdl
main: $(objects)
	$(CC) -o main $(objects)
main.o: lib.h
lib.o: lib.c
.PHONY: clean
clean:
	-rm $(objects)