#include "encriptador.h"

int main() {
    int opcion;

    do {
        printf("\n--- Menu de opciones ---\n");
        printf("1. Encriptar un archivo\n");
        printf("2. Desencriptar un archivo\n");
        printf("3. Salir\n");
        printf("Ingrese una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                encriptarArchivo();
                break;
            case 2:
                desencriptarArchivo();
                break;
            case 3:
                printf("Salio del programa con exito!!\n");
                break;
            default:
                printf("Opción inválida. Intente de nuevo.\n");
        }
    } while (opcion != 3);

    return 0;
}
