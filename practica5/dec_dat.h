#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#ifndef DEC_DAT_H
	#define DEC_DAT_H

		/* LISTA DE ENTERO */

		typedef struct NODO_INT
		{
			int data;
			struct NODO_INT* sig;
		}Nodo_int;
		
		typedef struct
		{
			Nodo_int *primero,*ultimo;
			int longitud;
			int puntero;
			double isInit;
		}l_int;
		
		void int_inicializar(l_int *L)
		{
			L->primero = NULL;
			L->ultimo = NULL;
			L->longitud = 0;
			L->puntero = 1;
			L->isInit = 1995;
		}
		
		void int_isInit(l_int *L)
		{
			if(L->isInit != 1995)
			{
				int_inicializar(L);
			}
		}

		int int_esVacia(l_int L)
		{
			int_isInit(&L);

			if (L.primero == NULL)
				return(1);
			else
				return(0);
		}
		
		int int_longitud(l_int *L)
		{
			int_isInit(L);

			return(L->longitud);
		}

		void int_insertar(l_int *L, int p, int e)
		{
			int_isInit(L);
			int i;
			Nodo_int *ant,*sigu,*nuevo;
			if(p>0 && p<=int_longitud(L)+1)
			{
				nuevo = (Nodo_int *) malloc(sizeof (Nodo_int));
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

		void int_eliminar(l_int *L, int p)
		{	
			int_isInit(L);
			Nodo_int *ant,*act,*sigu;
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
					for(i=1;i<p-1;i++)
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

		int int_consultarEnPosicion(l_int L, int p)
		{	
			int_isInit(&L);

			int i;
			Nodo_int *flash;
			
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
		int int_esta(l_int L, int e)
		{
			int_isInit(&L);

			Nodo_int *finder;
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

		void int_avanzarPuntero(l_int *L)
		{
			int_isInit(L);

			if(L->puntero < L->longitud)
				L->puntero++;
		}

		void int_retrocederPuntero(l_int *L)
		{
			int_isInit(L);

			if(L->puntero > 1)
				L->puntero--;
		}

		void int_punteroAInicio(l_int *L)
		{
			int_isInit(L);

			L->puntero = 1;
		}

		int int_consultar(l_int L)
		{
			int_isInit(&L);

			return int_consultarEnPosicion(L, L.puntero);
		}

		void int_limpiar(l_int *L)
		{
			int_isInit(L);

			if(L->longitud > 0)
			{
				Nodo_int *act, *sig;

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


		void int_copiar(l_int *origList, l_int *copyList)
		{
			int_isInit(origList);
			int_isInit(copyList);

			if(origList->longitud > 0)
			{
				Nodo_int *ant, *sigu, *cAct, *cAnt;
			
				copyList->longitud = origList->longitud;
				copyList->puntero = origList->puntero;

				cAct = (Nodo_int *) malloc(sizeof (Nodo_int));

				// Obtener puntero de lista original.
				ant = origList->primero;
				// Puntero al siguiente
				sigu = ant->sig;
	
				// Se copia los datos a un nuevo nodo.
				cAct->data = ant->data;
				// Se asigna como primer puntero.
				copyList->primero = cAct;

				while(ant->sig != NULL)
				{
					ant = sigu;
					sigu = sigu->sig;

					cAnt = cAct;

					cAct = (Nodo_int *) malloc(sizeof (Nodo_int));
					
					cAct->data = ant->data;
					cAnt->sig = cAct;
				}

				copyList->ultimo = cAct;
				cAct->sig = NULL;
			}

		}

		void int_insertarCopia(l_int *L, l_int *LCopia, int p, int e)
		{
			int_isInit(L);
			int_isInit(LCopia);

			int_copiar(L, LCopia);
			int_insertar(LCopia, p, e);
		}

		void int_eliminarCopia(l_int *L, l_int *LCopia, int p)
		{
			int_isInit(L);

			int_copiar(L, LCopia);
			int_eliminar(LCopia, p);
		}

		void int_eliminarDesdePosicion(l_int *L, int p)
		{	
			int_isInit(L);

			Nodo_int *act,*sigu;
			int i;
			if(p>0 && p <= L->longitud)
			{
				if(p==1)
				{
					int_limpiar(L);
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

		void int_eliminarCopiaDesdePosicion(l_int *L, l_int *LCopia, int p)
		{
			int_isInit(L);
			int_isInit(LCopia);

			int_copiar(L, LCopia);
			int_eliminarDesdePosicion(LCopia, p);
		}

		void int_sumarLista(l_int *L, int valor)
		{
			int_isInit(L);

			if(L->longitud > 0)
			{
				Nodo_int *act, *sig;

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

		int int_sumarValor(l_int *L, int valor)
		{
			int_isInit(L);

			int output = 0;
			if(L->longitud > 0)
			{
				output = valor;

				Nodo_int *act, *sig;

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

		void int_multiplicarLista(l_int *L, int valor)
		{
			int_isInit(L);

			if(L->longitud > 0)
			{
				Nodo_int *act, *sig;

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

		int int_multiplicarValor(l_int *L, int valor)
		{
			int_isInit(L);

			int output = 0;
			if(L->longitud > 0)
			{
				output = valor;

				Nodo_int *act, *sig;

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

		void int_restarLista(l_int *L, int valor)
		{
			int_isInit(L);

			int_sumarLista(L, -valor);
		}

		void int_dividirLista(l_int *L, int valor)
		{
			int_isInit(L);

			if(L->longitud > 0)
			{
				Nodo_int *act, *sig;

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

		void int_concatenarCopia(l_int *L1, l_int *L2, l_int *copyList)
		{
			int_isInit(L1);
			int_isInit(L2);
			int_isInit(copyList);
		
			Nodo_int *ant, *sigu, *cAct, *cSigu;

			//int_limpiar(copyList);
			
			if(L1->longitud > 0 && L2->longitud > 0)
			{	
				copyList->longitud = L1->longitud + L2->longitud;
				copyList->puntero = 1;

				cAct = (Nodo_int *) malloc(sizeof (Nodo_int));
				cSigu = (Nodo_int *) malloc(sizeof (Nodo_int));

				ant = L1->primero;
				sigu = ant->sig;

				cSigu->data = sigu->data;

				cAct->data = ant->data;
				cAct->sig = cSigu;

				copyList->primero = cAct;

				for(int i=0; i<L1->longitud - 2; i++)
				{
					cAct = cSigu;
					cSigu = (Nodo_int *) malloc(sizeof (Nodo_int));

					ant = sigu;
					sigu = sigu->sig;

					cSigu->data = sigu->data;
					cAct->sig = cSigu;
				}

				cAct = cSigu;
				cSigu = (Nodo_int *) malloc(sizeof (Nodo_int));

				ant = sigu;
				sigu = L2->primero;

				cSigu->data = sigu->data;
				cAct->sig = cSigu;

				for(int i=0; i<L2->longitud - 1; i++)
				{
					cAct = cSigu;
					cSigu = (Nodo_int *) malloc(sizeof (Nodo_int));

					ant = sigu;
					sigu = sigu->sig;

					cSigu->data = sigu->data;
					cAct->sig = cSigu;
				}
				
				cSigu->sig = NULL;
				copyList->ultimo = cSigu;
			}
		}

		void int_imprimir(l_int L)
		{
			int_isInit(&L);
			
			printf("[");
			
			if(L.longitud > 0)
			{
				Nodo_int *act, *sig;

				act = L.primero;
				sig = act->sig;	
				for (int i = 0; i < L.longitud - 1; i++)
				{
					printf("%d, ", act->data);
					act = sig;
					sig = sig->sig;
				}

				printf("%d", act->data);
			}
			printf("]\n");
		}

 /* LISTA DE FLOTANTE */
		typedef struct NODO_DOUBLE
		{
			double data;
			struct NODO_DOUBLE* sig;
		}Nodo_double;
		
		typedef struct
		{
			Nodo_double *primero,*ultimo;
			int longitud;
			int puntero;
			double isInit;
		}l_double;



		void double_inicializar(l_double *L)
		{
			L->primero = NULL;
			L->ultimo = NULL;
			L->longitud = 0;
			L->puntero = 1;
			L->isInit = 1995;
		}

		void double_isInit(l_double *L)
		{
			if(L->isInit != 1995)
			{
				double_inicializar(L);
			}
		}
	
	int double_esVacia(l_double L)
	{
		double_isInit(&L);

		if (L.primero == NULL)
			return(1);
		else
			return(0);
	}
	
	int double_longitud(l_double *L)
	{
		double_isInit(L);

		return(L->longitud);
	}

	void double_insertar(l_double *L, int p, double e)
	{
		double_isInit(L);

		int i;
		Nodo_double *ant,*sigu,*nuevo;
		if(p>0 && p<=double_longitud(L)+1)
		{
			nuevo = (Nodo_double *) malloc(sizeof (Nodo_double));
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

	void double_eliminar(l_double *L, int p)
	{	
		double_isInit(L);

		Nodo_double *ant,*act,*sigu;
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
				for(i=1;i<p - 1;i++)
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

	double double_consultarEnPosicion(l_double L, int p)
	{	
		double_isInit(&L);

		int i;
		Nodo_double *flash;
		
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
	int double_esta(l_double L, double e)
	{
		double_isInit(&L);

		Nodo_double *finder;
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



	void double_avanzarPuntero(l_double *L)
	{
		double_isInit(L);

		if(L->puntero < L->longitud)
			L->puntero++;
	}

	void double_retrocederPuntero(l_double *L)
	{
		double_isInit(L);

		if(L->puntero > 1)
			L->puntero--;
	}

	void double_punteroAInicio(l_double *L)
	{
		double_isInit(L);

		L->puntero = 1;
	}

	double double_consultar(l_double L)
	{
		double_isInit(&L);

		return double_consultarEnPosicion(L, L.puntero);
	}

	void double_limpiar(l_double *L)
	{
		double_isInit(L);

		if(L->longitud > 0)
		{
			Nodo_double *act, *sig;

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


	void double_copiar(l_double *origList, l_double *copyList)
	{
		double_isInit(origList);
		double_isInit(copyList);

			if(origList->longitud > 0)
			{
				Nodo_double *ant, *sigu, *cAct, *cAnt;
			
				copyList->longitud = origList->longitud;
				copyList->puntero = origList->puntero;

				cAct = (Nodo_double *) malloc(sizeof (Nodo_double));

				// Obtener puntero de lista original.
				ant = origList->primero;
				// Puntero al siguiente
				sigu = ant->sig;
	
				// Se copia los datos a un nuevo nodo.
				cAct->data = ant->data;
				// Se asigna como primer puntero.
				copyList->primero = cAct;

				while(ant->sig != NULL)
				{
					ant = sigu;
					sigu = sigu->sig;

					cAnt = cAct;

					cAct = (Nodo_double *) malloc(sizeof (Nodo_double));
					
					cAct->data = ant->data;
					cAnt->sig = cAct;
				}

				copyList->ultimo = cAct;
				cAct->sig = NULL;
			}
	}

	void double_insertarCopia(l_double *L, l_double *LCopia, int p, double e)
	{
		double_isInit(L);
		double_isInit(LCopia);

		double_copiar(L, LCopia);
		double_insertar(LCopia, p, e);
	}

	void double_eliminarCopia(l_double *L, l_double *LCopia, int p)
	{
		double_isInit(L);
		double_isInit(LCopia);

		double_copiar(L, LCopia);
		double_eliminar(LCopia, p);
	}

	void double_eliminarDesdePosicion(l_double *L, int p)
	{
		double_isInit(L);
	
		Nodo_double *act,*sigu;
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

	void double_eliminarCopiaDesdePosicion(l_double *L, l_double *LCopia, int p)
	{
		double_isInit(L);
		double_isInit(LCopia);

		double_copiar(L, LCopia);
		double_eliminarDesdePosicion(LCopia, p);
	}

	void double_sumarLista(l_double *L, double valor)
	{
		double_isInit(L);

		if(L->longitud > 0)
		{
			Nodo_double *act, *sig;

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

	double double_sumarValor(l_double *L, double valor)
	{
		double_isInit(L);

		double output = 0;
		if(L->longitud > 0)
		{
			output = valor;

			Nodo_double *act, *sig;

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

	void double_multiplicarLista(l_double *L, double valor)
	{
		double_isInit(L);

		if(L->longitud > 0)
		{
			Nodo_double *act, *sig;

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

	double double_multiplicarValor(l_double *L, double valor)
	{
		double_isInit(L);

		int output = 0;
		if(L->longitud > 0)
		{
			output = valor;

			Nodo_double *act, *sig;

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

	void double_restarLista(l_double *L, double valor)
	{
		double_isInit(L);

		double_sumarLista(L, -valor);
	}

	void double_dividirLista(l_double *L, double valor)
	{
		double_isInit(L);

		if(L->longitud > 0)
		{
			Nodo_double *act, *sig;

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

	void double_concatenarCopia(l_double *L1, l_double *L2, l_double *copyList)
	{
		double_isInit(L1);
		double_isInit(L2);
		double_isInit(copyList);

		if(L1->longitud > 0 && L2->longitud > 0)
		{
			Nodo_double *ant, *sigu, *cAct, *cSigu;

			double_limpiar(copyList);
			
			copyList->longitud = L1->longitud + L2->longitud;
			copyList->puntero = 1;

			cAct = (Nodo_double *) malloc(sizeof (Nodo_double));
			cSigu = (Nodo_double *) malloc(sizeof (Nodo_double));

			ant = L1->primero;
			sigu = ant->sig;

			cSigu->data = sigu->data;

			cAct->data = ant->data;
			cAct->sig = cSigu;

			copyList->primero = cAct;

			for(int i=0; i<L1->longitud - 2; i++)
			{
				cAct = cSigu;
				cSigu = (Nodo_double *) malloc(sizeof (Nodo_double));

				ant = sigu;
				sigu = sigu->sig;

				cSigu->data = sigu->data;
				cAct->sig = cSigu;
			}

			cAct = cSigu;
			cSigu = (Nodo_double *) malloc(sizeof (Nodo_double));

			ant = sigu;
			sigu = L2->primero;

			cSigu->data = sigu->data;
			cAct->sig = cSigu;

			for(int i=0; i<L2->longitud - 1; i++)
			{
				cAct = cSigu;
				cSigu = (Nodo_double *) malloc(sizeof (Nodo_double));

				ant = sigu;
				sigu = sigu->sig;

				cSigu->data = sigu->data;
				cAct->sig = cSigu;
			}
			
			cSigu->sig = NULL;
			copyList->ultimo = cSigu;
		}
	}

	void double_imprimir(l_double L)
	{
		double_isInit(&L);

		printf("[");
		
		if(L.longitud > 0)
		{
			Nodo_double *act, *sig;

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

	/* LISTA DE CHAR */

	typedef struct NODO_CHAR
	{
		char data;
		struct NODO_CHAR* sig;
	}Nodo_char;
	
	typedef struct
	{
		Nodo_char *primero,*ultimo;
		int longitud;
		int puntero;
		double isInit;
	}l_char;

	void char_inicializar(l_char *L)
	{
		L->primero = NULL;
		L->ultimo = NULL;
		L->longitud = 0;
		L->puntero = 1;
		L->isInit = 1995;
	}
	
	void char_isInit(l_char *L)
	{
		if(L->isInit != 1995)
		{
			char_inicializar(L);
		}
	}


	int char_esVacia(l_char L)
	{
		char_isInit(&L);

		if (L.primero == NULL)
			return(1);
		else
			return(0);
	}
	
	int char_longitud(l_char *L)
	{
		char_isInit(L);
		return(L->longitud);
	}

	void char_insertar(l_char *L, int p, char e)
	{
		char_isInit(L);

		int i;
		Nodo_char *ant,*sigu,*nuevo;
		if(p>0 && p<=char_longitud(L)+1)
		{
			nuevo = (Nodo_char *) malloc(sizeof (Nodo_char));
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

	void char_eliminar(l_char *L, int p)
	{	
		char_isInit(L);

		Nodo_char *ant,*act,*sigu;
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
				for(i=1;i<p - 1;i++)
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

	char char_consultarEnPosicion(l_char L, int p)
	{	
		char_isInit(&L);

		int i;
		Nodo_char *flash;
		
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
	int char_esta(l_char L, char e)
	{
		char_isInit(&L);

		Nodo_char *finder;
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


	void char_avanzarPuntero(l_char *L)
	{
		char_isInit(L);
		if(L->puntero < L->longitud)
			L->puntero++;
	}

	void char_retrocederPuntero(l_char *L)
	{
		char_isInit(L);
		if(L->puntero > 1)
			L->puntero--;
	}

	void char_punteroAInicio(l_char *L)
	{
		char_isInit(L);
		L->puntero = 1;
	}

	char char_consultar(l_char L)
	{
		char_isInit(&L);
		return char_consultarEnPosicion(L, L.puntero);
	}

	void char_limpiar(l_char *L)
	{
		char_isInit(L);

		if(L->longitud > 0)
		{
			Nodo_char *act, *sig;

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


	void char_copiar(l_char *origList, l_char *copyList)
	{
		char_isInit(origList);
		char_isInit(copyList);

		if(origList->longitud > 0)
		{
			Nodo_char *ant, *sigu, *cAct, *cAnt;
		
			copyList->longitud = origList->longitud;
			copyList->puntero = origList->puntero;

			cAct = (Nodo_char *) malloc(sizeof (Nodo_char));

			// Obtener puntero de lista original.
			ant = origList->primero;
			// Puntero al siguiente
			sigu = ant->sig;

			// Se copia los datos a un nuevo nodo.
			cAct->data = ant->data;
			// Se asigna como primer puntero.
			copyList->primero = cAct;

			while(ant->sig != NULL)
			{
				ant = sigu;
				sigu = sigu->sig;

				cAnt = cAct;

				cAct = (Nodo_char *) malloc(sizeof (Nodo_char));
				
				cAct->data = ant->data;
				cAnt->sig = cAct;
			}

			copyList->ultimo = cAct;
			cAct->sig = NULL;
		}
	}

	void char_insertarCopia(l_char *L, l_char *LCopia, int p, char e)
	{
		char_isInit(L);
		char_isInit(LCopia);

		char_copiar(L, LCopia);
		char_insertar(LCopia, p, e);
	}

	void char_eliminarCopia(l_char *L, l_char *LCopia, int p)
	{
		char_isInit(L);
		char_isInit(LCopia);

		char_copiar(L, LCopia);
		char_eliminar(LCopia, p);
	}

	void char_eliminarDesdePosicion(l_char *L, int p)
	{
		char_isInit(L);

		Nodo_char *act,*sigu;
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

	void char_eliminarCopiaDesdePosicion(l_char *L, l_char *LCopia, int p)
	{
		char_isInit(L);
		char_isInit(LCopia);

		char_copiar(L, LCopia);
		char_eliminarDesdePosicion(LCopia, p);
	}

	void char_concatenarCopia(l_char *L1, l_char *L2, l_char *copyList)
	{
		char_isInit(L1);
		char_isInit(L2);
		char_isInit(copyList);

		if(L1->longitud > 0 && L2->longitud > 0)
		{
			Nodo_char *ant, *sigu, *cAct, *cSigu;

			char_limpiar(copyList);
			
			copyList->longitud = L1->longitud + L2->longitud;
			copyList->puntero = 1;


			cAct = (Nodo_char *) malloc(sizeof (Nodo_char));
			cSigu = (Nodo_char *) malloc(sizeof (Nodo_char));

			ant = L1->primero;
			sigu = ant->sig;

			cSigu->data = sigu->data;

			cAct->data = ant->data;
			cAct->sig = cSigu;

			copyList->primero = cAct;

			for(int i=0; i<L1->longitud - 2; i++)
			{
				cAct = cSigu;
				cSigu = (Nodo_char *) malloc(sizeof (Nodo_char));

				ant = sigu;
				sigu = sigu->sig;

				cSigu->data = sigu->data;
				cAct->sig = cSigu;
			}

			cAct = cSigu;
			cSigu = (Nodo_char *) malloc(sizeof (Nodo_char));

			ant = sigu;
			sigu = L2->primero;

			cSigu->data = sigu->data;
			cAct->sig = cSigu;

			for(int i=0; i<L2->longitud - 1; i++)
			{
				cAct = cSigu;
				cSigu = (Nodo_char *) malloc(sizeof (Nodo_char));

				ant = sigu;
				sigu = sigu->sig;

				cSigu->data = sigu->data;
				cAct->sig = cSigu;
			}
			
			cSigu->sig = NULL;
			copyList->ultimo = cSigu;
		}
	}

	void char_imprimir(l_char L)
	{
		char_isInit(&L);

		printf("[");
		
		if(L.longitud > 0)
		{
			Nodo_char *act, *sig;

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

#endif