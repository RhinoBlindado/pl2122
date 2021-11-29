.SUFFIXES:

prueba: main.o y.tab.o
	gcc -o prueba main.o y.tab.o

y.tab.o: y.tab.c
	gcc -c y.tab.c

main.o: main.c
	gcc -c main.c

y.tab.c: prueba.y lex.yy.c
	bison -o y.tab.c prueba.y

lex.yy.c: prueba.l
	flex prueba.l

limpia:
	rm -f prueba main.o y.tab.o y.tab.c lex.yy.c

# NO ES VERDAD QUE LA OFICINA DE SOFTWARE HA HECHO ESTA MIERDA
todo:
	make limpia
	make prueba
