#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <unistd.h> 
#include <string.h>
 
char **arrayLinesWithPattern; // Array das linhas com pattern presente 
int arraySize = 0; // Length deste mesmo array

char **inputFileLines; // Array das linhas presentes no ficheiro de input 
int nLinesInput = 0; // Length deste mesmo array

char *inputFile; // Fichero de input
char *outputFile; // Ficheiro de out
int sensible = 0; // Flag c
int column; // Parâmetro column
char *pattern; // Parâmetro pattern

// Verifica se o ficheiro existe
bool file_exists(char *file) {
    return !(access(file, F_OK) == -1);
}

// Popular o array fileLines com as linhas do ficheiro presente em filePath
char **populateFileLinesArray(char *filePath) {
    
	char **fileLines= (char **)malloc(10 * sizeof(char *));
    int idx = 0; 
    char buffer[1024];

    // Caso o stdin não esteja vazio 
	if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) > 0) {
	  	rewind(stdin); // Voltamos a colocar o cursor no início do ficheiro
	  
        // Lemos linha a linha e populamos o ficheiro
		while ((fgets(buffer, 1024 , stdin)) != NULL) {
            fileLines[idx] = strdup(buffer);
            nLinesInput++;
            idx++;
		}
	} 
    // Caso o stdin esteja vazio vamos popular o array com as linhas do ficheiro presente em filePath
    else { 
        // Abrir o ficheiro em modo read
        FILE *file = fopen(filePath, "r");
        
        // Se o ficheiro for igual a null, quer dizere que houve um erro na sua abertura
        if (file == NULL) {
            printf("Error opening file\n");
            return fileLines;
        }

        // Corremos o ficheiro linha a linha e populamos o array
        while (fgets(buffer, sizeof(buffer), file)) {

            fileLines[idx] = strdup(buffer);
            nLinesInput++;
            idx++;    
        
        }
    }
    // Retornamos o array com as linhas presentes no ficheiro
    return fileLines;
}

bool contains_pattern(char *line, int column, char *pattern, int sensible) {

    char *token;

    // Substring de line
    token = strtok(line, ",");

    // Enquanto substring não for null, ou seja, ainda não tenhamos chegado ao fim do ficheiro, continuamos a comparar com pattern
    while (token != NULL) {
        
        // Caso a flag c esteja desativa
        if (sensible == 0) {
            // Utilizamos a função strcasecmp, que é insensível a uppercases e lowercases
            if ((strcasecmp(token, pattern)) == 0) {
                // Caso a substring seja igual a pattern, retornamos true
                return true;
            }
        }        
        else {
            // Caso contrário utilizamos a função strcmp que é sensível a uppercases e lowercases
            if ((strcmp(token, pattern)) == 0) {
                // Caso a substring seja igual a pattern, retornamos true
                return true;
            }
        }
        token = strtok(NULL, ",");
    }
    // Se ainda não tivermos retornado da função, quer dizer que o pattern não está presente nesta linha, logo retornamos false
    return false;
}


char **return_lines_with_pattern(char **filePath, int column, char *pattern, int sensible) {
    
    // Array que vai armazenar as linhas que têm presente pattern
    char **lines_with_pattern = (char **)malloc(10 *sizeof(char*)); // Alocate memorry dinamically 

    // Corremos as strings presentes array que guarda as linhas do ficheiro
    for (int line = 0; line < nLinesInput; line++) {
        char *to_write = strdup(filePath[line]);

        // Caso a linha contenha o pattern 
        if (contains_pattern(filePath[line], column, pattern, sensible)) {
            // Copiamo-la para o array que guarda as strings com pattern presente
            lines_with_pattern[arraySize] = strdup(to_write);
            // E aumentamos o tamanho do array
            arraySize++;
        }
    } 
    // Por fim, retornamos o array
    return lines_with_pattern;
}

int writeOnOutput(char **arrayLinesWithPattern, char *outputFile) {
    // Abrir o ficheiro de output em modo write. Caso este não exista, é criado pela função fopen.
    FILE *file = fopen(outputFile, "w");    
    // Se o ficheiro for igual a null, quer dizer que houve algum erro na sua abertura e por retornamos                 
    if (file == NULL) {
        for (int i = 0; i < arraySize; i++) {
            printf("%s", arrayLinesWithPattern[i]);
        }
        return 1;
    }
    // Corremos o ficheiro que armazena as linhas onde pattern está presente
    for (int i = 0; i < arraySize; i++) {
        // Escrevemos as linhas presentes no array, no ficheiro de output 
        fprintf(file, "%s", arrayLinesWithPattern[i]);
    }
    // Por fim, fechamos o ficheiro e retornamos 0 da função para indicar que correu tudo bem
    fclose(file);
    return 0;
}



int processOptions(int argc, char **argv) {
    
    // Verificar se foram passadas opções 
    if (argc < 2) {
        return 1;
    }

    // Lemos a variávle de ambiente "CSV_FILTER_PATH"
    const char* input_file_path = getenv("CSV_FILTER_PATH"); 
    // Alocamos memória para a string que vai guardar o path completo do ficheiro 
    // char *inputFile = (char *)malloc(1024 * sizeof(char));
    // Criamos um buffer para armazenar o caminho da diretoria corrente
    char buffer[1024];

    
    // Caso a variável de ambiente não esteja definida 
    if (input_file_path == NULL) {
        // Definimos o caminho para o ficheiro de input como a diretoria corrente
        input_file_path = getcwd(buffer, sizeof(buffer));
    }

    // Corremos os argumentos para encontrarmos tanto a coluna como o pattern
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] >= '1' && argv[i][0]<= '9') {
            column = argv[i][0] - 48;
            pattern = argv[i + 1];
        }
    }

    // Variável que representa as options passadas na chamada da ferramenta
    int option;
    // Buffer do path do ficheiro 
    char filePath[1024];

    while ((option = getopt(argc, argv, "i:o:c")) != -1) {
        switch (option) {
            case 'i':
                // Juntamos a string que contém o path da diretoria ao nome do ficheiro, adicionando uma '/' entre eles
                snprintf(filePath, sizeof(filePath), "%s/%s", input_file_path, optarg); 
                // E verificamos se o ficheiro existe
                if (!file_exists(filePath)) { 
                    return 1;
                }
                // E definimos a variável global inputFile
                // printf("inputFile = %s\n", inputFile);
                inputFile = strdup(filePath);
                // printf("inputFile = %s\n", inputFile);
                break;
            case 'o': 
                // Definimos a variável global outputFile com o nome do ficheiro passado na chamada da ferramenta 
                outputFile = optarg;
                break;
            case 'c':
                // Caso a flag c esteja ativa, colocamos sensible a 1
                sensible = 1;
                break;
            case '?':
                // Caso seja recebida outra opção que não as referidas anteriormente, a função retorna erro
                return 1;               
        }
    }    
    
    return 0;   
}

int csv_filter(int argc, char **argv) {

    // Caso as opções sejam processadas com sucesso
    if (processOptions(argc, argv) == 0) {
        // Lemos as linhas do ficheiro de input
        char **fileLines = populateFileLinesArray(inputFile);
        // Guardamos as linhas que têm pattern presente 
        arrayLinesWithPattern = return_lines_with_pattern(fileLines, column, pattern, sensible);        
        // E escrevemo-las no ficheiro de output 
        writeOnOutput(arrayLinesWithPattern, outputFile);
    } 
    else {
        return 1;
    } 
    return 0;
}

int main(int argc, char **argv) {
    return csv_filter(argc, argv);
}

