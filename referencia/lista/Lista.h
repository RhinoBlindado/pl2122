#ifndef LISTA_H
#define LISTA_H


	typedef struct NODO
	{
		int information;
		struct NODO* sig;
	}Nodo;
	
	typedef struct
	{
		Nodo *primero,*ultimo;
		int longitud;
	}Lista;
	
	void vacia(Lista *L);
	int esVacia(Lista L);
	void insertar(Lista *L, int p,int e);
	void eliminar(Lista *L,int p);
	int consultar(Lista L, int p);
	int esta(Lista L, int e);
	int longitud(Lista *L);
	
#endif
