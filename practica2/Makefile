.PHONY : all clean

all: exe p2.c p2.l
	@echo "Ejecutando prueba:"
	@./exe < prueba.c+o-

clean:
	@echo "Limpiando proyecto..."
	@rm -rf exe *.c

p2.c: p2.l
	@echo "Creando fichero c..."
	@lex -o p2.c p2.l

exe: p2.c
	@echo "Creando ejecutable..."
	@gcc -o exe p2.c
