/// @file password-test.c
/// @brief Contiene pruebas para el tipo de datos `Password` definido
///        en `src/types/password.h`
/// @author Gabriel Peraza. CI: 26929687
/// @version 0.1.0
/// @date 2021-08-17

#include <stdio.h>
#include <assert.h>
#include "../src/types/password.h"

/// @brief Utilidad para imprimir mensajes en stderr.
/// @param msg. Cadena que se mostrará en stderr.
#define PRINT(msg) fprintf( stderr , msg )

/// @brief Prueba 1. Muestra las propiedades de la cadena "ABCDEFG"
/// @param p. valor que se sobreescribe durante la prueba.
void property_not_all_chars( Password* p );

/// @brief Prueba 2. Verifica si las contraseñas cumplen con la restricción de longitud.
/// @param p. valor que se sobreescribe durante la prueba.
void property_bad_length   ( Password* p );

/// @brief Prueba 3. Verifica el funcionamiento de la función `password_is_valid(...)`
/// @param p. valor que se sobreescribe durante la prueba.
void property_valid_str    ( Password* p );

/// @brief Prueba 4. Cuenta el número de caracteres inválidos dentro de una contraseña.
/// @param p. valor que se sobreescribe durante la prueba.
void property_invalid_char ( Password* p );

/// @brief Prueba 5. Debe mostrar dónde están los caracteres repetidos.
/// @param p. valor que se sobreescribe durante la prueba.
void property_bad_repeat   ( Password* p );

/// @brief Prueba 6. Preuba la función `password_priv_push_char(...)` con una
//                   entrada extensa.
/// @param p. valor que se sobreescribe durante la prueba.
void property_so_long      ( Password* p );

int main(){
    Password p;
    password_init( &p );
    PRINT( "Password Unit test:\n"
           "-------------------\n" );
    property_not_all_chars( &p );
    property_bad_length   ( &p );
    property_valid_str    ( &p );
    property_invalid_char ( &p );
    property_bad_repeat   ( &p );
    property_so_long      ( &p );

    // Prueba la salida de los errores:
    PRINT( "\nPrint test\n" );
    password_from_str( &p , "-aBC4@@@_|?45cortado ?werj" );
    fprintf( stderr , "psw: %s\n" , p.body );
    password_describe_errors( &p , stderr );

    password_from_str( &p , "-???eh??? this should work." );
    fprintf( stderr , "psw: %s\n" , p.body );
    password_describe_errors( &p , stderr );


    password_from_str( &p , "aaabbbcccEEEEEEEEEEEEEEEEEE" );
    fprintf( stderr , "psw: %s\n" , p.body );
    password_describe_errors( &p , stderr );

    return 0;
}

void property_not_all_chars( Password* p ){
    password_from_str( p , "ABCDEFG" );

    assert( !password_is_valid     ( p ) );
    assert(  password_has_uppercase( p ) );
    assert( !password_has_lowercase( p ) );
    assert(  password_too_short    ( p ) );
    assert( !password_has_special  ( p ) );
    PRINT( "Test - Not all chars: OK\n" );
}

void property_bad_length( Password* p ){
    password_from_str( p , "123456789ABCDEF@" );

    assert(  password_too_long ( p ) ); // '@' hace la cadena muy extensa.
    assert( !password_too_short( p ) );

    password_from_str( p , "short" );
    assert( !password_too_long ( p ) );
    assert(  password_too_short( p ) );
    PRINT( "Test - bad length: OK\n" );
}

void property_valid_str( Password* p ){
    password_from_str( p , "12345678Aa-" );
    assert(  password_is_valid( p ) );

    password_from_str( p , "1@@2aB**" );
    assert(  password_is_valid( p ) );

    PRINT( "Test - valid string: OK\n" );
}

void property_invalid_char ( Password* p ){
    password_from_str( p , "Hey!!=ñ<>\'~uº¿¡á" );
    assert( password_has_invalid(p) );
    PRINT( "Test - Invalid Chars: OK\n" );
}

void property_bad_repeat( Password* p ){
    password_from_str( p , "----abcde12C" );
    assert( !password_below_repeat_limit( p ) ); // debe detectar el tercer -

    password_from_str( p , "@1234567777-" );
    assert( !password_below_repeat_limit( p ) ); // debe detectar el tercer 7

    password_from_str( p , "@@@****AAA!! " );
    assert( !password_below_repeat_limit( p ) ); // debe detectar el tercer @
    PRINT( "Test - So many repetitions: OK\n" );
}

void property_so_long( Password* p ){
    password_from_str( p , "1234567890ABCDEF-*@._1234567890ABCDEF-*@" );
    assert( !password_is_valid( p ) );
    PRINT( "Test - So long: OK\n" );
}


#undef PRINT
