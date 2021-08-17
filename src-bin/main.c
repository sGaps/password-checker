
#include <stdio.h>
#include <stdlib.h>
#include "../src/types/password.h"
#include "../src/interactive.h"
#include "../src/parser.tab.h"

extern Password* global_psw;
extern char*     PROMPT;

int main(){
    // Set-up:
    Password usr_psw;
    global_psw = &usr_psw;

    // Run:
    reset_password(global_psw);
    printf( PROMPT );
    yyparse();
    printf( "\n\n" );

    return 0;
}

