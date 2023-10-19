#include <stdio.h>
#include <string.h>
/**
 * Separa em palavras isoladas o texto recebido no parâmetro text com formato string C
 *
 *  O array de ponteiros 'words', cuja dimensão é definida por 'words_size', deve ser preenchido
 *  com os endereços de ínicio das palavras pela ordem que se encontram no texto.
 * 
 * O caractere separador, no final de cada palavra, deve ser substituído por '\0' para que as 
 *  palavras fiquem formatadas como strings C
 * 
 * A função devolve o número de palavras assinaladas em words.
 * 
 * No caso de a função devolver um valor igual a words_size, pode também significar que o número
 *  de palavras no texto é superior à capacidade de words
 * 
 * Nesse caso, as restantes palavras podem ser obtidas em chamadas posteriores. A forma de indicar
 *  que se pretende continuar a obter palavras do texto da chamada anterior é usar o argumento NULL
 *  como primeiro parâmetro. 
 * 
*/

size_t string_split(char *text, char *separators, char *words[], size_t words_size) {
    
    static char *previousText;
    size_t count = 0;
    char *token;
    
    if (text == NULL) {
        if (previousText != NULL) {
            text = previousText;  // Usamos o text anterior, se este existir
        } else {
            return 0;  // Retornamos 0 para indicar que não foram encontradas palavras
        }
    }
    
    // Dividir a string em substrings, pelos separadores
    token = strtok(text, separators);

    // Repetir o processo para todas as substrings 
    while (token != NULL && count < words_size) {
        
        // Adicionar a string ao array
        words[count] = token;
        // Incrementar o índice do array
        count++;

        // Passamos à próxima palavra
        token = strtok(NULL, separators);
    }
    
    
    // Atribuimos o valor text à variável estática previousText para que consigamos aceder ao texto numa chamada posterior
    previousText = text;
        
    
    // Retornar o número de palavras contadas
    return count;
}