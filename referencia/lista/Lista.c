#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Lista.h"


	void vacia(Lista *L)
	{
		L->primero = NULL;
		L->ultimo = NULL;
		L->longitud = 0;
	}
	
	int esVacia(Lista L)
	{
		if (L.primero == NULL)
			return(1);
		else
			return(0);
	}
	
	void insertar(Lista *L, int p,int e)
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
	void eliminar(Lista *L,int p)
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
	int consultar(Lista L, int p)
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
	int esta(Lista L, int e)
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

	int longitud(Lista *L)
	{
		return(L->longitud);
	}
