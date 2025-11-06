#include <stdio.h>

int main() {
    float nota;

    printf("Ingrese la nota de la asignatura: ");
    scanf("%f", &nota);

    while (nota < 0.0 || nota > 5.0) {
        printf("ERROR: Nota incorrecta, debe ser >= 0.0 y <= 5.0\n");
        printf("Ingrese nuevamente la nota: ");
        scanf("%f", &nota);
    }

    if (nota >= 3.0)
        printf("APROBADO\n");
    else
        printf("SUSPENDIDO\n");

    return 0;
}
