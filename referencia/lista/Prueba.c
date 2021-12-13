#include <stdio.h>
#include "int_list.h"

void imprimir(int_list MEME)
{
	int i,j;
	j = int_longitud(&MEME);
	for (i=1;i<=j;i++)
	{
		printf("%d ",int_consultarEnPosicion(MEME,i));
	}
	printf("\n");
}

void pruebaPuntero()
{
	printf("Prueba puntero\n");

	int_list x;
	int_inicializar(&x);

	for(int i=1;i<=5;i++)
	{
		int_insertar(&x,i,i + (i-1));
	}	

	imprimir(x);
	int_avanzarPuntero(&x);
	int_avanzarPuntero(&x);

	int_punteroAInicio(&x);

	printf("Consultado: %d\n", int_consultar(x));	

}

void pruebaCopia()
{
	printf("Prueba copia\n");

	int_list x, y;
	int_inicializar(&x);
	int_inicializar(&y);


	for(int i=0;i<10;i++)
	{
		int_insertar(&x, i + 1, i+2 + (2*i+1));
	}	
	printf("Original: \n");
	imprimir(x);


	printf("Copia, añadiendo elemento: \n");
	int_insertarCopia(&x, &y, 3, 99);
	imprimir(y);

	printf("Original modificado: \n");
	int_limpiar(&x);
	int_insertar(&x, 1, 1);
	imprimir(x);

	printf("Copia: \n");

	imprimir(y);

	int_list z;
	int_inicializar(&z);

	int_eliminarCopiaDesdePosicion(&y, &z, 3);
	printf("Copia: %d\n", int_sumarValor(&z, 1));

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
