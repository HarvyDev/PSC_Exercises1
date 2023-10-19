#include <stdio.h>
#include <string.h>

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon; // 0-11
    int tm_year; // year - 1900
    int tm_wday; int tm_yday; int tm_isdst; 
};

int string_to_time(const char *string, struct tm *tm) {
    
    // Se a string não tiver 18 caracters quer dizer que está mal formatada.
    if (strlen(string) != 18) 
        return 0;

    // Extraímos valores formatados da string e colocamos nas devidas variáveis. Caso o retorno desta função seja diferente de 6, retornamos 0, 
    // pois isto quer dizer que não lemos 6 items da string, o que quer dizer que esta está mal formatada. 
    if (sscanf(string, "%d-%d-%d %d:%d:%d", &tm->tm_mday, &tm->tm_mon, &tm->tm_year, &tm->tm_hour, &tm->tm_min, &tm->tm_sec) != 6)  
        return 0; 


    // Ajustar o campo tm_mon para acertar com a convenção de que o mês vai de 0..11
    tm->tm_mon -= 1; 
    tm->tm_year -= 1900;

    // Colocar a 0 os campos referidos no enunciado
    tm->tm_wday = 0;
    tm->tm_yday = 0;
    tm->tm_isdst = 0;

    // Retornamos 1 para indicar que a string estava bem formatada
    return 1;
}