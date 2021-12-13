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
		
		void int_inicializar(int_list *L);

		int int_esVacia(int_list L);

		void int_insertar(int_list *L, int p, int e);

		void int_eliminar(int_list *L,int p);

		int int_esta(int_list L, int e);

		void int_limpiar(int_list *L);

		// Funciones añadidas

		// Listo - Probado
		void int_avanzarPuntero(int_list *L);

		// Listo - Probado
		void int_retrocederPuntero(int_list *L);

		// Listo - Probado
		void int_punteroAInicio(int_list *L);

		// Listo - Probado
		int int_longitud(int_list *L);

		// Listo - Probado
		int int_consultarEnPosicion(int_list L, int p);

		// Listo - Probado
		int int_consultar(int_list L);

		// Listo - Probado
		void int_insertarCopia(int_list *L, int_list *LCopia, int p, int e);

		// Listo - Probado
		void int_eliminarCopia(int_list *L, int_list *LCopia, int p);

		// Listo - Probado
		void int_eliminarCopiaDesdePosicion(int_list *L, int_list *LCopia, int p);

		void int_concatenarCopia(int_list *L1, int_list *L2, int_list *LCopia);

		// Listo - Probado
		void int_sumarLista(int_list *L, int valor);

		// Listo - Probado
		int int_sumarValor(int_list *L, int valor);

		// Listo - Probado
		void int_multiplicarLista(int_list *L, int valor);

		// Listo - Probado
		int int_multiplicarValor(int_list *L, int valor);

		// Listo - Probado
		void int_restarLista(int_list *L, int valor);

		// Listo - Probado
		void int_dividirLista(int_list *L, int valor);

#endif
