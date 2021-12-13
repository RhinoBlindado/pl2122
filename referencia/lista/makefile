# Makefile de Punto2D

# Variables
CC = gcc
EJECUTABLE = Prueba

# Programa Principal
all: int_list.o
	$(CC) int_list.o Prueba.c -o $(EJECUTABLE) -g
# Librerías

int_list.o: int_list.h int_list.c
	$(CC) -c int_list.c

# Borrar los Archivos Objeto y el Ejecutable
clean:
	rm -rf *.o $(EJECUTABLE)
