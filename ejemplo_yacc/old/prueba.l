%{
/*********************************************************
 **
 ** Fichero: PRUEBA.L
 ** Función: Pruebas de FLEX para practicas de PL.
 **
 *********************************************************/

/** Buffer usado para almacenamiento temporal de cadenas de texto
**/

#define MAXLONBUFFER 200
char buffer[MAXLONBUFFER] ;

%}

/** La siguiente declaración indica que no se va a escribir la función ’yywrap’
*** evita que se tenga que enlazar con -lfl, o que se tenga que escribir ’yywrap’
**/

%option noyywrap

%%

el|la { return(ARTICULO) ; }
a { return(ARTICULO_A) ; }
habla|ve|mira { return(VERBO) ; }
Juan|Pepe|Maria { return(NOMBRE_PROPIO) ; }
perro|gato|luna { return(NOMBRE) ; }

\n { /** lleva la cuenta de las líneas **/
	linea_actual++ ;
	}

[ \t]+ { /** accion vacia: se ignoran los espacios y tabuladores **/
       }

.      { /** token desconocido, se produce un mensaje de error **/
          snprintf(buffer, MAXLONBUFFER, "el(los) caracter(es) '%s' no forma(n) ningun token conocido", yytext);
			 yyerror(buffer);
		 }
