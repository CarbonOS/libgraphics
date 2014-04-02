/*

Based on ANSI C Yacc grammar:
http://www.lysator.liu.se/c/ANSI-C-grammar-y.html

IF YOU MODIFY THIS FILE YOU ALSO NEED TO RUN generate_parser.sh,
WHICH GENERATES THE GLSL ES PARSER (glslang_tab.cpp AND glslang_tab.h).
*/

%{
#include "defines.h"
extern int yylex();
extern int yyparse();
void yyerror(const char *s);
%}

%union
{
	int code;
	int ival;
	float fval;
	char *sval;
}

%token AUTO
%token EFFECT
%token IDENTIFIER
%token CONSTANT
%token STRING_LITERAL
%token SIZEOF

%start effect_unit
%%

effect_entry
	: EFFECT '(' ')'
	;

effect_unit
	: effect_entry
	;



%%
