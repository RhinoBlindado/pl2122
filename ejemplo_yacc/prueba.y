%{
/*********************************************************
 **
 ** Fichero: PRUEBA.Y
 ** Función: Pruebas de YACC para practicas de PL
 **
 ********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** La siguiente declaracion permite que ’yyerror’ se pueda invocar desde el
*** fuente de lex (prueba.l)
**/

void yyerror( char * msg ) ;

/** La siguiente variable se usará para conocer el numero de la línea
*** que se esta leyendo en cada momento. También es posible usar la variable
*** ’yylineno’ que también nos muestra la línea actual. Para ello es necesario
*** invocar a flex con la opción ’-l’ (compatibilidad con lex).
**/

int linea_actual = 1 ;
%}

/** Para uso de mensajes de error sintáctico con BISON.
*** La siguiente declaración provoca que ’bison’, ante un error sintáctico,
*** visualice mensajes de error con indicación de los tokens que se esperaban
*** en el lugar en el que produjo el error (SÓLO FUNCIONA CON BISON>=2.1).
*** Para Bison<2.1 es mediante
***
*** #define YYERROR_VERBOSE
***
*** En caso de usar mensajes de error mediante ’mes’ y ’mes2’ (ver apéndice D)
*** nada de lo anterior debe tenerse en cuenta.
**/

//%error-verbose
%define parse.error verbose

/** A continuación declaramos los nombres simbólicos de los tokens.
*** byacc se encarga de asociar a cada uno un código
**/

%token NOMBRE VERBO
%token NOMBRE_PROPIO
%token ARTICULO ARTICULO_A
%token DESCONOCIDO

%%

/** Sección de producciones que definen la gramática.
**/

frase              : sintagma_nominal sintagma_predicado ;
sintagma_nominal   : ARTICULO NOMBRE
                   | NOMBRE_PROPIO
                   ;
sintagma_predicado : VERBO modificador objeto_directo ;
objeto_directo     : sintagma_nominal ;
modificador        : ARTICULO_A
                   | /** esto representa la cadena vacía **/
                   ;

%%

/** aqui incluimos el fichero generado por el ’lex’
*** que implementa la función ’yylex’
**/

#ifdef DOSWINDOWS /* Variable de entorno que indica la plataforma */
#include "lexyy.c"
#else
#include "lex.yy.c"
#endif

/** se debe implementar la función yyerror. En este caso
*** simplemente escribimos el mensaje de error en pantalla
**/

void yyerror( char *msg )
{
   fprintf(stderr,"[Linea %d]: %s\n", linea_actual, msg) ;
}
