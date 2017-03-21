#include "str.h"

void str_trim_crlf( char *str )
{
    char *p = &str[ strlen(str) - 1 ];
    while( *p == '\r' || *p == '\n'){
        *p = '\0';
        p--;
    }
}

void str_split(const char *str, char *left, char *right, char limit)
{
    char *p = strrchr(str, limit);
    if( p == NULL ){
        strcpy(left, str);
    }else{
        strncpy(left, str, p - str);
        strcpy(right, p + 1 );
    }
}

void str_upper( char *str)
{
    char *p = str;
    while( *p != '\0'){
        *p = toupper(*p);
        p++;
    }
}
