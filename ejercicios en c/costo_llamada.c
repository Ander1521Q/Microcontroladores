#include <stdio.h>

int main() {
    int minutos;
    float costo;

    printf("Ingrese la duracion de la llamada en minutos: ");
    scanf("%d", &minutos);

    costo = minutos * 45.48;

    printf("El costo de la llamada es: %.2f pesos\n", costo);

    return 0;
}
