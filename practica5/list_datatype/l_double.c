#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "l_double.h"


	void double_inicializar(l_int *L)
	{
		L->primero = NULL;
		L->ultimo = NULL;
		L->longitud = 0;
		L->puntero = 1;
	}
	
	int double_esVacia(l_int L)
	{
		if (L.primero == NULL)
			return(1);
		else
			return(0);
	}
	
	void double_insertar(l_int *L, int p, double e)
	{
		int i;
		Nodo *ant,*sigu,*nuevo;
		if(p>0 && p<=double_longitud(L)+1)
		{
			nuevo = (Nodo *) malloc(sizeof (Nodo));
			nuevo->data = e;
			if(p==1)
			{
				nuevo->sig = L->primero;
				L->primero = nuevo;
			}
			else
			{
				ant = L->primero;
				sigu = ant->sig;
				for(i=0;i<p-2;i++)
				{
					ant = sigu;
					sigu = sigu->sig;
				}
				ant->sig = nuevo;
				nuevo->sig = sigu;
			}
			
			if(p == L->longitud+1)
			{
				L->ultimo = nuevo;
			}
			L->longitud = L->longitud+1;
		}
	}

	void double_eliminar(l_int *L, int p)
	{	

		Nodo *ant,*act,*sigu;
		int i;
		if(p>0 && p <= L->longitud)
		{
			if(p==1)
			{
				act = L->primero;
				L->primero = L->primero->sig;
				
				if(L->primero == NULL)
					L->ultimo = NULL;
			}
			else
			{
				ant = L->primero;
				act = ant->sig;
				sigu = act->sig;
				for(i=1;i<p;i++)
				{
					ant = act;
					act = sigu;
					sigu = sigu->sig;
				}
				ant->sig = sigu;
				act->sig = NULL;
				
				if(sigu == NULL)
					L->ultimo = ant;
			}
			free(act);
			L->longitud = L->longitud-1;
		}	
	}

	int double_consultarEnPosicion(l_int L, int p)
	{	

		int i;
		Nodo *flash;
		
		if(p>0 && p <= L.longitud)
		{
			if(p == 1)
				return(L.primero->data);
			else if(p == L.longitud)
				return(L.ultimo->data);
			else
			{
				flash = L.primero;
				for(i=2;i<=p;i++)
				{
					flash = flash->sig;
				}
				return(flash->data);
			}
		}
		
	}
	int double_esta(l_int L, double e)
	{
		Nodo *finder;
		int i,flag;
		
		finder=L.primero;
		
		for(i=0;i<L.longitud;i++)
		{
			if(finder->data == e)
			{
				return ++i;
			}
			finder = finder->sig;	
		}
		return -1;
	}

	int double_longitud(l_int *L)
	{
		return(L->longitud);
	}

	void double_avanzarPuntero(l_int *L)
	{
		if(L->puntero < L->longitud)
			L->puntero++;
	}

	void double_retrocederPuntero(l_int *L)
	{
		if(L->puntero > 1)
			L->puntero--;
	}

	void double_punteroAInicio(l_int *L)
	{
		L->puntero = 1;
	}

	int double_consultar(l_int L)
	{
		return double_consultarEnPosicion(L, L.puntero);
	}

	void double_limpiar(l_int *L)
	{
		if(L->longitud > 0)
		{
			Nodo *act, *sig;

			act = L->primero;
			sig = act->sig;

			for (int i = 0; i < L->longitud - 1; i++)
			{
				free(act);
				act = sig;
				sig = sig->sig;
			}

			free(act);

			L->longitud = 0;
			L->primero = NULL;
			L->ultimo = NULL;
			L->puntero = 0;
		}
	}


	void double_copiar(l_int *origList, l_int *copyList)
	{
		Nodo *ant, *sigu, *cAct, *cSigu;

		double_limpiar(copyList);
	
		copyList->longitud = origList->longitud;
		copyList->puntero = origList->puntero;

		cAct = (Nodo *) malloc(sizeof (Nodo));
		cSigu = (Nodo *) malloc(sizeof (Nodo));

		ant = origList->primero;
		sigu = ant->sig;

		cSigu->data = sigu->data;

		cAct->data = ant->data;
		cAct->sig = cSigu;

		copyList->primero = cAct;

		for(int i=0; i<copyList->longitud - 2; i++)
		{
			cAct = cSigu;
			cSigu = (Nodo *) malloc(sizeof (Nodo));

			ant = sigu;
			sigu = sigu->sig;

			cSigu->data = sigu->data;
			cAct->sig = cSigu;
		}

		cSigu->sig = NULL;
		copyList->ultimo = cSigu;
	}

	void double_insertarCopia(l_int *L, l_int *LCopia, int p, double e)
	{
		double_copiar(L, LCopia);
		double_insertar(LCopia, p, e);
	}

	void double_eliminarCopia(l_int *L, l_int *LCopia, int p)
	{
		double_copiar(L, LCopia);
		double_eliminar(LCopia, p);
	}

	void double_eliminarDesdePosicion(l_int *L, int p)
	{	
		Nodo *act,*sigu;
		int i;
		if(p>0 && p <= L->longitud)
		{
			if(p==1)
			{
				double_limpiar(L);
				L->longitud = 0;
			}
			else
			{
				act = L->primero;
				sigu = act->sig;

				for(i = 1; i < p; i++)
				{
					act = sigu;
					sigu = sigu->sig;	
				}
				
				L->ultimo = act;
				act->sig = NULL;

				for(i = p; i < L->longitud; i++)
				{
					act = sigu;
					sigu = sigu->sig;
					free(act);
				}
				L->longitud = p;
			}
		}	
	}

	void double_eliminarCopiaDesdePosicion(l_int *L, l_int *LCopia, int p)
	{
		double_copiar(L, LCopia);
		double_eliminarDesdePosicion(LCopia, p);
	}

	void double_sumarLista(l_int *L, double valor)
	{
		if(L->longitud > 0)
		{
			Nodo *act, *sig;

			act = L->primero;
			sig = act->sig;

			for (int i = 0; i < L->longitud - 1; i++)
			{
				act->data += valor;
				act = sig;
				sig = sig->sig;
			}

			act->data += valor;
		}
	}

	double double_sumarValor(l_int *L, double valor)
	{
		double output = 0;
		if(L->longitud > 0)
		{
			output = valor;

			Nodo *act, *sig;

			act = L->primero;
			sig = act->sig;

			for (int i = 0; i < L->longitud - 1; i++)
			{
				output += act->data;
				act = sig;
				sig = sig->sig;
			}

			output += act->data;
		}
		return output;
	}

	void double_multiplicarLista(l_int *L, double valor)
	{
		if(L->longitud > 0)
		{
			Nodo *act, *sig;

			act = L->primero;
			sig = act->sig;

			for (int i = 0; i < L->longitud - 1; i++)
			{
				act->data *= valor;
				act = sig;
				sig = sig->sig;
			}

			act->data *= valor;
		}
	}

	double double_multiplicarValor(l_int *L, double valor)
	{
		int output = 0;
		if(L->longitud > 0)
		{
			output = valor;

			Nodo *act, *sig;

			act = L->primero;
			sig = act->sig;

			for (int i = 0; i < L->longitud - 1; i++)
			{
				output *= act->data;
				act = sig;
				sig = sig->sig;
			}

			output *= act->data;
		}
		return output;
	}

	void double_restarLista(l_int *L, double valor)
	{
		double_sumarLista(L, -valor);
	}

	void double_dividirLista(l_int *L, double valor)
	{
		if(L->longitud > 0)
		{
			Nodo *act, *sig;

			act = L->primero;
			sig = act->sig;

			for (int i = 0; i < L->longitud - 1; i++)
			{
				act->data /= valor;
				act = sig;
				sig = sig->sig;
			}

			act->data /= valor;
		}
	}

	void double_concatenarCopia(l_int *L1, l_int *L2, l_int *copyList)
	{
		Nodo *ant, *sigu, *cAct, *cSigu;

		double_limpiar(copyList);
		
		copyList->longitud = L1->longitud + L2->longitud;
		copyList->puntero = 1;

		cAct = (Nodo *) malloc(sizeof (Nodo));
		cSigu = (Nodo *) malloc(sizeof (Nodo));

		ant = L1->primero;
		sigu = ant->sig;

		cSigu->data = sigu->data;

		cAct->data = ant->data;
		cAct->sig = cSigu;

		copyList->primero = cAct;

		for(int i=0; i<L1->longitud - 2; i++)
		{
			cAct = cSigu;
			cSigu = (Nodo *) malloc(sizeof (Nodo));

			ant = sigu;
			sigu = sigu->sig;

			cSigu->data = sigu->data;
			cAct->sig = cSigu;
		}

		cAct = cSigu;
		cSigu = (Nodo *) malloc(sizeof (Nodo));

		ant = sigu;
		sigu = L2->primero;

		cSigu->data = sigu->data;
		cAct->sig = cSigu;

		for(int i=0; i<L2->longitud - 1; i++)
		{
			cAct = cSigu;
			cSigu = (Nodo *) malloc(sizeof (Nodo));

			ant = sigu;
			sigu = sigu->sig;

			cSigu->data = sigu->data;
			cAct->sig = cSigu;
		}
		
		cSigu->sig = NULL;
		copyList->ultimo = cSigu;
	}

	void double_imprimir(l_int L)
	{
		printf("[");
		
		if(L.longitud > 0)
		{
			Nodo *act, *sig;

			act = L.primero;
			sig = act->sig;	
			for (int i = 0; i < L.longitud - 1; i++)
			{
				printf("%f, ", act->data);
				act = sig;
				sig = sig->sig;
			}

			printf("%f", act->data);
		}
		printf("]\n");
	}
