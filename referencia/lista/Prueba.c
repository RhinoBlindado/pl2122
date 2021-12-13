#include <stdio.h>
#include "int_list.h"

void imprimir(int_list MEME)
{
	int i,j;
	j = longitud(&MEME);
	for (i=1;i<=j;i++)
	{
		printf("%d ",consultarEnPosicion(MEME,i));
	}
	printf("\n");
}

void pruebaPuntero()
{
	printf("Prueba puntero\n");

	int_list x;
	inicializar(&x);

	for(int i=1;i<=5;i++)
	{
		insertar(&x,i,i + (i-1));
	}	

	imprimir(x);
	avanzarPuntero(&x);
	avanzarPuntero(&x);

	punteroAInicio(&x);

	printf("Consultado: %d\n", consultar(x));	

}

void pruebaCopia()
{
	printf("Prueba copia\n");

	int_list x, y;
	inicializar(&x);
	inicializar(&y);


	for(int i=0;i<10;i++)
	{
		insertar(&x, i + 1, i+2 + (2*i+1));
	}	
	printf("Original: \n");
	imprimir(x);


	printf("Copia, añadiendo elemento: \n");
	insertarCopia(&x, &y, 3, 99);
	imprimir(y);

	printf("Original modificado: \n");
	limpiar(&x);
	insertar(&x, 1, 1);
	imprimir(x);

	printf("Copia: \n");

	imprimir(y);

	int_list z;
	inicializar(&z);

	eliminarCopiaDesdePosicion(&y, &z, 3);
	printf("Copia: %d\n", sumarValor(&z, 1));

	imprimir(z);
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
