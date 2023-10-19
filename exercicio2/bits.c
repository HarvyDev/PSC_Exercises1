#include <stdio.h>
#include <limits.h>

#define ULONG_BIT (sizeof(unsigned long) * CHAR_BIT)

// {.data = { 0xBFFFFFECABCD1234, 0xC, 2, 3 }, .index = 60, .length = 8, .result_value = 0xcb}

unsigned long getbits(unsigned long data[], unsigned index, unsigned length) { 
    
    // Calcular o índice do array data, que tem o unsigned long desejado
    unsigned long arrayIdx = index / ULONG_BIT; 
    
    // Calcular a posição do primeiro bit desejado
    unsigned long bitIdx = index % ULONG_BIT;


    if (bitIdx + length > 64) {

		unsigned long mask1 = (1 << (ULONG_BIT - index)) - 1;
		unsigned long mask2 = (1 << (length - (ULONG_BIT - index))) - 1;

		unsigned long shiftedData1 = data[arrayIdx] >> bitIdx;
		unsigned long shiftedData2 = data[++arrayIdx];

		unsigned long data1 = (shiftedData1 & mask1);
		unsigned long data2 = (shiftedData2 & mask2) << (ULONG_BIT - index);

		
		return data1 + data2;
	}



    // Colocar os bits desejados nos bits de menor valor 
    unsigned long shiftedData = data[arrayIdx] >> bitIdx;

    unsigned long mask;

    // Máscara dos bits desejados para realizarmos a operação lógica AND
    if (length == 64) // Caso length seja 64, colocamos todos os bits a 1 pois se shiftarmos 64 vezes, vamos ultrapassar o máximo de memória alocado para esta variável  
        mask = -1LL; 
    else 
        mask = (1 << length) - 1; // Caso contrário shiftamos os bits normalmente    

    // Retorna apenas a 1 os bits desejados e os restantes a 0
    return shiftedData & mask;
}

void setbits(unsigned long data[], unsigned index, unsigned length, unsigned long value) {
    
    // Índice da unsigned long no vetor de bits.
    unsigned long arrayIndex = index / ULONG_BIT;
    
    // Índice do bit dentro da unsigned long.
    unsigned long bitIndex = index % ULONG_BIT;


    if (index + length > 64) {

        // Máscara do primeiro índice da data
    	unsigned long sizeMask1 = ULONG_BIT - index;
    	unsigned long mask1 = ~(((1UL << sizeMask1) - 1) << bitIndex); // Colocar apenas bits a 0 nos bits que pretendemos alterar

    	// Máscara do segundo índice a ser considerado da data
    	// unsigned long sizeMask2 = length - sizeMask1;
    	unsigned long mask2 = ~((1UL << (length - sizeMask1)) - 1);

        // And lógico bit a bit com a máscara para colocar os bits desejados a 0
    	data[arrayIndex] &= mask1;
        // Or lógico bit a bit com o value para colocar o valor desejado nos bits desejados
    	
        data[arrayIndex] |= ((value & ((1UL << sizeMask1) -1)) << bitIndex);

        // O mesmo para o segundo elemento de data
    	arrayIndex += 1;
    	data[arrayIndex] &= mask2;
    	data[arrayIndex] |= (value & ((1UL << (length - sizeMask1)) - 1));
    } 
    else {
        // Máscara para preservar os bits fora da região alvo.
        unsigned long preserveMask = ~(((1UL << length) - 1) << bitIndex);

        // Limpa os bits alvo.
        data[arrayIndex] &= preserveMask;

        // Insere os bits de menor peso de 'value' na posição correta.
        data[arrayIndex] |= (value & ((1UL << length) - 1)) << bitIndex;
    }    
}