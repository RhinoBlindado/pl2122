#ifndef L_DOUBLE_H
	#define L_DOUBLE_H

		typedef struct NODO
		{
			double data;
			struct NODO* sig;
		}Nodo;
		
		typedef struct
		{
			Nodo *primero,*ultimo;
			int longitud;
			int puntero;
		}l_int;
		
		void double_inicializar(l_int *L);

		int double_esVacia(l_int L);

		void double_insertar(l_int *L, int p, double e);

		void double_eliminar(l_int *L, int p);

		int double_esta(l_int L, double e);

		void double_limpiar(l_int *L);

		// Funciones añadidas

		// Listo - Probado
		void double_avanzarPuntero(l_int *L);

		// Listo - Probado
		void double_retrocederPuntero(l_int *L);

		// Listo - Probado
		void double_punteroAInicio(l_int *L);

		// Listo - Probado
		int double_longitud(l_int *L);

		// Listo - Probado
		int double_consultarEnPosicion(l_int L, int p);

		// Listo - Probado
		int double_consultar(l_int L);

		// Listo - Probado
		void double_insertarCopia(l_int *L, l_int *LCopia, int p, double e);

		// Listo - Probado
		void double_eliminarCopia(l_int *L, l_int *LCopia, int p);

		// Listo - Probado
		void double_eliminarCopiaDesdePosicion(l_int *L, l_int *LCopia, int p);

		void double_concatenarCopia(l_int *L1, l_int *L2, l_int *LCopia);

		// Listo - Probado
		void double_sumarLista(l_int *L, double valor);

		// Listo - Probado
		double double_sumarValor(l_int *L, double valor);

		// Listo - Probado
		void double_multiplicarLista(l_int *L, double valor);

		// Listo - Probado
		double double_multiplicarValor(l_int *L, double valor);

		// Listo - Probado
		void double_restarLista(l_int *L, double valor);

		// Listo - Probado
		void double_dividirLista(l_int *L, double valor);

		// 
		void double_imprimir(l_int L);

#endif
