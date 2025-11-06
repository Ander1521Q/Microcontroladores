#include <stdio.h>

// Intercambiar valores de dos variables
void intercambiar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Determinar si un número es par o impar
void par_o_impar(int n) {
    if (n % 2 == 0)
        printf("%d es PAR\n", n);
    else
        printf("%d es IMPAR\n", n);
}

// Determinar si un año es bisiesto o normal
void bisiesto_o_normal(int anio) {
    if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0))
        printf("El año %d es BISIESTO\n", anio);
    else
        printf("El año %d es NORMAL\n", anio);
}

// Calcular factorial de un número
long long factorial(int n) {
    long long fact = 1;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

// Función principal para probar las demás
int main() {
    int a, b, numero, anio;

    // Intercambio de variables
    printf("Ingrese dos numeros para intercambiar:\n");
    scanf("%d %d", &a, &b);
    printf("Antes del intercambio: a = %d, b = %d\n", a, b);
    intercambiar(&a, &b);
    printf("Despues del intercambio: a = %d, b = %d\n\n", a, b);

    // Par o impar
    printf("Ingrese un numero para saber si es par o impar: ");
    scanf("%d", &numero);
    par_o_impar(numero);
    printf("\n");

    // Año bisiesto o normal
    printf("Ingrese un año: ");
    scanf("%d", &anio);
    bisiesto_o_normal(anio);
    printf("\n");

    // Factorial
    printf("Ingrese un numero para calcular su factorial: ");
    scanf("%d", &numero);
    printf("El factorial de %d es: %lld\n", numero, factorial(numero));

    return 0;
}
