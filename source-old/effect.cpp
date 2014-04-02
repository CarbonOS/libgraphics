#include <stdio.h>
#include "graphics.h"
#include "device.h"
#include "private.h"
#include "effects/compiler.h"
#include "effects/scanner.h"
#include "effects/parser.h"

extern "C" int ParseEffect(const char *text)
{
/*
	Scanner scanner;
	if (!scanner.Init(text))
	{
		return -1;
	}

	Token token;
	while (scanner.GetToken(&token))
	{
		printf("\t%d\n", token.code);
	}
*/
	Parser parser;
	parser.Parse(text);

	return 0;
}


