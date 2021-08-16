
%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n",s);
    exit(1);
}

// TODO: BE CAREFUL! THE FIRST TIME YOU PUT AN STRING IN STDIN EVERYTHING IS FINE,
//       BUT ONCE YOU PUT ANOTHER ONE, THE PROGRAM CRASHES (Syntax error)
//       WHY?, BECAUSE WE'RE TRYING TO MAKE AN PASSWORD.
//          PASSWORD = anything-\n
//                              ^^ This is the problem. \n is = END
//       As we got END, we actually cannot build another password. so we have to find out a way
//       to exit after matching an password.

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
    : %empty
    | END
    | List END { printf("DONE.\n"); }
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

