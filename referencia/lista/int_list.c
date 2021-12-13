#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "int_list.h"


	void inicializar(int_list *L)
	{
		L->primero = NULL;
		L->ultimo = NULL;
		L->longitud = 0;
		L->puntero = 1;
	}
	
	int esVacia(int_list L)
	{
		if (L.primero == NULL)
			return(1);
		else
			return(0);
	}
	
	void insertar(int_list *L, int p,int e)
	{
		int i;
		Nodo *ant,*sigu,*nuevo;
		if(p>0 && p<=longitud(L)+1)
		{
			nuevo = (Nodo *) malloc(sizeof (Nodo));
			nuevo->information = e;
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

	void eliminar(int_list *L, int p)
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

	int consultarEnPosicion(int_list L, int p)
	{	

		int i;
		Nodo *flash;
		
		if(p>0 && p <= L.longitud)
		{
			if(p == 1)
				return(L.primero->information);
			else if(p == L.longitud)
				return(L.ultimo->information);
			else
			{
				flash = L.primero;
				for(i=2;i<=p;i++)
				{
					flash = flash->sig;
				}
				return(flash->information);
			}
		}
		
	}
	int esta(int_list L, int e)
	{
		Nodo *finder;
		int i,flag;
		
		finder=L.primero;
		
		for(i=0;i<L.longitud;i++)
		{
			if(finder->information == e)
			{
				return ++i;
			}
			finder = finder->sig;	
		}
		return -1;
	}

	int longitud(int_list *L)
	{
		return(L->longitud);
	}

	void avanzarPuntero(int_list *L)
	{
		if(L->puntero < L->longitud)
			L->puntero++;
	}

	void retrocederPuntero(int_list *L)
	{
		if(L->puntero > 1)
			L->puntero--;
	}

	void punteroAInicio(int_list *L)
	{
		L->puntero = 1;
	}

	int consultar(int_list L)
	{
		return consultarEnPosicion(L, L.puntero);
	}

	void limpiar(int_list *L)
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


	void copiar(int_list *origList, int_list *copyList)
	{
		Nodo *ant, *sigu, *cAct, *cSigu;

		limpiar(copyList);
	
		copyList->longitud = origList->longitud;
		copyList->puntero = origList->puntero;

		cAct = (Nodo *) malloc(sizeof (Nodo));
		cSigu = (Nodo *) malloc(sizeof (Nodo));

		ant = origList->primero;
		sigu = ant->sig;

		cSigu->information = sigu->information;

		cAct->information = ant->information;
		cAct->sig = cSigu;

		copyList->primero = cAct;

		for(int i=0; i<copyList->longitud - 2; i++)
		{
			cAct = cSigu;
			cSigu = (Nodo *) malloc(sizeof (Nodo));

			ant = sigu;
			sigu = sigu->sig;

			cSigu->information = sigu->information;
			cAct->sig = cSigu;
		}

		cSigu->sig = NULL;
		copyList->ultimo = cSigu;
	}

	void insertarCopia(int_list *L, int_list *LCopia, int p, int e)
	{
		copiar(L, LCopia);
		insertar(LCopia, p, e);
	}

	void eliminarCopia(int_list *L, int_list *LCopia, int p)
	{
		copiar(L, LCopia);
		eliminar(LCopia, p);
	}

	void eliminarDesdePosicion(int_list *L, int p)
	{	
		Nodo *act,*sigu;
		int i;
		if(p>0 && p <= L->longitud)
		{
			if(p==1)
			{
				limpiar(L);
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

	void eliminarCopiaDesdePosicion(int_list *L, int_list *LCopia, int p)
	{
		copiar(L, LCopia);
		eliminarDesdePosicion(LCopia, p);
	}

	void sumarLista(int_list *L, int valor)
	{
		if(L->longitud > 0)
		{
			Nodo *act, *sig;

			act = L->primero;
			sig = act->sig;

			for (int i = 0; i < L->longitud - 1; i++)
			{
				act->information += valor;
				act = sig;
				sig = sig->sig;
			}

			act->information += valor;
		}
	}

	int sumarValor(int_list *L, int valor)
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
				output += act->information;
				act = sig;
				sig = sig->sig;
			}

			output += act->information;
		}
		return output;
	}

	void multiplicarLista(int_list *L, int valor)
	{
		if(L->longitud > 0)
		{
			Nodo *act, *sig;

			act = L->primero;
			sig = act->sig;

			for (int i = 0; i < L->longitud - 1; i++)
			{
				act->information *= valor;
				act = sig;
				sig = sig->sig;
			}

			act->information *= valor;
		}
	}

	int multiplicarValor(int_list *L, int valor)
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
				output *= act->information;
				act = sig;
				sig = sig->sig;
			}

			output *= act->information;
		}
		return output;
	}

	void restarLista(int_list *L, int valor)
	{
		sumarLista(L, -valor);
	}

	void dividirLista(int_list *L, int valor)
	{
		if(L->longitud > 0)
		{
			Nodo *act, *sig;

			act = L->primero;
			sig = act->sig;

			for (int i = 0; i < L->longitud - 1; i++)
			{
				act->information /= valor;
				act = sig;
				sig = sig->sig;
			}

			act->information /= valor;
		}
	}

	void concatenarCopia(int_list *L1, int_list *L2, int_list *LCopia)
	{

	}

