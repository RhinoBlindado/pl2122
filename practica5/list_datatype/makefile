# Makefile de Punto2D

# Variables
CC = gcc
EJECUTABLE = Prueba

# Programa Principal
all: l_int.o
	$(CC) l_int.o Prueba.c -o $(EJECUTABLE) -g
# Librerías

l_int.o: l_int.h l_int.c
	$(CC) -c l_int.c

l_char.o: l_char.h l_char.c
	$(CC) -c l_char.c

l_double.o: l_double.h l_double.c
	$(CC) -c l_double.c


# Borrar los Archivos Objeto y el Ejecutable
clean:
	rm -rf *.o $(EJECUTABLE)
