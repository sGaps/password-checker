#include <stdio.h>
#include "interactive.h"
#include "types/password.h"

char* PROMPT = "Ingrese su contraseña:\n    ";

void reset_password( Password* psw ){
    password_init( psw );
}

void password_resume( Password* psw ){
    if( password_is_valid(psw) ) {
        printf( "\n" );
        printf( "    * La contraseña ingresada ( %s ) es válida.\n\n" , password_str(psw) );
    }
    else {
        fprintf( stderr , "\n" );
        fprintf( stderr , "    . Contraseña: %s\n" , password_str(psw) );
        fprintf( stderr , "\n" );

        password_describe_errors( psw , stderr );
        fprintf( stderr , "\n" );
    }
}

void report_and_prepare( Password* psw ){
    password_commit(psw);
    password_resume(psw);
    reset_password (psw); 
    printf( PROMPT );
}

#undef PROMPT
