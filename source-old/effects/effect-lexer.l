%{
#include "defines.h"
%}

%option stack
%option noyywrap nounput never-interactive

D			[0-9]
F                       [0-9]+\.[0-9]+
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%%
"/*"			{ /*comment();*/ }

"auto"			{ return(AUTO); }
"effect"		{ return(EFFECT); }

[ \t\v\n\f]		{  }
.			{ /* ignore bad characters */ }

%%
