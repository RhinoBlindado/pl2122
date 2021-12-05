.PHONY : all clean

all: exe

clean:
	@echo "Limpiando proyecto..."
	@rm -f g+o- *.o y.tab.* lex.yy.c

semantic.o: semantic.c
	gcc -c semantic.c

codegen.o: codegen.c
	gcc -c codegen.c

lex.yy.c: p5.l
	flex p5.l

y.tab.c: p5.y lex.yy.c codegen.o semantic.o
	yacc -d -t p5.y

y.tab.o: y.tab.c
	gcc -c y.tab.c

main.o: main.c
	gcc -c main.c

exe: main.o y.tab.o
	gcc -o g+o- main.o y.tab.o
