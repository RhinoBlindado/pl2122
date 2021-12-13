#ifndef INT_LIST_H
	#define INT_LIST_H


		typedef struct NODO
		{
			int information;
			struct NODO* sig;
		}Nodo;
		
		typedef struct
		{
			Nodo *primero,*ultimo;
			int longitud;
			int puntero;
		}int_list;
		
		void inicializar(int_list *L);

		int esVacia(int_list L);

		void insertar(int_list *L, int p, int e);

		void eliminar(int_list *L,int p);

		int esta(int_list L, int e);

		void limpiar(int_list *L);

		// Funciones añadidas

		// Listo - Probado
		void avanzarPuntero(int_list *L);

		// Listo - Probado
		void retrocederPuntero(int_list *L);

		// Listo - Probado
		void punteroAInicio(int_list *L);

		// Listo - Probado
		int longitud(int_list *L);

		// Listo - Probado
		int consultarEnPosicion(int_list L, int p);

		// Listo - Probado
		int consultar(int_list L);

		// Listo - Probado
		void insertarCopia(int_list *L, int_list *LCopia, int p, int e);

		// Listo - Probado
		void eliminarCopia(int_list *L, int_list *LCopia, int p);

		// Listo - Probado
		void eliminarCopiaDesdePosicion(int_list *L, int_list *LCopia, int p);

		void concatenarCopia(int_list *L1, int_list *L2, int_list *LCopia);

		// Listo - Probado
		void sumarLista(int_list *L, int valor);

		// Listo - Probado
		int sumarValor(int_list *L, int valor);

		// Listo - Probado
		void multiplicarLista(int_list *L, int valor);

		// Listo - Probado
		int multiplicarValor(int_list *L, int valor);

		// Listo - Probado
		void restarLista(int_list *L, int valor);

		// Listo - Probado
		void dividirLista(int_list *L, int valor);

#endif
