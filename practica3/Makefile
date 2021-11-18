.PHONY : all clean

all: exe

clean:
	@echo "Limpiando proyecto..."
	@rm -f exe main.o y.tab.* lex.yy.c

lex.yy.c: p3.l
	flex p3.l

y.tab.c: p3.y lex.yy.c
	yacc -d -v -t p3.y

y.tab.o: y.tab.c
	gcc -c y.tab.c

main.o: main.c
	gcc -c main.c

exe: main.o y.tab.o
	gcc -o exe main.o y.tab.o
