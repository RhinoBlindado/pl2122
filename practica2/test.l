%option noyywrap
%{
	int n_lineas = 0;
%}

%%

\"[^"]*\"                             { printf("CADENA"); }
[A-Za-z][A-Za-z0-9]*                      { printf("IDENTIF"); } // añadir al PDF
[0-9]*\.[0-9]*|[0-9]+|'.'|Verdadero|Falso { printf("LITERAL"); }

 /* \n { ++n_lineas; } */
[ \t]* { printf(" "); }
. { printf("\nError: %s\n", yytext); }

%%

int main() {
	yylex();
	// printf("# of lines = %d, # of chars = %d\n", num_lines, num_chars);
}
