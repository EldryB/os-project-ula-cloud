#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * service_leak: Un programa que devora memoria.
 * Su objetivo es superar el límite impuesto por el orquestador (RLIMIT_AS).
 */

#define CHUNK_SIZE (10 * 1024 * 1024) // 10 MB por cada iteración

int main() {
    size_t total_allocated = 0;
    printf("[Leak] Iniciado. PID: %d. Objetivo: Devorar RAM.\n", getpid());

    while (1) {
        // Intentar reservar un bloque de 10 MB
        void *ptr = malloc(CHUNK_SIZE);

        if (ptr == NULL) {
            fprintf(stderr, "\n[Leak] ❌ malloc() falló. Ya no puedo pedir más memoria.\n");
            fprintf(stderr, "[Leak] Total acumulado antes del fallo: %zu MB\n", total_allocated / (1024 * 1024));
            
            // Si el malloc falla, el programa sale con error.
            // Esto debería ser capturado por el monitor como STATE_CRASHED.
            exit(1);
        }

        // IMPORTANTE: Escribir en la memoria para que el SO realmente la asigne (Physical Pages)
        memset(ptr, 0, CHUNK_SIZE);
        
        total_allocated += CHUNK_SIZE;
        printf("[Leak] Reservados: %zu MB... (Sigo con hambre)\n", total_allocated / (1024 * 1024));

        // Esperar un segundo para que el dashboard del orquestador pueda mostrar el progreso
        sleep(1);
    }

    return 0;
}
