#include <stdio.h>
#include <string.h>
#include "password.h"
// local:
static int password_priv_push_char( Password* psw , char c );

void password_init( Password* psw ){
    psw->len       = 0;             // linked with body.
    psw->uppercase = 0;
    psw->lowercase = 0;
    psw->special   = 0;
    psw->digit     = 0;

    psw->invalid         = 0;       // linked with invalids.
    psw->last_repetition = 0;       // last count:

    psw->illegal_repetitions = 0;   // linked with repeats.
}

int password_from_str( Password* psw , char* str ){
    password_init( psw );
    char* head;
    for( head = str ; *head != '\0' ; head += 1 )
        password_push_char( psw , *head );
    password_commit( psw );
}

void password_commit( Password* psw ){
    unsigned short end = (psw->len > PASSWORD_BUFFER-1) ? PASSWORD_BUFFER-1 : psw->len;
    psw->body[end]     = '\0';
}

// returns 1 if all done correctly, 0 otherwise
void password_push_char( Password* psw , char c ){
    if     ( 'A' <= c && c <= 'Z' )         password_push_uppercase( psw , c );
    else if( 'a' <= c && c <= 'z' )         password_push_lowercase( psw , c );
    else if( '0' <= c && c <= '9' )         password_push_digit    ( psw , c );
    else if( strchr("=*-_.@&",c) != NULL )  password_push_special  ( psw , c );
    else                                    password_push_invalid  ( psw , c );
}

// returns 1 if all done correctly, 0 otherwise
static int password_priv_push_char( Password* psw , char c ){
    if( psw->len > PASSWORD_BUFFER-1 ) return 0;
    
    // Updates repetition count:
    if( psw->len > 0 && psw->body[psw->len-1] == c ) {
        psw->last_repetition += 1;

        if( psw->last_repetition == REPEAT_LIMIT ) {            // Detects an illegal repetition.
            psw->repeats[psw->illegal_repetitions] = psw->len;  // register the position where an invalid repetition occurs
            psw->illegal_repetitions += 1;                      // updates the illegal repetition count.
        }
    } else {
        psw->last_repetition = 1;
    }

    psw->body[psw->len] = c;
    psw->len           += 1;
    return 1;
}

int password_too_long     ( Password* psw ){ return psw->len > PASSWORD_MAX_LEN; }
int password_too_short    ( Password* psw ){ return psw->len < PASSWORD_MIN_LEN; }
int password_has_uppercase( Password* psw ){ return psw->uppercase > 0; }
int password_has_lowercase( Password* psw ){ return psw->lowercase > 0; }
int password_has_special  ( Password* psw ){ return psw->special   > 0; }
int password_has_digit    ( Password* psw ){ return psw->digit     > 0; }
int password_has_invalid  ( Password* psw ){ return psw->invalid   > 0; }
int password_below_repeat_limit( Password* psw ){ return psw->illegal_repetitions < 1; }

// returns 0 if invalid;
int password_is_valid( Password* psw ){
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
    static const char   prompt[] = "Password: ";
    static const size_t offset   = sizeof(prompt) / sizeof(char);
    static const size_t spaces   = 4;
    static const char   mark     = '-';
    static const char   mark2    = '^';
    size_t nextlvl = spaces + spaces;

    // Shows the strings like: Password: something
    //fprintf( f , "%s%s" , prompt , psw->body );
    if( password_too_long(psw) ) {
        fprintf( f , "%*c+ Tiene demasiados caracteres! (máximo: %d).\n" , spaces , ' ' , PASSWORD_MAX_LEN );
        fprintf( f , "%*c%s\n" , nextlvl  , ' ' , psw->body );
        fprintf( f , "%*c%*c" , nextlvl  , ' ' , PASSWORD_MAX_LEN , ' ');
        // spotlight:
        int repeat = psw->len - (PASSWORD_MAX_LEN+1);
        while( repeat-- > 0 ) fputc( mark , f );
        fputc( '\n' , f );

        fprintf( f , "%*c%*csupera el límite permitido.\n" , nextlvl  , ' ' , PASSWORD_MAX_LEN+1 , ' ');
    }
    if( password_too_short(psw) )
        fprintf( f , "%*c- Tiene muy pocos caracteres! (mínimo: %d).\n" , spaces , ' ' , PASSWORD_MIN_LEN );

    if( !password_has_uppercase(psw) )
        fprintf( f , "%*c- No tiene letras mayúsculas.\n" , spaces , ' ' );

    if( !password_has_lowercase(psw) )
        fprintf( f , "%*c- No tiene letras minúsculas.\n" , spaces , ' ' );

    if( !password_has_special  (psw) )
        fprintf( f , "%*c- No tiene caracteres especiales.\n" , spaces , ' ' );

    if( !password_has_digit(psw) )
        fprintf( f , "%*c- No tiene digitos (números).\n" , spaces , ' ' );

    if(  password_has_invalid  (psw) ){
        fprintf( f , "%*c+ tiene caracteres inválidos.(%d %s).\n" , spaces , ' ' , psw->invalid , psw->invalid < 2? "solo" : "de ellos" );
        fprintf( f , "%*c%s\n" , nextlvl  , ' ' , psw->body );
        fprintf( f , "%*c" , nextlvl  , ' ' );
        // Prints the invalid sequences
        int count = 0;
        for( int i = 0 ; i < psw->len ; i += 1 ){
            if( count < psw->invalid && i == psw->invalids[count] ){
                fputc( mark2 , f );
                count += 1;
            } else
                fputc( ' ' , f );
        }
        fputc( '\n' , f );
        fprintf( f , "%*cConsidere cambiarlos.\n" , nextlvl+spaces  , ' ' );
    }

    if( !password_below_repeat_limit(psw) ) {
        fprintf( f , "%*c+ Repite más de %d veces un caracter.\n" , spaces , ' ' , REPEAT_LIMIT );
        fprintf( f , "%*c%s\n" , nextlvl  , ' ' , psw->body );
        fprintf( f , "%*c" , nextlvl  , ' ' );

        // Prints the repeated sequences
        int  count  = 0;
        int  i      = 0;
        int  rindex = 0;
        char found  = '\0';
        // 'i's increments are implicit
        while( i < psw->len && count < psw->illegal_repetitions ){
            // Prints each bad repetition:
            rindex = psw->repeats[count];
            found  = psw->body[ rindex ];

            // skip until reach the repetition:
            for( ; i < psw->len && i < rindex ; i += 1 )
                fputc( ' ' , f );

            // marks those repeated:
            for( ; i < psw->len && found == psw->body[i]; i += 1 )
                fputc( mark2 , f );

            count += 1;
        }

        fputc( '\n' , f );
        fprintf( f , "%*cConsidere removerlos.\n" , nextlvl+spaces, ' ' );
    }
}

char* password_str( Password* psw ){ return psw->body; }

