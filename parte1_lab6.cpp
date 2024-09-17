/*
*-----------------------------------------------------------
* parte1_lab6.c
*-----------------------------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERÍA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACIÓN
*
* CC3086 - Programación de Microprocesadores
*
*-----------------------------------------------------------
* Descripción: 
* Este programa utiliza Pthreads en C para calcular y listar
* los números de la serie Fibonacci entre 0 y un número dado 
* por el usuario (entre 0 y 100). La suma de todos los números 
* de Fibonacci encontrados se muestra al final.
*-----------------------------------------------------------
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>  

// Estructura para pasar los datos al hilo, que contiene el límite
// superior y un array para almacenar los números de Fibonacci.
struct ThreadData {
    int limit; // Límite superior hasta donde se calcularán los números de Fibonacci.
    int *fibonacciNumbers; // Puntero para almacenar los números de Fibonacci.
    int count; // Cantidad de números de Fibonacci encontrados.
};

// Función para calcular manualmente el n-ésimo número de Fibonacci.
int manualFibonacci(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    int a = 0, b = 1, result = 0;
    for (int i = 2; i <= n; ++i) {
        result = a + b;
        a = b;
        b = result;
    }
    return result;
}

// Función que será ejecutada por el hilo para calcular la serie
// Fibonacci y almacenar los resultados en un array dentro de ThreadData.
void* calculateFibonacci(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg; // Conversión de argumento a estructura ThreadData.
    
    // Alocamos espacio en el heap para almacenar los resultados
    data->fibonacciNumbers = (int*) malloc(100 * sizeof(int));
    
    int n = 0, next = 0;

    // Calcula los números de Fibonacci hasta que superen el límite.
    while (next <= data->limit) {
        next = manualFibonacci(n); // Calcula el siguiente número de Fibonacci
        if (next <= data->limit) {
            data->fibonacciNumbers[data->count] = next; // Almacena el número si está dentro del límite
            data->count++;
        }
        n++;
    }

    pthread_exit(NULL); // Termina el hilo.
    return NULL;
}

int main() {
    int limit; // Variable para almacenar el límite superior proporcionado por el usuario.

    // Solicita al usuario ingresar un número entre 0 y 100.
    printf("Ingrese un número entre 0 y 100: ");
    scanf("%d", &limit);

    // Validación del número ingresado para asegurar que está dentro del rango permitido.
    if (limit < 0 || limit > 100) {
        printf("Número fuera del rango permitido. Por favor, ingrese un número entre 0 y 100.\n");
        return 1; // Termina el programa si el número es inválido.
    }

    // Inicialización de la estructura ThreadData con el límite ingresado por el usuario.
    struct ThreadData data;
    data.limit = limit;
    data.count = 0; // Inicializa el contador de números de Fibonacci.

    pthread_t thread; // Declaración de la variable para el hilo.

    // Creación del hilo que ejecutará la función calculateFibonacci.
    pthread_create(&thread, NULL, calculateFibonacci, &data);

    // Espera a que el hilo termine su ejecución antes de continuar.
    pthread_join(thread, NULL);

    // Inicializa la suma total de los números de Fibonacci encontrados.
    int sum = 0;

    // Imprime los números de Fibonacci encontrados y calcula la suma total.
    printf("\nNúmeros de Fibonacci entre 0 y %d:\n", limit);
    printf("------------------------------------\n");
    printf(" Iteración   Valor de Fibonacci\n");


    for (int i = 0; i < data.count; ++i) {
        printf("    F%d               %d\n", i, data.fibonacciNumbers[i]);
        sum += data.fibonacciNumbers[i];
    }

    // Muestra la suma total de los números de Fibonacci encontrados.
    printf("------------------------------------\n");
    printf("Suma total de los números de Fibonacci encontrados  : %d\n", sum);

    // Libera la memoria alocada
    free(data.fibonacciNumbers);

    return 0; // Fin del programa.
}