#include <stdio.h>
#include "l_int.h"

void pruebaPuntero()
{
	printf("Prueba puntero\n");

	l_int x;
	int_inicializar(&x);

	for(int i=1;i<=5;i++)
	{
		int_insertar(&x,i,i + (i-1));
	}	

	int_imprimir(x);
	int_avanzarPuntero(&x);
	int_avanzarPuntero(&x);

	int_punteroAInicio(&x);

	printf("Consultado: %d\n", int_consultar(x));	

}

void pruebaCopia()
{
	printf("Prueba copia\n");

	l_int x, y;
	int_inicializar(&x);
	int_inicializar(&y);


	for(int i=0;i<10;i++)
	{
		int_insertar(&x, i + 1, i+2 + (2*i+1));
	}	
	printf("Original: \n");
	int_imprimir(x);


	printf("Copia, añadiendo elemento: \n");
	int_insertarCopia(&x, &y, 3, 99);
	int_imprimir(y);

	printf("Original modificado: \n");
	int_limpiar(&x);
	int_insertar(&x, 1, 1);
	int_imprimir(x);

	printf("Copia: \n");

	int_imprimir(y);

	l_int c1, c2, c3;
	int_inicializar(&c1);
	int_inicializar(&c2);
	int_inicializar(&c3);

	for (int i = 4; i > 0; i--)
	{
		int_insertar(&c1, 1, i);
		int_insertar(&c2, 1, i + 4);
	}
	
	int_imprimir(c1);
	int_imprimir(c2);

	int_concatenarCopia(&c1, &y, &c3);

	int_imprimir(c3);
}


int main()
{
/*	Lista X;
	int i;
	inicializar(&X);	
	for(i=1;i<=15;i++)
	{
		insertar(&X,i,i);
	}			
	imprimir(X);
	insertar(&X,6,19);
	imprimir(X);
	eliminar(&X,13);
	imprimir(X);
	
	printf("Esta esta mierda? %d ? En la pos: %d\n",19,esta(X,19));
	
*/
	//pruebaPuntero();
	pruebaCopia();

}
