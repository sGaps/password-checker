/// @file main.c
/// @brief Archivo que contiene el programa principal.
/// @author Gabriel Peraza
/// @version 0.1.0
/// @date 2021-08-17

#include <stdio.h>
#include <stdlib.h>
#include "../src/types/password.h"
#include "../src/interactive.h"
#include "../src/parser.tab.h"

Password*    global_psw;    ///< Referencia a la contraseña global empleada en `src/parser.y` .
extern char* PROMPT;        ///< Mensaje de la línea de comandos.

/// @brief Función principal del programa. Establece los valores
///        del entorno para su correcto funcionamiento.
int main(){
    // Prepara el entorno:
    Password usr_psw;
    global_psw = &usr_psw;
    reset_password(global_psw);

    // Ejecuta el parser.
    printf( PROMPT );
    yyparse();
    printf( "\n\n" );

    return 0;
}

