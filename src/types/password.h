/// @file password.h
/// @brief Contiene el tipo de dato Password junto a sus operaciones.
/// @author Gabriel Peraza. CI: 26929687
/// @version 0.1.0
/// @date 2021-08-17

#ifndef __PRELUDE_H__
#define __PRELUDE_H__

/// @brief Máxima longitud permitida por cadena.
#define PASSWORD_MAX_LEN 15

/// @brief Capacidad física de las contraseñas.
#define PASSWORD_BUFFER  30

/// @brief Longitud mínima permitida por cadena.
#define PASSWORD_MIN_LEN 8

/// @brief Número de repeticiones necesarias
///        para que una contreaseña sea inválida.
#define REPEAT_LIMIT     3

#if (PASSWORD_MAX_LEN >= PASSWORD_BUFFER)
#error "valor de PASSWORD_MAX_LEN muy alto. las contraseñas no se podrán almacenar ya que supera el valor asignado para PASSWORD_BUFFER"
#endif

#if (REPEAT_LIMIT < 1)
#error "valor muy bajo REPEAT_LIMIT. los caracters deben aparecer por lo menos una vez!"
#endif



/// @brief Tipo de datos que modela las contraseñas. junto
///        a sus propiedades.
///        **NOTA:** Sólo soporta caracteres ASCII.
typedef struct password {
    // Info:
    unsigned short  uppercase;              ///< número minúsculas en body.
    unsigned short  lowercase;              ///< número lowercase en body.
    unsigned short  special;                ///< número de caracteres especiales en body.
    unsigned short  digit;                  ///< número de dígitos en body.

    unsigned short  invalid;                ///< número de caracteres inválidos en body.
    unsigned short  last_repetition;        ///< último número de repeticiones registradas
                                            ///< para el caracter más recientemente insertado.
    unsigned short  illegal_repetitions;    ///< número de repeticiones ilegales reconocidas.

    // Structure: (Minimal align issues)
    unsigned short  len;                                        ///< Longitud de la cadena.
    unsigned short  repeats [PASSWORD_BUFFER/REPEAT_LIMIT+1];   ///< Contiene los índices donde ocurre una repetición inválida.
    unsigned short  invalids[PASSWORD_BUFFER];                  ///< Contiene los índices donde están los caracteres inválidos.
    char            body    [PASSWORD_BUFFER];                  ///< Contiene la contraseña.

} Password;

/// @brief Inicializa una contraseña y la prepara para las llamadas
///        de la familia `password_push_[..]` y de `password_commit`.
/// @param psw. Constraseña a inicializar.
void password_init    ( Password* psw );

/// @brief Inicializa una contraseña a partir de una cadena.
///        útil para realizar unidades de pruebas.
/// @param psw. Constraseña a inicializar.
/// @param str. Cadena que contiene una posible contraseña.
void password_from_str( Password* psw , char* str ); // Auto-commit

/// @brief verifica si una cadena es válido.
/// @param psw Constraseña de entrada
/// @return 1 si es válida. 0 en caso contrario.
int password_is_valid     ( Password* psw );

/// @brief verifica si la contraseña es muy extensa.
/// @param psw Constraseña de entrada
/// @return 1 si es muy extensa. 0 en caso contrario.
int password_too_long     ( Password* psw );

/// @brief verifica si la contraseña es muy corta.
/// @param psw Constraseña de entrada
/// @return 1 si es muy corta. 0 en caso contrario.
int password_too_short    ( Password* psw );

/// @brief verifica si la contraseña tiene mayúsculas.
/// @param psw Constraseña de entrada
/// @return 1 si tiene alguna letra mayúscula. 0 en caso contrario.
int password_has_uppercase( Password* psw );

/// @brief verifica si la contraseña tiene minúsculas.
/// @param psw Constraseña de entrada
/// @return 1 si tiene alguna letra minúscula. 0 en caso contrario.
int password_has_lowercase( Password* psw );

/// @brief verifica si la contraseña tiene símbolos especiales (permitidos).
/// @param psw Constraseña de entrada
/// @return 1 si tiene algún símbolo especial. 0 en caso contrario.
int password_has_special  ( Password* psw );

/// @brief verifica si la contraseña tiene dígitos.
/// @param psw Constraseña de entrada
/// @return 1 si tiene algún dígito. 0 en caso contrario.
int password_has_digit    ( Password* psw );

/// @brief verifica si la contraseña tiene caracteres inválidos.
/// @param psw Constraseña de entrada
/// @return 1 si tiene alǵun caracter inválido. 0 en caso contrario.
int password_has_invalid  ( Password* psw );

/// @brief verifica si ningún caracter se repite más de `REPEAT_LIMIT` veces consecutivas.
/// @param psw Constraseña de entrada
/// @return 1 si hay caracteres que se repiten más de lo permitido. 0 en caso contrario.
int password_below_repeat_limit( Password* psw );

/// @brief Finaliza la escritura de la contraseña y la convierte internamente en una 
///        Cadena válida del lenguaje C. (termina en '\0').
/// @param psw Contraseña a modificar.
void password_commit( Password* psw );

/// @brief Inserta un caracter en la contraseña.
/// @param psw. Contraseña a modificar.
/// @param c.   Caracter a insertar.
void password_push_char     ( Password* psw , char c );

/// @brief Inserta un caracter y la registra como mayúscula sin verificar su tipo.
/// @param psw. Contraseña a modificar.
/// @param c.   Caracter a insertar.
void password_push_uppercase( Password* psw , char c );

/// @brief Inserta un caracter y la registra como minúscula sin verificar su tipo.
/// @param psw. Cadena a modificar.
/// @param c.   Caracter a insertar.
void password_push_lowercase( Password* psw , char c );

/// @brief Inserta un caracter y la registra como símbolo especial sin verificar su tipo.
/// @param psw. Cadena a modificar.
/// @param c.   Caracter a insertar.
void password_push_special  ( Password* psw , char c );

/// @brief Inserta un caracter y la registra como dígito sin verificar su tipo.
/// @param psw. Cadena a modificar.
/// @param c.   Caracter a insertar.
void password_push_digit    ( Password* psw , char c );

/// @brief Inserta un caracter y la registra como caracter inválido sin verificar su tipo.
/// @param psw. Cadena a modificar.
/// @param c.   Caracter a insertar.
void password_push_invalid  ( Password* psw , char c );

/// @brief Describe los errores de la cadena en la salida estándar.
///        **NOTA**: Sólo puede resaltar los caracteres ASCII. los caracteres anchos
///        (áéíóúñÑ) ocupan 2 bytes cada uno. No asegura que los caracteres ínvalidos
///        sean subrayados, pero se reportará que hay caracteres inválidos.
/// @param psw. Cadena de entrada.
/// @param f.   stream de salida.
void password_describe_errors( Password* psw , FILE* f );

/// @brief Obtiene la cadena almacenada en el tipo de datos Password.
///        **NOTA:** Debe llamar a la función `password_commit` antes
///        de usar ésta función.
/// @param psw Contraseña de entrada
/// @return Cadena que contiene la contraseña.
char* password_str( Password* psw );

#endif
