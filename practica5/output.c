int funcionA((null))(null);/* PROGRAMA GENERADO POR YACC/LEX */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "dec_fun.c"

int ve, ve2;
double vf;
char vc, v2, v4;
bool vl, vl2, vl3, vl4;

int main()
{
	{
		int tmp1;
		tmp1 = pow(3, 2);

		int tmp2;
		tmp2 = 4 * tmp1;

		ve = tmp2;
	}
	{
		vf = 3.5;
	}
	if(!true) goto etiqueta2;
	{
		{
			ve = 7;
		}
		if(!true) goto etiqueta1;
		{
			{
				ve = 1;
			}
			{
				vf = 4.33;
			}
			{
				vf = 7.44;
			}
			if(!false) goto etiqueta0;
			{
				{
					ve = 4444;
				}
			}
			etiqueta0:
			{
			}
		}
		etiqueta1:
		{
		}
	}
	etiqueta2:
	{
	}
	printf("%d\n", ve);
	printf("%lf\n", vf);
}
