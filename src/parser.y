%{

/// @file parser.y
/// @brief Contiene la gramática del parser.
/// @author Gabriel Peraza. CI: 26929687
/// @version 0.1.0
/// @date 2021-08-17

#include <stdio.h>
#include <stdlib.h>
#include "types/password.h"
#include "interactive.h"


/// @brief valor de retorno de la variable global `yylval`.
#define YYSTYPE char

/// @brief Analizador léxico
/// @return Valor del TOKEN obtenido.
extern int yylex();

/// @brief Referencia a la contraseña. Es definida en `src-bin/main.c`.
extern Password* global_psw;

// funciones de `interactive.h`
extern void reset_password    ( Password* psw );
extern void report_and_prepare( Password* psw );
extern void password_resume   ( Password* psw );

/// @brief Reporta los errores de sintaxis.
/// @param s. mensaje a escribir antes de salir con errores.
void yyerror( const char* s );

%}

%token NOPERM
%token UPPER
%token LOWER
%token SPECIAL
%token DIGIT
%token END

%start Password

%%

Password
    : Password END      { report_and_prepare(global_psw);}
    | Password List END { report_and_prepare(global_psw);}
    |
    ;

List
    : Good
    | Bad
    | List Good
    | List Bad
    ;

Bad
    : NOPERM            { password_push_invalid  (global_psw,$1); }
    ;

Good
    : UPPER             { password_push_uppercase(global_psw,$1); }
    | LOWER             { password_push_lowercase(global_psw,$1); }
    | SPECIAL           { password_push_special  (global_psw,$1); }
    | DIGIT             { password_push_digit    (global_psw,$1); }
    ;

%%


void yyerror( const char* s ) {
    fprintf( stderr , "Error de sitaxis: %s\n" , s );
    exit( 1 );
}

