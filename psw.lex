%option noyywrap

%{
// Preliminar C Code here
//#include <prelude.h>
#include <stdio.h>
#include "psw.tab.h"
#define ECHO printf( "GOT: %s\n", yytext )
extern void yyerror( void* );

%}

upper   [A-Z]
lower   [a-z]
digit   [0-9]
special [\=\*\-\_\.\@\&]
white   [ \t]
noperm  [^A-Za-z0-9\=\*\-\_\.\@\&\n]|{white}
end     [\n]

%%

{noperm}    { printf("NOP: %s\n",yytext); return NOPERM;   }
{upper}     { printf("UPP: %s\n",yytext); return UPPER;    }
{lower}     { printf("LOW: %s\n",yytext); return LOWER;    }
{special}   { printf("SPC: %s\n",yytext); return SPECIAL;  }
{digit}     { printf("DIG: %s\n",yytext); return DIGIT;    }
{end}       { printf("END: %s\n",yytext); return END;      }
<<EOF>>     { yyterminate();    }

%%

// Extra C code


