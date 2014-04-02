#ifndef __GRAPHICS_PARSER_H__
#define __GRAPHICS_PARSER_H__

#include <list>
#include <string>

class Parser : public Scanner
{
private:
	Token m_temp;

private:
	bool ParseScan();
	bool ParseExpect(int type);
	bool ParseTypeDeclaration();
	bool IsToken(int type);

public:
	bool Parse(const char *text);
};

#endif
