#include <stdio.h>
#include <string.h>

#define MAX_ESTUDIANTES 100
#define LONG_NOMBRE 50

// Estructura para almacenar los datos de un estudiante
struct Estudiante {
    char nombre[LONG_NOMBRE];
    int codigo;
    float nota;
};

// Función para ingresar estudiantes
int ingresarEstudiantes(struct Estudiante lista[]) {
    int n;
    printf("Ingrese la cantidad de estudiantes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("\n--- Estudiante %d ---\n", i + 1);
        printf("Nombre: ");
        scanf(" %[^\n]", lista[i].nombre);  // Leer cadena con espacios
        printf("Codigo: ");
        scanf("%d", &lista[i].codigo);
        printf("Nota: ");
        scanf("%f", &lista[i].nota);
    }
    return n;
}

// Mostrar lista de aprobados o no aprobados
void mostrarLista(struct Estudiante lista[], int n, int aprobados) {
    printf("\nEstudiantes %s:\n", aprobados ? "APROBADOS" : "NO APROBADOS");
    for (int i = 0; i < n; i++) {
        if ((aprobados && lista[i].nota >= 3.0) || (!aprobados && lista[i].nota < 3.0)) {
            printf("Nombre: %s | Codigo: %d | Nota: %.2f\n",
                   lista[i].nombre, lista[i].codigo, lista[i].nota);
        }
    }
}

// Calcular y mostrar porcentajes
void mostrarPorcentajes(struct Estudiante lista[], int n) {
    int contAprob = 0;
    for (int i = 0; i < n; i++) {
        if (lista[i].nota >= 3.0)
            contAprob++;
    }
    float porcentajeAprob = (contAprob * 100.0) / n;
    float porcentajeNoAprob = 100.0 - porcentajeAprob;

    printf("\nPorcentaje de estudiantes aprobados: %.2f%%\n", porcentajeAprob);
    printf("Porcentaje de estudiantes no aprobados: %.2f%%\n", porcentajeNoAprob);
}

// Buscar estudiante por nombre
void buscarPorNombre(struct Estudiante lista[], int n) {
    char nombreBuscado[LONG_NOMBRE];
    int encontrado = 0;

    printf("\nIngrese el nombre del estudiante a buscar: ");
    scanf(" %[^\n]", nombreBuscado);

    for (int i = 0; i < n; i++) {
        if (strcmp(lista[i].nombre, nombreBuscado) == 0) {
            printf("\nEstudiante encontrado:\n");
            printf("Nombre: %s | Codigo: %d | Nota: %.2f\n",
                   lista[i].nombre, lista[i].codigo, lista[i].nota);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
        printf("\nNo se encontro ningun estudiante con ese nombre.\n");
}

int main() {
    struct Estudiante lista[MAX_ESTUDIANTES];
    int n;

    // Ingreso de estudiantes
    n = ingresarEstudiantes(lista);

    // Mostrar resultados
    mostrarPorcentajes(lista, n);
    mostrarLista(lista, n, 1);  // Aprobados
    mostrarLista(lista, n, 0);  // No aprobados
    buscarPorNombre(lista, n);

    return 0;
}
