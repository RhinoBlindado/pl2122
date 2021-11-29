.PHONY : all clean

all: exe

clean:
	@echo "Limpiando proyecto..."
	@rm -f exe y.tab.* lex.yy.c

lex.yy.c: main.l
	flex main.l

y.tab.c: main.y lex.yy.c
	yacc -d -t main.y

y.tab.o: y.tab.c
	gcc -c y.tab.c

exe: y.tab.o
	gcc -o exe y.tab.o
