#include <stdio.h>

int main() {
    float num1, num2;

    printf("Ingrese el primer numero: ");
    scanf("%f", &num1);
    printf("Ingrese el segundo numero: ");
    scanf("%f", &num2);

    printf("Suma: %.2f\n", num1 + num2);
    printf("Resta: %.2f\n", num1 - num2);
    printf("Multiplicacion: %.2f\n", num1 * num2);

    if (num2 != 0)
        printf("Division: %.2f\n", num1 / num2);
    else
        printf("Division: No se puede dividir entre cero.\n");

    return 0;
}