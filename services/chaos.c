#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

/**
 * service_chaos: Un programa diseñado para fallar.
 * Útil para probar la robustez del monitor y el manejo de señales.
 */

int main() {
    // Inicializar semilla aleatoria con el PID y el tiempo
    srand(time(NULL) ^ (getpid() << 16));

    printf("[Chaos] Iniciado. PID: %d. No planeo durar mucho...\n", getpid());

    while (1) {
        int r = rand() % 100;

        if (r < 10) {
            // Escenario 1: Salida con error (Exit Code 1)
            printf("[Chaos] Decidí cansarme y salir con error. Adiós.\n");
            exit(1); 
        } 
        else if (r < 15) {
            // Escenario 2: Desastre total (División por cero)
            // Esto generará un SIGFPE que el orquestador debe detectar.
            printf("[Chaos] Intentando dividir por cero para ver qué pasa...\n");
            int a = 10;
            int b = 0;
            int c = a / b;
            printf("Resultado: %d\n", c); // Nunca llegará aquí
        }
        else if (r < 20) {
            // Escenario 3: Acceso inválido (Segfault)
            // Esto generará un SIGSEGV.
            printf("[Chaos] Accediendo a memoria prohibida...\n");
            int *ptr = NULL;
            *ptr = 42; 
        }

        printf("[Chaos] Sigo vivo... (r=%d)\n", r);
        sleep(3);
    }

    return 0;
}
