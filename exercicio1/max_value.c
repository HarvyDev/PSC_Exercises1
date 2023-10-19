#include <stdio.h>
#include <limits.h>

unsigned long max_value(unsigned nchars) {
    int char_max_bits = CHAR_BIT;
    unsigned long max = 1;
    if (nchars > sizeof(unsigned long) || nchars <= 0) {
        return 0; // O valor não é representável num unsigned long
    }
    for (int i = 1; i < nchars * char_max_bits; i++) {
        max <<= 1;
        if (max == 0) 
            return 0;
    }
    return max - 1;
}