%option noyywrap

%{
// Preliminar C Code here
//#include <prelude.h>
#include <stdio.h>
#include "psw.tab.h"
#include "password.h"

#define ECHO printf( "MATCH: %s\n", yytext )
extern void yyerror( void* );
extern void reset_password( Password* );
extern void password_resume( Password* psw );
extern Password* global_psw;

%}

upper   [A-Z]
lower   [a-z]
digit   [0-9]
special [\=\*\-\_\.\@\&]
white   [ \t]
noperm  [^A-Za-z0-9\=\*\-\_\.\@\&\n]|{white}
end     [\n]|[\n\r]|[\r]

%%

{noperm}    { yylval = yytext[0]; printf("NOP(0:%c): %s\n",yytext[0],yytext); return NOPERM;   }
{upper}     { yylval = yytext[0]; printf("UPP(0:%c): %s\n",yytext[0],yytext); return UPPER;    }
{lower}     { yylval = yytext[0]; printf("LOW(0:%c): %s\n",yytext[0],yytext); return LOWER;    }
{special}   { yylval = yytext[0]; printf("SPC(0:%c): %s\n",yytext[0],yytext); return SPECIAL;  }
{digit}     { yylval = yytext[0]; printf("DIG(0:%c): %s\n",yytext[0],yytext); return DIGIT;    }
{end}       { yylval = yytext[0]; printf("END(0:%c): %s\n",yytext[0],yytext); return END;      }
<<EOF>>     { yyterminate();    }

%%

// Extra C code


