#include <stdio.h>

int main() {
    int valor;

    printf("Ingrese el valor de la variable (0-255): ");
    scanf("%d", &valor);

    if (valor > 127)
        printf("LED VERDE encendido\n");
    else if (valor >= 64)
        printf("LED AMARILLO encendido\n");
    else
        printf("LED ROJO encendido\n");

    return 0;
}