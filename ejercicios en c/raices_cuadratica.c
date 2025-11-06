#include <stdio.h>
#include <math.h>

int main() {
    float a, b, c, discriminante, x1, x2;

    printf("Ingrese los valores de a, b y c: ");
    scanf("%f %f %f", &a, &b, &c);

    discriminante = b*b - 4*a*c;

    if (discriminante > 0) {
        x1 = (-b + sqrt(discriminante)) / (2*a);
        x2 = (-b - sqrt(discriminante)) / (2*a);
        printf("Raices reales: x1 = %.2f, x2 = %.2f\n", x1, x2);
    } else if (discriminante == 0) {
        x1 = -b / (2*a);
        printf("Una raiz real doble: x = %.2f\n", x1);
    } else {
        float parteReal = -b / (2*a);
        float parteImag = sqrt(-discriminante) / (2*a);
        printf("Raices complejas: x1 = %.2f + %.2fi, x2 = %.2f - %.2fi\n",
               parteReal, parteImag, parteReal, parteImag);
    }

    return 0;
}