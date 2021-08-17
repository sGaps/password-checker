%option noyywrap
%option noinput
%option nounput

%{

/// @file lexer.lex
/// @brief Contiene el analizador léxico del programa.
/// @author Gabriel Peraza. CI: 26929687
/// @version 0.1.0
/// @date 2021-08-17

// Preliminar C Code here
#include <stdio.h>
#include "parser.tab.h"
#include "interactive.h"

/// @brief macro creada para depurar las ocurrencias
///        obtenidas de la entrada estándar.
#define ECHO printf( "MATCH: %s\n", yytext )

// Funciones compartidas:
extern void yyerror( void* );
extern void reset_password ( Password* psw );
extern int  fileno(FILE *__stream) __THROW __wur;

// Globals:
extern YYSTYPE   yylval;

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


