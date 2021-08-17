/// @file password.c
/// @brief Contiene las definiciones de las operaciones Password.
/// @author Gabriel Peraza. CI: 26929687
/// @version 0.1.0
/// @date 2021-08-17

#include <stdio.h>
#include <string.h>
#include "password.h"

// local:
/// @brief Subrutina local. Inserta un caracter en la estructura
///        interna de la contraeña y registra el número de
///        repeticiones consecutivas del caracter.
/// @param psw. Contraseña a modificar.
/// @param c. Caracter de entrada
///
/// @return 1 si fue posible insertar el caracter. 0 en caso contrario.
static int password_priv_push_char( Password* psw , char c );

void password_init( Password* psw ){
    // Reinicia todos los contadores internos
    psw->len       = 0;             // relacionado con body.
    psw->uppercase = 0;
    psw->lowercase = 0;
    psw->special   = 0;
    psw->digit     = 0;

    psw->invalid         = 0;       // relacionado con invalids.
    psw->last_repetition = 0;

    psw->illegal_repetitions = 0;   // relacionado con repeats.
}

void password_from_str( Password* psw , char* str ){
    // Prepara la cadena para insertar caracteres:
    password_init( psw );

    // Inserta los caracteres de str uno a uno.
    char* head;
    for( head = str ; *head != '\0' ; head += 1 )
        password_push_char( psw , *head );

    // Finaliza el proceso de construcción de la contraseña.
    password_commit( psw );
}

void password_commit( Password* psw ){
    // Finaliza el proceso de construcción de la contraseña.
    unsigned short end = (psw->len > PASSWORD_BUFFER-1) ? PASSWORD_BUFFER-1 : psw->len;
    psw->body[end]     = '\0';
}

void password_push_char( Password* psw , char c ){
    // Inserta un caracter según su tipo:
    if     ( 'A' <= c && c <= 'Z' )         password_push_uppercase( psw , c );
    else if( 'a' <= c && c <= 'z' )         password_push_lowercase( psw , c );
    else if( '0' <= c && c <= '9' )         password_push_digit    ( psw , c );
    else if( strchr("=*-_.@&",c) != NULL )  password_push_special  ( psw , c );
    else                                    password_push_invalid  ( psw , c );
}

static int password_priv_push_char( Password* psw , char c ){
    // Finaliza si la capacidad de psw no es suficiente para almacenar c.
    if( psw->len >= PASSWORD_BUFFER-1 ) return 0;
    
    // Actualiza el contador de repeticiones:
    if( psw->len > 0 && psw->body[psw->len-1] == c ) {
        psw->last_repetition += 1;

        // Registra cuando ocurre una repetición ilegal.
        if( psw->last_repetition > REPEAT_LIMIT ) {             // Detecta una repetición ilegal.
            psw->repeats[psw->illegal_repetitions] = psw->len;  // registra la posicion donde ocurre. (un índice)
            psw->illegal_repetitions += 1;                      // actualiza el número de repeticiones ilegales.
        }
    } else {
        // Reestablece la cadena de entrada:
        psw->last_repetition = 1;
    }

    // Reestablece la cadena de entrada:
    psw->body[psw->len] = c;
    psw->len           += 1;
    return 1;
}

// Documentadas en `src/types/password.h`
int password_too_long     ( Password* psw ){ return psw->len > PASSWORD_MAX_LEN; }
int password_too_short    ( Password* psw ){ return psw->len < PASSWORD_MIN_LEN; }
int password_has_uppercase( Password* psw ){ return psw->uppercase > 0; }
int password_has_lowercase( Password* psw ){ return psw->lowercase > 0; }
int password_has_special  ( Password* psw ){ return psw->special   > 0; }
int password_has_digit    ( Password* psw ){ return psw->digit     > 0; }
int password_has_invalid  ( Password* psw ){ return psw->invalid   > 0; }
int password_below_repeat_limit( Password* psw ){ return psw->illegal_repetitions < 1; }

int password_is_valid( Password* psw ){
    // Verifica si cumple con todas las propiedes
    return ( password_below_repeat_limit(psw) &&
             password_has_uppercase     (psw) &&
             password_has_lowercase     (psw) &&
             password_has_special       (psw) &&
             password_has_digit         (psw) &&
             !( password_has_invalid       (psw) ||
                password_too_short         (psw) ||
                password_too_long          (psw) )
           );
}

void password_push_uppercase( Password* psw , char c ){
    if( password_priv_push_char( psw , c ) )
        psw->uppercase += 1;
}

void password_push_lowercase( Password* psw , char c ){
    if( password_priv_push_char( psw , c ) )
        psw->lowercase += 1;
}

void password_push_special( Password* psw , char c ){
    if( password_priv_push_char( psw , c ) )
        psw->special += 1;
}

void password_push_digit  ( Password* psw , char c ){
    if( password_priv_push_char( psw , c ) )
        psw->digit += 1;
}

void password_push_invalid( Password* psw , char c ){
    if( password_priv_push_char( psw , c ) ) {
        psw->special += 1;
        psw->invalids[psw->invalid] = psw->len-1;
        psw->invalid += 1;
    }
}


void password_describe_errors( Password* psw , FILE* f ){
    static const int    depth0  = 4;                // Indentación nivel 0.
    static const int    depth1  = depth0 + depth0;  // Indentación nivel 1.
    static const int    depth2  = depth1 + depth0;  // Indentación nivel 2.
    static const char   space   = ' ';              // Espacio.
    static const char   bar     = '-';              // Marca una región.
    static const char   arrow   = '^';              // Marca un caracter.

    // Verifica si se cumple cada propiedad. Imprime un mensaje si
    // cuando alguna de ellas no se cumple.

    if( password_too_long(psw) ) {
        fprintf( f , "%*c+ Tiene demasiados caracteres! (máximo: %d).\n" 
                               , depth0 , space , PASSWORD_MAX_LEN );
        fprintf( f , "%*c%s\n" , depth1  , space , psw->body );
        fprintf( f , "%*c%*c"  , depth1  , space , PASSWORD_MAX_LEN , space );

        // Subraya la región que debe ser removida:
        int repeat = psw->len - (PASSWORD_MAX_LEN);
        while( repeat-- > 0 ) fputc( bar , f );
        fputc( '\n' , f );

        fprintf( f , "%*c%*csupera el límite permitido.\n" 
                   , depth1  , space , PASSWORD_MAX_LEN+1 , space );
    }

    if( password_too_short(psw) )
        fprintf( f , "%*c- Tiene muy pocos caracteres! (mínimo: %d).\n" , depth0 , space , PASSWORD_MIN_LEN );

    if( !password_has_uppercase(psw) )
        fprintf( f , "%*c- No tiene letras mayúsculas.\n" , depth0 , space );

    if( !password_has_lowercase(psw) )
        fprintf( f , "%*c- No tiene letras minúsculas.\n" , depth0 , space );

    if( !password_has_special  (psw) )
        fprintf( f , "%*c- No tiene caracteres especiales.\n" , depth0 , space );

    if( !password_has_digit(psw) )
        fprintf( f , "%*c- No tiene digitos (números).\n" , depth0 , space );

    if(  password_has_invalid  (psw) ){
        fprintf( f , "%*c+ tiene caracteres inválidos.(%d %s).\n" 
                               , depth0 , space , psw->invalid , psw->invalid < 2 ? "solo" : "de ellos" );
        fprintf( f , "%*c%s\n" , depth1 , space , psw->body );
        fprintf( f , "%*c"     , depth1 , space );

        // Imprime dónde se encuentran los caracteres inválidos
        int count = 0;
        for( int i = 0 ; i < psw->len ; i += 1 ){
            // Imprime '^' si el caracter es inválido, ' ' si es válido.
            if( count < psw->invalid && i == psw->invalids[count] ){
                fputc( arrow , f );
                // Actualiza el contador de caracteres ilegales. 
                count += 1;
            } else
                fputc( space , f );
        }
        fputc( '\n' , f );
        fprintf( f , "%*cConsidere cambiarlos.\n" , depth2  , space );
    }

    if( !password_below_repeat_limit(psw) ) {
        fprintf( f , "%*c+ Repite más de %d veces un caracter.\n" , depth0 , space , REPEAT_LIMIT );
        fprintf( f , "%*c%s\n" , depth1  , space , psw->body );
        fprintf( f , "%*c"     , depth1  , space );

        // Imprime dónde se encuentran las secuencias repetidas.
        int  count  = 0;
        int  i      = 0;
        int  rindex = 0;
        char found  = '\0';
        // Incrementa el valor de 'i' de forma implícita.
        while( i < psw->len && count < psw->illegal_repetitions ){
            // Obtiene los índices de las repeticiones ilegales.
            rindex = psw->repeats[count];
            found  = psw->body[ rindex ];

            // Descarta los caracters hasta conseguir la región de repetición.
            for( ; i < psw->len && i < rindex ; i += 1 )
                fputc( space , f );

            // Indica dónde están los repetidos al imprimir '^'
            for( ; i < psw->len && found == psw->body[i]; i += 1 )
                fputc( arrow , f );

            // Actualiza el contador de símbolos repetidos
            count += 1;
        }

        fputc( '\n' , f );
        fprintf( f , "%*cConsidere removerlos.\n" , depth2 , space );
    }

}

char* password_str( Password* psw ){ return psw->body; }

