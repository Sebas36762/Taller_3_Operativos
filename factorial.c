#include <stdio.h>
#include <stdlib.h>

volatile double dummy = 0;

double factorial(int n) {
    if (n % 50 == 0)
        printf("Calculando factorial(%d)\n", n);

    for (long i = 0; i < 2000000L; i++)
        dummy += i * 0.000001;

    if (n == 0 || n == 1)
        return 1;
    return n * factorial(n - 1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <numero>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    printf("Iniciando calculo de %d!\n", n);
    double resultado = factorial(n);
    printf("Terminado: %d!\n", n);
    return 0;
}
