#include <stdio.h>

int main() {
    int a = 0, b = 1, c;

    printf("Serie de Fibonacci hasta 1000:\n");
    printf("%d, %d", a, b);

    c = a + b;
    while (c <= 1000) {
        printf(", %d", c);
        a = b;
        b = c;
        c = a + b;
    }

    printf("\n");
    return 0;
}