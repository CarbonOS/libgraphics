#include <stdio.h>
#include <string>
#include "compiler.h"
#include "scanner.h"

int Scanner::Read(void)
{
	if ((m_offset >= m_text.length()))
		return -1;
	return (int)m_text[m_offset++];
}

int Scanner::Peek(int count)
{
	if (((m_offset + count) >= m_text.length()))
		return -1;
	return (int)m_text[m_offset + count];
}

int Scanner::IsDigit(int code)
{
	if (((code >= '0') && (code <= '9')) || (code == '.'))
		return 1;
	return 0;
}

int Scanner::IsLetter(int code)
{
    if ((code >= 'a' && code <= 'z') || 
        (code >= 'A' && code <= 'Z') || (code == '_'))
        return 1;
    return 0;
}

static int IsOperator(int code)
{
	if (code == '+' ||
		code == '-' ||
		code == '*' ||
		/* code == '/' || */
		code == '^' ||
		code == '&' ||
		code == '>' ||
		code == '<' ||
		code == '=' )
		return 1;
    return 0;
}

int Scanner::Skip(void)
{
	int code = 0;
	while ((code = Peek(0)) != -1)
	{
		switch (code)
		{
		    case ' ':
		    case '\t':
			Read();
			continue;
		    case '\r':
		    case '\n':
			Read();
			m_col = 1;
			m_line++;
			continue;
		    default:
			return 1;
		}
	}

	return 0;
}

int Scanner::ParseDigit(Token *token)
{
	int code = Peek(0);
	if (!IsDigit(code))
		return 0;

	int e = 0;
	int s = m_offset;
	while (IsDigit(Peek(0)))
	{
		Read();
		e++;
	}

	token->code = TOKEN_NUMBER;
	token->i_value = 0;
	printf("%s\n", m_text.substr(s, e).c_str());
	
	return 1;
}

int Scanner::ParseString(Token *token)
{
	int code = Peek(0);
	if (!IsLetter(code))
		return 0;

	int e = 0;
	int s = m_offset;
	while (IsLetter(Peek(0)))
	{
		Read();
		e++;
	}

	token->code = TOKEN_STRING;
	token->text = m_text.substr(s, e);
	printf("%s\n", m_text.substr(s, e).c_str());

	return 1;
}

int Scanner::ParseOperator(Token *token)
{
    int code = Peek(0);
    if (!IsOperator(code))
        return false;

    Read();
    token->code = code;
    token->text.clear();
    return true;
}

int Scanner::ParseCustom(Token *token)
{
    int code = Peek(0);
    if (code != '(' &&
        code != ')' &&
        code != '{' &&
        code != '}' &&
        code != '[' &&
        code != ']' &&
        code != ',' &&
        code != ';')
        return false;

    return true;
}

int Scanner::ParseToken(Token *token)
{
    if (!ParseString(token))
        return 0;

    if (!token->text.compare("if"))
        token->code = 10;

    if (!token->text.compare("else"))
        token->code = 11;

    if (!token->text.compare("while"))
        token->code = 12;

    return 1;
}

bool Scanner::Init(const char *text)
{
	if ((text == NULL))
	{
		return false;
	}

	m_col = 1;
	m_line = 1;
	m_text = text;
	m_offset = 0;
	return true;
}

bool Scanner::GetToken(Token *token)
{
	if ((token == NULL))
	{
		return false;
	}

	Skip();

	if (ParseDigit(token))
		return true;

	if (ParseToken(token))
		return true;

	if (ParseOperator(token))
		return true;

	return false;
}

