CC = gcc
CFLAGS= -c -I.

ejecutable: encriptador.o main.o
	$(CC) -o ejecutable encriptador.o main.o

encriptador.o: encriptador.c
	$(CC) $(CFLAGS) encriptador.c
main.o: main.c
	$(CC) $(CFLAGS) main.c

.PHONY: clean
clean:
	rm *.o ejecutable
