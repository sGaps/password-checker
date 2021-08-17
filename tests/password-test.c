#include <stdio.h>
#include <assert.h>
#include "../src/types/password.h"

#define PRINT(msg) fprintf( stderr , msg )

void property_not_all_chars( Password* p );
void property_bad_length   ( Password* p );
void property_valid_str    ( Password* p );
void property_bad_repeat   ( Password* p );
void property_invalid_char ( Password* p );
void property_so_long      ( Password* p );

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

    assert(  password_too_long ( p ) ); // @ makes it too long
    assert( !password_too_short( p ) );

    password_from_str( p , "short" );
    assert( !password_too_long ( p ) );
    assert(  password_too_short( p ) );
    PRINT( "Test - bad length: OK\n" );
}

void property_valid_str( Password* p ){
    password_from_str( p , "12345678Aa-" );
    assert(  password_is_valid( p ) ); // @ makes it too long

    password_from_str( p , "1@@2aB**" );
    assert(  password_is_valid( p ) ); // @ makes it too long

    PRINT( "Test - valid string: OK\n" );
}

void property_invalid_char ( Password* p ){
    password_from_str( p , "Hey!!=ñ<>\'~uº¿¡á" );
    assert( password_has_invalid(p) );
    PRINT( "Test - Invalid Chars: OK\n" );
}

void property_bad_repeat( Password* p ){
    password_from_str( p , "---abcde12C" );
    assert( !password_below_repeat_limit( p ) ); // @ makes it too long

    password_from_str( p , "@123456777-" );
    assert( !password_below_repeat_limit( p ) ); // @ makes it too long

    password_from_str( p , "@@@***AAA!! " );
    assert( !password_below_repeat_limit( p ) ); // @ makes it too long
    PRINT( "Test - So many repetitions: OK\n" );
}

void property_so_long( Password* p ){
    password_from_str( p , "1234567890ABCDEF-*@._1234567890ABCDEF-*@" );
    assert( !password_is_valid( p ) );
    PRINT( "Test - So long: OK\n" );
}

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

    // Must Show errors:
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

#undef PRINT
