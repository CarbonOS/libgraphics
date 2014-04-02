#include <stdio.h>
#include "compiler.h"
#include "scanner.h"
#include "parser.h"

bool Parser::IsToken(int type)
{
    if (m_temp.code == type)
        return true;
    return false;
}

bool Parser::ParseScan()
{
    if (!GetToken(&m_temp))
        return false;
    return true;
}

bool Parser::ParseExpect(int type)
{
    if (!ParseScan())
        return false;

    if (m_temp.code == type)
        return true;

    printf("Ungültiger Ausdruck \"%d\".\n", m_temp.code);
    return false;
}

bool Parser::ParseTypeDeclaration()
{
    return false;
}

bool Parser::Parse(const char *text)
{
	if (!Init(text))
	{
		return false;
	}

	while (ParseExpect(1))
	{

	}

	return true;
}

