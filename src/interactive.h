#ifndef __INTERACTIVE_H__
#define __INTERACTIVE_H__

#include <stdio.h>
#include "types/password.h"

extern char* PROMPT;
void reset_password    ( Password* psw );
void report_and_prepare( Password* psw );
void password_resume   ( Password* psw );



#endif
