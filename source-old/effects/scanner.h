#ifndef __GRAPHICS_SCANNER_H__
#define __GRAPHICS_SCANNER_H__

#include <list>
#include <string>

#define TOKEN_NUMBER 1
#define TOKEN_STRING 2
#define TOKEN_IDENT 3
#define TOKEN_FUNCTION 4

class Token
{
public:
	int code;
	int i_value;
	std::string text;
};

class Scanner
{
private:
	int m_col;
	int m_line;
	int m_length;
	int m_offset;
	std::string m_text;

private:
	int Read(void);
	int Peek(int count);
	int IsDigit(int code);
	int IsLetter(int code);
	int ParseDigit(Token *token);
	int ParseString(Token *token);
	int ParseOperator(Token *token);
	int ParseCustom(Token *token);
	int ParseToken(Token *token);
	int Skip(void);

public:
	bool Init(const char *text);
	bool GetToken(Token *token);
};

#endif
