
%{
#include <stdio.h>
#include <stdlib.h>
#include "password.h"

extern int yylex();

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n",s);
    exit(1);
}

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
    : Password END      { printf("JUST EMPTY.\n"); }
    | Password List END { printf("DONE.\n"); /*End here, then reset*/ }
    |
    ;

List
    : Good
    | Bad
    | List Good
    | List Bad
    ;

Bad
    : NOPERM

Good
    : UPPER
    | LOWER
    | SPECIAL
    | DIGIT
    ;

%%

int main(){
    yyparse();
    return 0;
}

