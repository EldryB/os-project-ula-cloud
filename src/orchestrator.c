#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "orchestrator.h"

/**
 * TODO: Implementar el despliegue del microservicio.
 * * Requisitos:
 * 1. Crear un nuevo proceso.
 * 2. En el proceso HIJO:
 * - Configurar las restricciones de recursos (memoria).
 * - Reemplazar la imagen del proceso por el binario del servicio.
 * 3. En el proceso PADRE:
 * - Registrar el PID y el estado inicial en el dashboard.
 * - Retornar el PID asignado.
 */
int spawn_service(int index) {
    // 1. Clonar el proceso actual
    pid_t pid = fork();

    if (pid < 0) {
        // El kernel no pudo crear el proceso por cualquier razon
        perror("[Orquestador] Error al ejecutar fork");
        return -1;
        
    } 
    
    else if (pid == 0) {
        
        // TODO: Aquí llamaremos a apply_resource_limits() más adelante.

        // Preparamos los argumentos para execvp. 
        // args[0] por convención es el nombre/ruta del programa.
        // El último elemento DEBE ser NULL.
        char *args[] = { dashboard[index].path, NULL };

        // El proceso hijo se convierte en el microservicio.
        execvp(args[0], args);

        // Si execvp tiene éxito, la imagen del proceso se reemplaza y el código NUNCA llega aquí.
        // Si llegamos a esta línea, significa que execvp falló.
        fprintf(stderr, "[Orquestador] Error crítico: No se pudo ejecutar %s\n", dashboard[index].path);
        perror("Detalle del error de execvp");
        exit(1); 
        
    } 
    
    else {
        // Caso C: ESTE ES EL PROCESO PADRE
        // Registramos la información vital en nuestro dashboard global
        dashboard[index].pid = pid;
        dashboard[index].state = STATE_RUNNING;

        // Retornamos el PID para cualquier uso futuro
        return pid;
    }
}
