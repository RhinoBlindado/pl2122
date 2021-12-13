#include <stdio.h>
#include "Lista.h"

void imprimir(Lista MEME)
{
	int i,j;
	j = longitud(&MEME);
	for (i=1;i<=j;i++)
	{
		printf("%d ",consultar(MEME,i));
	}
	printf("\n");
}

int main()
{
	Lista X;
	int i;
	vacia(&X);	
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
	

}
