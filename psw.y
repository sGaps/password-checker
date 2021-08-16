
%{
#include <stdio.h>
#include <stdlib.h>
#include "password.h"
#define YYSTYPE char
extern int yylex();

Password* global_psw;
void reset_password( Password* psw );
void password_resume( Password* psw );
void yyerror(const char* s);

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
    : Password END      { password_commit(global_psw); password_resume(global_psw); reset_password(global_psw); }
    | Password List END { password_commit(global_psw); password_resume(global_psw); reset_password(global_psw); }
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

void reset_password( Password* psw ){
    password_init( psw );
}

void password_resume( Password* psw ){
    if( password_is_valid(psw) )
        printf( "La contraseña ingresada es válida.\n" );
    else {
        printf( "Contraseña: %s\n" , password_str(psw) );
        password_describe_errors( psw , stdout );
    }
}

void yyerror(const char* s) {
    fprintf(stderr, "Error de sitaxis: %s\n",s);
    exit(1);
}
int main(){
    // Set-up:
    Password usr_psw;
    global_psw = &usr_psw;

    // Run:
    reset_password(global_psw);
    yyparse();
    return 0;
}


