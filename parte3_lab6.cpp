/*
*-----------------------------------------------------------
* parte3_lab6_alternada_infinito.cpp
*-----------------------------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERÍA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACIÓN
*
* CC3086 - Programación de Microprocesadores
*
*-----------------------------------------------------------
* Descripción:
* Este programa calcula la suma parcial de una serie alternada 
* utilizando hilos de Pthreads. El valor de "infinito" se fija en 
* un valor predefinido, mientras que el valor máximo de n es 
* ingresado por el usuario.
*-----------------------------------------------------------
*/

#include <pthread.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;
//std::

// Estructura para pasar los datos al hilo
struct ThreadData {
    int n;         // El valor de n para el término de la serie
    double result; // Resultado del cálculo de la serie alternada para el término n
};

// Función que calcula un término de la serie alternada
void* calculateTerm(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int n = data->n;
    data->result = pow(-1, n + 1) / n; // Calcula (-1)^(n+1) / n
    pthread_exit((void*)&data->result); // Retorna el resultado mediante pthread_exit

    return NULL;
}

int main() {
    int max_n;

    // Establecer el valor de "infinito" como 1000
    const int infinito = 1000;

    // Solicitar el valor máximo de n
    cout << "Ingrese el valor máximo de n (entre 1 y " << infinito << "): ";
    cin >> max_n;

    // Validación del valor ingresado
    if (max_n < 1 || max_n > infinito) {
        cerr << "El valor de n debe estar entre 1 y " << infinito << "." << endl;
        return 1;
    }

    pthread_t* threads = new pthread_t[max_n];  // Arreglo dinámico de hilos
    ThreadData* threadData = new ThreadData[max_n];  // Arreglo dinámico de estructuras ThreadData
    int rc;

    // Crear hilos para calcular cada término de la serie alternada
    for (int n = 1; n <= max_n; n++) {
        threadData[n - 1].n = n;
        rc = pthread_create(&threads[n - 1], NULL, calculateTerm, (void*)&threadData[n - 1]);
        if (rc) {
            cerr << "ERROR: el código de retorno de pthread_create() es " << rc << endl;
            exit(-1);
        }
    }

    // Esperar a que todos los hilos terminen y sumar los resultados
    double sum = 0.0;
    for (int n = 1; n <= max_n; n++) {
        double* result;
        rc = pthread_join(threads[n - 1], (void**)&result);
        if (rc) {
            cerr << "ERROR: el código de retorno de pthread_join() es " << rc << endl;
            exit(-1);
        }
        sum += *result;
    }

    // Imprimir el resultado final de la sumatoria de la serie
    cout << "El valor de la suma parcial de la serie alternada hasta n = " << max_n << " es: " << sum << endl;

    // Liberar la memoria asignada
    delete[] threads;
    delete[] threadData;

    pthread_exit(NULL);
}