#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "encriptador.h"

int validarArchivo(const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo != NULL) {
        fclose(archivo);
        return 1;
    }
    return 0;
}

void encriptarArchivo() {
    char nombreOriginal[80], nombreEncriptado[80];
    FILE *archivoOriginal, *archivoEncriptado, *archivoResumen;
    int tipo_desplazamiento, desplazamiento, clave_xor;
    char caracter;

    printf("Ingrese el nombre del archivo que desea encriptar: ");
    scanf("%79s", nombreOriginal);

    if (validarArchivo(nombreOriginal) == 0) {
        printf("El archivo '%s' no existe.\n", nombreOriginal);
        return;
    }

    sprintf(nombreEncriptado, "Enc-%s", nombreOriginal);

    archivoOriginal = fopen(nombreOriginal, "r");
    if (archivoOriginal == NULL) {
        printf("No se pudo abrir el archivo '%s'.\n", nombreOriginal);
        return;
    }

    archivoEncriptado = fopen(nombreEncriptado, "w");
    if (archivoEncriptado == NULL) {
        printf("No se pudo crear el archivo '%s'.\n", nombreEncriptado);
        fclose(archivoOriginal);
        return;
    }

    archivoResumen = fopen("resumen.txt", "a");
    if (archivoResumen == NULL) {
        printf("No se pudo abrir o crear el archivo 'resumen.txt'.\n");
        fclose(archivoOriginal);
        fclose(archivoEncriptado);
        return;
    }

    srand(time(NULL));
    tipo_desplazamiento = rand() % 2;
    desplazamiento = 1 + rand() % 7;
    clave_xor = rand() % 2;

    while ((caracter = fgetc(archivoOriginal)) != EOF) {
        if (tipo_desplazamiento == 1) {
            caracter = (caracter << desplazamiento) | (caracter >> (8 - desplazamiento));
        } else {
            caracter = (caracter >> desplazamiento) | (caracter << (8 - desplazamiento));
        }
        caracter = caracter ^ clave_xor;
        fputc(caracter, archivoEncriptado);
    }

    fprintf(archivoResumen, "%s-%d-%d-%d\n", nombreEncriptado, tipo_desplazamiento, desplazamiento, clave_xor);

    fclose(archivoOriginal);
    fclose(archivoEncriptado);
    fclose(archivoResumen);

    printf("El archivo ha sido encriptado como '%s'.\n", nombreEncriptado);
}


void desencriptarArchivo() {
    char nombreEncriptado[80], leerLine[250], resumen[150], nombreDesencriptado[100];
    FILE *archivoOriginal, *archivoEncriptado, *archivoResumen;
    int tipo_desplazamiento, desplazamiento, clave_xor, encontrado = 0;
    char caracter;

    printf("Ingrese el nombre del archivo que desea desencriptar: ");
    scanf("%79s", nombreEncriptado);

    if (validarArchivo(nombreEncriptado) == 0) {
        printf("El archivo '%s' no existe.\n", nombreEncriptado);
        return;
    }

    archivoResumen = fopen("resumen.txt", "r");
    if (archivoResumen == NULL) {
        printf("No se pudo abrir el archivo 'resumen.txt'.\n");
        return;
    }

    char nombreOriginal[80];
    while (fgets(leerLine, sizeof(leerLine), archivoResumen) && encontrado == 0) {

	int num_campos = sscanf(leerLine, "%[^-]-%[^-]-%d-%d-%d", resumen, nombreOriginal, &tipo_desplazamiento, &desplazamiento, &clave_xor);

    	if (num_campos == 5) {
        // Concatenar el prefijo "Enc-" con el nombre original
       	    sprintf(resumen, "%s-%s", resumen, nombreOriginal);

            printf("Comparando '%s' con '%s'\n", resumen, nombreEncriptado);

            if (strcmp(resumen, nombreEncriptado) == 0) {
                encontrado = 1;
             }
        } else {
        printf("Formato incorrecto en el archivo 'resumen.txt': %s", leerLine);
        }
    }

    fclose(archivoResumen);

    if (!encontrado) {
        printf("No se encontraron datos para desencriptar '%s' en 'resumen.txt'.\n", nombreEncriptado);
        return;
    }

    sprintf(nombreDesencriptado, "Des-%s", nombreEncriptado + 4);

    archivoEncriptado = fopen(nombreEncriptado, "r");
    if (archivoEncriptado == NULL) {
        printf("No se pudo abrir el archivo '%s'.\n", nombreEncriptado);
        return;
    }

    archivoOriginal = fopen(nombreDesencriptado, "w");
    if (archivoOriginal == NULL) {
        printf("No se pudo crear el archivo '%s'.\n", nombreDesencriptado);
        fclose(archivoEncriptado);
        return;
    }

    while ((caracter = fgetc(archivoEncriptado)) != EOF) {
        caracter = caracter ^ clave_xor;
        if (tipo_desplazamiento == 1) {
            caracter = (caracter >> desplazamiento) | (caracter << (8 - desplazamiento));
        } else {
            caracter = (caracter << desplazamiento) | (caracter >> (8 - desplazamiento));
        }
        fputc(caracter, archivoOriginal);
    }

    fclose(archivoEncriptado);
    fclose(archivoOriginal);

    printf("El archivo ha sido desencriptado como '%s'.\n", nombreDesencriptado);
}
