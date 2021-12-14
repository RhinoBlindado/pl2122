#ifndef L_INT_H
	#define L_INT_H

		typedef struct NODO
		{
			int data;
			struct NODO* sig;
		}Nodo;
		
		typedef struct
		{
			Nodo *primero,*ultimo;
			int longitud;
			int puntero;
		}l_int;
		
		void int_inicializar(l_int *L);

		int int_esVacia(l_int L);

		void int_insertar(l_int *L, int p, int e);

		void int_eliminar(l_int *L,int p);

		int int_esta(l_int L, int e);

		void int_limpiar(l_int *L);

		// Funciones añadidas

		// Listo - Probado
		void int_avanzarPuntero(l_int *L);

		// Listo - Probado
		void int_retrocederPuntero(l_int *L);

		// Listo - Probado
		void int_punteroAInicio(l_int *L);

		// Listo - Probado
		int int_longitud(l_int *L);

		// Listo - Probado
		int int_consultarEnPosicion(l_int L, int p);

		// Listo - Probado
		int int_consultar(l_int L);

		// Listo - Probado
		void int_insertarCopia(l_int *L, l_int *LCopia, int p, int e);

		// Listo - Probado
		void int_eliminarCopia(l_int *L, l_int *LCopia, int p);

		// Listo - Probado
		void int_eliminarCopiaDesdePosicion(l_int *L, l_int *LCopia, int p);

		void int_concatenarCopia(l_int *L1, l_int *L2, l_int *LCopia);

		// Listo - Probado
		void int_sumarLista(l_int *L, int valor);

		// Listo - Probado
		int int_sumarValor(l_int *L, int valor);

		// Listo - Probado
		void int_multiplicarLista(l_int *L, int valor);

		// Listo - Probado
		int int_multiplicarValor(l_int *L, int valor);

		// Listo - Probado
		void int_restarLista(l_int *L, int valor);

		// Listo - Probado
		void int_dividirLista(l_int *L, int valor);

		// 
		void int_imprimir(l_int L);

#endif
