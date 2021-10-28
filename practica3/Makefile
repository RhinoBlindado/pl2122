.PHONY : all clean

all: exe p3.c p3.l
	@echo "Ejecutando prueba:"
	@./exe < prueba.c+o-

clean:
	@echo "Limpiando proyecto..."
	@rm -rf exe *.c

p3.c: p3.l
	@echo "Creando fichero c..."
	@lex -o p3.c p3.l

exe: p3.c
	@echo "Creando ejecutable..."
	@gcc -o exe p3.c
