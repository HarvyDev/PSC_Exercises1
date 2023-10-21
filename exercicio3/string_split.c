#include <stdio.h>
#include <string.h>

size_t string_split(char *text, char *separators, char *words[], size_t words_size) {
    
    // Contador de palavras na string
    size_t count = 0;
    // Variável que vai representar as substrings separadas pelo separador, static para preservar o seu valor mesmo fora do seu scope
    static char *token;
    
    if (text != NULL) {
        // Se o text não for null quer dizer que é uma nova string, logo começamos a dividir a string pelos separadores
        token = strtok(text, separators);
    }

    // Repetir o processo para todas as substrings 
    while (token != NULL && count < words_size) {
        
        // Adicionar a string ao array
        words[count] = token;
        // Incrementar o índice do array
        count++;

        // Passamos à próxima palavra
        token = strtok(NULL, separators);
    }
    
    // Retornar o número de palavras contadas
    return count;
}
