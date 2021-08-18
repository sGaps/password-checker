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
#include <string.h> //TODO: Delete later
#include "parser.tab.h"
#include "interactive.h"

/// @brief macro creada para depurar las ocurrencias
///        obtenidas de la entrada estándar.
#define ECHO printf( "(MATCH: %s, LEN: %ld )\n", yytext , strlen(yytext) )

// Funciones compartidas:
extern void yyerror( void* );
extern void reset_password ( Password* psw );
extern int  fileno(FILE *__stream) __THROW __wur;

// Globals:
extern YYSTYPE   yylval;

// Patrones de búsqueda:
// --------------------

// upper:   Concide con Mayúsculas.
// lower:   Concide con Minúsculas.
// digit:   Concide con dígitos.
// special: Concide con caracteres especiales permitidos.
// white:   Coincide con espacios en blanco.
// wide:    Coincide con caracteres multi-bytes no permitidos en una contraseña.
// noperm:  Coincide con los caracteres no permitidos: No es Mayúscula,
//          Ni Minúscula, Ni dígito, pero puede ser espacio en blanco, un caracter multi-byte (wide)
//          y otros =>    ! # $ , / : ˆ ( ) { } "; % + ? < > ∼ º ¿ ¡ á é ı́ ó ú ñ Ñ |
// end:     Coincide con fin de línea.


%}

upper   [A-Z]
lower   [a-z]
digit   [0-9]
special [\=\*\-\_\.\@\&]
white   [ \t]
wide    "á"|"é"|"í"|"ó"|"ú"|"ñ"|"Ñ"|"’"|"‘"|"¿"|"¡"|"º"
noperm  {wide}|{white}|[^A-Za-z0-9\=\*\-\_\.\@\&\n]
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


