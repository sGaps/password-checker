#ifndef __PRELUDE_H__
#define __PRELUDE_H__

#define PASSWORD_MAX_LEN 15
#define PASSWORD_BUFFER  20
#define PASSWORD_MIN_LEN 8
#define REPEAT_LIMIT     3

typedef struct password {
    // Info:
    unsigned short  uppercase;
    unsigned short  lowercase;
    unsigned short  special;
    unsigned short  digit;

    unsigned short  invalid;
    unsigned short  last_repetition;
    unsigned short  illegal_repetitions;

    // Structure: (Minimal align issues)
    unsigned short  len;
    unsigned short  repeats [PASSWORD_BUFFER/REPEAT_LIMIT+1]; //Unused: last index.
    unsigned short  invalids[PASSWORD_BUFFER];                //Unused: last index.
    char            body    [PASSWORD_BUFFER];


} Password;

void password_init    ( Password* psw );
void password_from_str( Password* psw , char* str ); // Auto-commit

int password_is_valid     ( Password* psw );
int password_too_long     ( Password* psw );
int password_too_short    ( Password* psw );
int password_has_uppercase( Password* psw );
int password_has_lowercase( Password* psw );
int password_has_special  ( Password* psw );
int password_has_digit    ( Password* psw );
int password_has_invalid  ( Password* psw );
int password_below_repeat_limit( Password* psw );

void password_commit( Password* psw );
void password_push_char     ( Password* psw , char c );
void password_push_uppercase( Password* psw , char c );
void password_push_lowercase( Password* psw , char c );
void password_push_special  ( Password* psw , char c );
void password_push_digit    ( Password* psw , char c );
void password_push_invalid  ( Password* psw , char c );

void password_describe_errors( Password* psw , FILE* f );

char* password_str( Password* psw );

#endif
