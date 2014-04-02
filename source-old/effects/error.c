#include <stdio.h>
#include "defines.h"

void yyerror(const char *s)
{
	if (s == NULL)
	{
		return;
	}

	printf("%s\n", s);
}

