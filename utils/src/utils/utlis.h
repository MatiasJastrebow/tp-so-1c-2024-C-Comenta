#ifndef UTILS_H
#define UTILS_H

#include<string.h>
#include<commons/string.h>

/**
 * @fn    string_arr_as_string
 * @brief convierte un array de strings en un string del tipo "[str1,str2,str3]" SI O SI TIENE QUE TERMINAR EN NULL SI NO ANDA
 */
char * string_arr_as_string(char ** string_arr);

#endif