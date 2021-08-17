/// @file interactive.h
/// @brief Contiene utilidades para la ejecución interactiva del programa.
/// @author Gabriel Peraza. CI: 26929687
/// @version 0.1.0
/// @date 2021-08-17

#ifndef __INTERACTIVE_H__
#define __INTERACTIVE_H__

#include <stdio.h>
#include "types/password.h"

/// @brief Mensaje de la línea de comandos.
extern char* PROMPT;

/// @brief reinicia el valor de la contraseña en tiempo de ejecución.
/// @param psw. Contraseña a reiniciar.
void reset_password    ( Password* psw );

/// @brief reporta la validez de la contraseña (valida/invalida),
///        la reinicia e imprime el PROMPT.
/// @param psw. Contraseña a reiniciar.
void report_and_prepare( Password* psw );

/// @brief imprime en pantalla si la contraseña es correcta o
///        muestra sus errores en stderr.
/// @param psw. Contraseña a evaluar.
void password_resume   ( Password* psw );

#endif
