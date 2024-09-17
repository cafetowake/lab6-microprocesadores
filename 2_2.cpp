/*
*-----------------------------------------------------------
* 2_2.cpp
*-----------------------------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERÍA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACIÓN
*
* CC3086 - Programación de Microprocesadores
*
*-----------------------------------------------------------
* Descripción:
* Este programa utiliza Pthreads para crear múltiples hilos que 
* imprimen un mensaje "Hello world" junto con el número del hilo. 
* Los hilos se crean y se les hace join dentro de un mismo ciclo for.
*-----------------------------------------------------------
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10 // Definición del número de hilos

// Función que será ejecutada por cada hilo
void *PrintHello(void *paramID) {
    int *id = (int *)paramID; // Cast del parámetro de hilo a un entero (número de hilo)
    
    // Imprimir el mensaje con el número de hilo
    printf("Hello world thread No. %d\n", *id);

    // Finalizar el hilo
    pthread_exit(NULL);
    return NULL; 
}

int main(int argc, char *argv[]) {
    pthread_t threadsID[NUM_THREADS]; // Arreglo para almacenar los IDs de los hilos
    int rc, t, param[NUM_THREADS]; // Variables de control y parámetros para los hilos

    // Crear hilos y hacer join en cada hilo dentro del mismo ciclo
    for (t = 0; t < NUM_THREADS; t++) {
        printf("Main: creando el hilo %d\n", t);
        param[t] = t;
        
        // Crear un hilo que ejecute la función PrintHello
        rc = pthread_create(&threadsID[t], NULL, PrintHello, (void *)&param[t]);
        if (rc) {
            printf("ERROR: el código de retorno de pthread_create() es %d\n", rc);
            exit(-1);
        }

        // Hacer join en el hilo inmediatamente después de crearlo
        pthread_join(threadsID[t], NULL);
    }

    // Finalizar la ejecución del programa
    pthread_exit(NULL);    
}
