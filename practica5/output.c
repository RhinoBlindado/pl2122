/* PROGRAMA GENERADO POR YACC/LEX */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "dec_func.h"
#include "dec_dat.h"

int a, b, c, cont;
l_int lista1, lista2, lista3, listaAux1, listaAux2;

int main()
{
	printf("%s", "Ingresa un numero:\n>");
	scanf("%d", &a);
	{
		b = pares(a);
	}
	{
		b = impares(a);
	}
	{
		b = imprimir_lista();
	}
	{
		l_int tmp21;
		int_sumarLista(&lista1,3);int_copiar(&lista1,&tmp21);

		lista3 = tmp21;
	}
	printf("%s", "sumando tres a pares: \n");
	{
		int tmp22;
		tmp22 = int_longitud(&lista3);

		cont = tmp22;
	}
	{
		c = 1;
	}
	etiqueta12:
	if(c > cont) goto etiqueta13;
	{
		int tmp24;
		{
			int tmp23;
			tmp23 = int_consultarEnPosicion(lista3, c);

			tmp24 = tmp23;
		}
		printf("%d", tmp24);
		printf("%s", ", ");
	}
	c += 1;
	goto etiqueta12;
	etiqueta13:
	{
	}
	printf("%s", "\n");
	{
		l_int tmp25;
		int_multiplicarLista(&lista1,2);
int_copiar(&lista1,&tmp25);

		lista3 = tmp25;
	}
	printf("%s", "multiplicando pares por dos: \n");
	{
		int tmp26;
		tmp26 = int_longitud(&lista3);

		cont = tmp26;
	}
	{
		c = 1;
	}
	etiqueta14:
	if(c > cont) goto etiqueta15;
	{
		int tmp28;
		{
			int tmp27;
			tmp27 = int_consultarEnPosicion(lista3, c);

			tmp28 = tmp27;
		}
		printf("%d", tmp28);
		printf("%s", ", ");
	}
	c += 1;
	goto etiqueta14;
	etiqueta15:
	{
	}
	printf("%s", "\n");
}
