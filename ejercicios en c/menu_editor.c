#include <stdio.h>

int main() {
    int menu, opcion;
    int salir = 0;

    while (!salir) {
        printf("\n=== MENUS ===\n");
        printf("1. Archivo\n");
        printf("2. Edicion\n");
        printf("3. Texto\n");
        printf("4. Salir\n");
        printf("Seleccione un menu: ");
        scanf("%d", &menu);

        switch (menu) {
            case 1:
                printf("\n--- MENU ARCHIVO ---\n");
                printf("1. Nuevo\n");
                printf("2. Abrir\n");
                printf("3. Guardar\n");
                printf("4. Cerrar\n");
                printf("5. Salir al menu principal\n");
                printf("Seleccione una opcion: ");
                scanf("%d", &opcion);

                if (opcion >= 1 && opcion <= 4)
                    printf("Se ha seleccionado la opcion ");
                switch (opcion) {
                    case 1: printf("Nuevo del menu Archivo.\n"); break;
                    case 2: printf("Abrir del menu Archivo.\n"); break;
                    case 3: printf("Guardar del menu Archivo.\n"); break;
                    case 4: printf("Cerrar del menu Archivo.\n"); break;
                    default: break;
                }
                break;

            case 2:
                printf("\n--- MENU EDICION ---\n");
                printf("1. Deshacer\n");
                printf("2. Cortar\n");
                printf("3. Copiar\n");
                printf("4. Pegar\n");
                printf("5. Salir al menu principal\n");
                printf("Seleccione una opcion: ");
                scanf("%d", &opcion);

                if (opcion >= 1 && opcion <= 4)
                    printf("Se ha seleccionado la opcion ");
                switch (opcion) {
                    case 1: printf("Deshacer del menu Edicion.\n"); break;
                    case 2: printf("Cortar del menu Edicion.\n"); break;
                    case 3: printf("Copiar del menu Edicion.\n"); break;
                    case 4: printf("Pegar del menu Edicion.\n"); break;
                    default: break;
                }
                break;

            case 3:
                printf("\n--- MENU TEXTO ---\n");
                printf("1. Menor\n");
                printf("2. Medio\n");
                printf("3. Mayor\n");
                printf("4. Salir al menu principal\n");
                printf("Seleccione una opcion: ");
                scanf("%d", &opcion);

                if (opcion >= 1 && opcion <= 3)
                    printf("Se ha seleccionado la opcion ");
                switch (opcion) {
                    case 1: printf("Menor del menu Texto.\n"); break;
                    case 2: printf("Medio del menu Texto.\n"); break;
                    case 3: printf("Mayor del menu Texto.\n"); break;
                    default: break;
                }
                break;

            case 4:
                salir = 1;
                printf("Saliendo del programa...\n");
                break;

            default:
                printf("Opcion no valida.\n");
        }
    }

    return 0;
}
