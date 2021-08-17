#include <stdio.h>
#include "interactive.h"
#include "types/password.h"

char* PROMPT = "Ingrese su contraseña:\n    ";

void reset_password( Password* psw ){
    password_init( psw );
}

void password_resume( Password* psw ){
    if( password_is_valid(psw) ) {
        // Indica que la cadena es válida con formato:
        printf( "\n" );
        printf( "    * La contraseña ingresada ( %s ) es válida.\n\n" , password_str(psw) );
    }
    else {
        // Muestra los errores con formato:
        fprintf( stderr , "\n    . Contraseña: %s\n\n" , password_str(psw) );

        password_describe_errors( psw , stderr );
        fprintf( stderr , "\n" );
    }
}

void report_and_prepare( Password* psw ){
    password_commit(psw);   // Finaliza la escritura de la cadena.
    password_resume(psw);   // Muestra los resultados de su validez.
    reset_password (psw);   // Reinicia la cadena para el siguiente ciclo.
    printf( PROMPT );       // Muestra el prompt
}

#undef PROMPT
