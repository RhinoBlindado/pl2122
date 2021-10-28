.PHONY : all clean

exe: main.o y.tab.o
	gcc -o exe main.o y.tab.o

main.o: main.c
	gcc -c main.c

y.tab.o: y.tab.c
	gcc -c y.tab.c

y.tab.c: p3.l lex.yy.c
	yacc -o y.tab.c p3.y -d

lex.yy.c: p3.l
	flex p3.l




#all: exe p3.c p3.l
#	@echo "Ejecutando prueba:"
#	@./exe < prueba.c+o-
#
clean:
	@echo "Limpiando proyecto..."
	@rm -f exe main.o y.tab.* lex.yy.c

#p3.c: p3.l
#	@echo "Creando fichero c..."
#	@lex -o p3.c p3.l
#
#exe: p3.c
#	@echo "Creando ejecutable..."
#	@gcc -o exe p3.c
