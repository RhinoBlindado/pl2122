# Makefile de Punto2D

# Variables
CC = gcc
EJECUTABLE = Prueba

# Programa Principal
all: Lista.o
	$(CC) Lista.o Prueba.c -o $(EJECUTABLE) -g
# Librerías

Lista.o: Lista.h Lista.c
	$(CC) -c Lista.c

# Borrar los Archivos Objeto y el Ejecutable
clean:
	rm -rf *.o $(EJECUTABLE)
