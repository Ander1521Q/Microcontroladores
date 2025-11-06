#include <stdio.h>

int main() {
    float a, b, c;

    printf("Ingrese tres numeros diferentes: ");
    scanf("%f %f %f", &a, &b, &c);

    if (a > b && a > c)
        printf("El mayor es: %.2f\n", a);
    else if (b > a && b > c)
        printf("El mayor es: %.2f\n", b);
    else
        printf("El mayor es: %.2f\n", c);

    return 0;
}