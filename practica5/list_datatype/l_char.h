#ifndef L_CHAR_H
	#define L_CHAR_H

		typedef struct NODO
		{
			char data;
			struct NODO* sig;
		}Nodo;
		
		typedef struct
		{
			Nodo *primero,*ultimo;
			int longitud;
			int puntero;
		}l_int;
		
		void char_inicializar(l_int *L);

		int char_esVacia(l_int L);

		void char_insertar(l_int *L, int p, char e);

		void char_eliminar(l_int *L, int p);

		int char_esta(l_int L, char e);

		void char_limpiar(l_int *L);

		// Funciones añadidas

		// Listo - Probado
		void char_avanzarPuntero(l_int *L);

		// Listo - Probado
		void char_retrocederPuntero(l_int *L);

		// Listo - Probado
		void char_punteroAInicio(l_int *L);

		// Listo - Probado
		int char_longitud(l_int *L);

		// Listo - Probado
		int char_consultarEnPosicion(l_int L, int p);

		// Listo - Probado
		int char_consultar(l_int L);

		// Listo - Probado
		void char_insertarCopia(l_int *L, l_int *LCopia, int p, char e);

		// Listo - Probado
		void char_eliminarCopia(l_int *L, l_int *LCopia, int p);

		// Listo - Probado
		void char_eliminarCopiaDesdePosicion(l_int *L, l_int *LCopia, int p);

		void char_concatenarCopia(l_int *L1, l_int *L2, l_int *LCopia);

		void char_imprimir(l_int L);

#endif
