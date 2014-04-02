#ifndef __GRAPHIC_TOKEN__
#define __GRAPHIC_TOKEN__

/* C++ Interface */
#ifdef __cplusplus
extern "C" {
#endif

/* Token Operations */
#define TOKEN_OP_ADD    0x00
#define TOKEN_OP_SUB    0x01
#define TOKEN_OP_MUL    0x02
#define TOKEN_OP_DIV    0x03

/* TOKEN Object */
/* A 2D point that consists of two */
/* single-precision floating-point values (x, y). */
typedef struct __Token
{
	unsigned op : 8;

} IToken;


#ifdef __cplusplus
}
#endif
#endif
