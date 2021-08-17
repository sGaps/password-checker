%option noyywrap

%{

// Preliminar C Code here
#include <stdio.h>
#include "psw.tab.h"
#include "password.h"

#define ECHO printf( "MATCH: %s\n", yytext )

// Shared functions:
extern void yyerror( void* );
extern void reset_password ( Password* psw );
extern void password_resume( Password* psw );

// Globals:
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

{noperm}    { yylval = yytext[0]; return NOPERM;   }
{upper}     { yylval = yytext[0]; return UPPER;    }
{lower}     { yylval = yytext[0]; return LOWER;    }
{special}   { yylval = yytext[0]; return SPECIAL;  }
{digit}     { yylval = yytext[0]; return DIGIT;    }
{end}       { yylval = yytext[0]; return END;      }
<<EOF>>     { yyterminate();    }

%%

// Extra C code


