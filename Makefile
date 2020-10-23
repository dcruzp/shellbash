CC = gcc
CFLAGS =  -I.

mishell.o: mishell.c includes.c
	$(CC) includes.c mishell.c -o mishell.o $(CFLAGS)


