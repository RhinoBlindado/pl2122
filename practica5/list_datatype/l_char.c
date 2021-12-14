#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "l_char.h"


	void char_inicializar(l_int *L)
	{
		L->primero = NULL;
		L->ultimo = NULL;
		L->longitud = 0;
		L->puntero = 1;
	}
	
	int char_esVacia(l_int L)
	{
		if (L.primero == NULL)
			return(1);
		else
			return(0);
	}
	
	void char_insertar(l_int *L, int p, char e)
	{
		int i;
		Nodo *ant,*sigu,*nuevo;
		if(p>0 && p<=char_longitud(L)+1)
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

	void char_eliminar(l_int *L, int p)
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

	int char_consultarEnPosicion(l_int L, int p)
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
	int char_esta(l_int L, char e)
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

	int char_longitud(l_int *L)
	{
		return(L->longitud);
	}

	void char_avanzarPuntero(l_int *L)
	{
		if(L->puntero < L->longitud)
			L->puntero++;
	}

	void char_retrocederPuntero(l_int *L)
	{
		if(L->puntero > 1)
			L->puntero--;
	}

	void char_punteroAInicio(l_int *L)
	{
		L->puntero = 1;
	}

	int char_consultar(l_int L)
	{
		return char_consultarEnPosicion(L, L.puntero);
	}

	void char_limpiar(l_int *L)
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


	void char_copiar(l_int *origList, l_int *copyList)
	{
		Nodo *ant, *sigu, *cAct, *cSigu;

		char_limpiar(copyList);
	
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

	void char_insertarCopia(l_int *L, l_int *LCopia, int p, char e)
	{
		char_copiar(L, LCopia);
		char_insertar(LCopia, p, e);
	}

	void char_eliminarCopia(l_int *L, l_int *LCopia, int p)
	{
		char_copiar(L, LCopia);
		char_eliminar(LCopia, p);
	}

	void char_eliminarDesdePosicion(l_int *L, int p)
	{	
		Nodo *act,*sigu;
		int i;
		if(p>0 && p <= L->longitud)
		{
			if(p==1)
			{
				char_limpiar(L);
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

	void char_eliminarCopiaDesdePosicion(l_int *L, l_int *LCopia, int p)
	{
		char_copiar(L, LCopia);
		char_eliminarDesdePosicion(LCopia, p);
	}

	void char_concatenarCopia(l_int *L1, l_int *L2, l_int *copyList)
	{
		Nodo *ant, *sigu, *cAct, *cSigu;

		char_limpiar(copyList);
		
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

	void char_imprimir(l_int L)
	{
		printf("[");
		
		if(L.longitud > 0)
		{
			Nodo *act, *sig;

			act = L.primero;
			sig = act->sig;	
			for (int i = 0; i < L.longitud - 1; i++)
			{
				printf("%c, ", act->data);
				act = sig;
				sig = sig->sig;
			}

			printf("%c", act->data);
		}
		printf("]\n");
	}
