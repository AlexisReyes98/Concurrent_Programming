#include <stdio.h>
#include <stdlib.h>

#define MAX_BITS 64

long long suma_binaria(long long a, long long b);
long long bin2dec(int* bin);
void dec2bin(long long a, int* bin);
void printBin(int* bin);
void initBin(int* bin);

int main(int argc, char *argv[])
{

    long a = 10;
    long b = 5;
    long c = suma_binaria(a, b);

    printf("\nLa suma binaria es %ld\n", c);

    return 0;
}


long long suma_binaria(long long a, long long b) {
    // Convertir a y b a binario
    int binA[MAX_BITS];
    int binB[MAX_BITS];
    int binC[MAX_BITS];

    initBin(binA);
    initBin(binB);
    initBin(binC);

    dec2bin(a, binA);
    dec2bin(b, binB);
    
    //printBin(binA);
    //printBin(binB);

    int res = 0;
    int i = 0;
     do {
        int sumaBits = binA[i] + binB[i] + res;
        binC[i] = sumaBits % 2;
        res = sumaBits / 2;
        i++;
    } while (i < MAX_BITS);

    //printBin(binC);

    long long resultado = bin2dec(binC);

    return resultado;
}

void dec2bin(long long a, int* bin) {
    int i = 0;
    while (a != 0 && i < MAX_BITS) {
        long long residuo = a % 2;
        bin[i] = residuo;
        a = a/2;
        i++;
    }
}

long long bin2dec(int* bin) {
    long long potencia = 1;
    long long decimal = 0;

    for (size_t i = 0; i < MAX_BITS; i++)
    {
        decimal = decimal + bin[i]*potencia;
        potencia *= 2;
    }

    return decimal;
}

void printBin(int* bin) {
    printf("\n");
    for (size_t i = 0; i < MAX_BITS; i++)
        printf("%d", bin[i]);
}

void initBin(int* bin) {
    for (size_t i = 0; i < MAX_BITS; i++)
        bin[i] = 0;
}
