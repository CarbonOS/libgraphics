#ifndef __GRAPHICS_COMPILER_H__
#define __GRAPHICS_COMPILER_H__

/* C++ Interface */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct __Token
{
	int col;
	int line;
	int code;
	int i_value;
	const char *text;
	int length;

} IToken;

typedef struct __Scanner
{
	int col;
	int line;
	const char *text;
	int offset;
	int length;

} IScanner;

int ScannerInit(IScanner *scanner, const char *text);

int GetNextToken(IScanner *scanner, IToken *token);

#ifdef __cplusplus
}
#endif

#endif
