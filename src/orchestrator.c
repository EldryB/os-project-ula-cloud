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
    // Clonamos el proceso actual
    pid_t pid = fork();

    if (pid == 0) //Caso que sea un proceso hijo
    {
        
        // Preparamos los argumentos para execvp. 
        // args[0] por convención es el nombre/ruta del programa.
        // El último elemento DEBE ser NULL.
        char *args[] = { dashboard[index].path, NULL };

        //Aplicamos los limites de ram que pueda consumir el microservicio
        apply_resource_limits(dashboard[index].mem_limit);

        // El proceso hijo se convierte en el microservicio.
        execvp(args[0], args);

        // Si execvp tiene éxito, la imagen del proceso se reemplaza y el código NUNCA llega aquí.
        // Si llegamos a esta línea, significa que execvp falló.
        perror("Error en el execvp");
        exit(1); 
        
    } 
    
    else if (pid < 0) 
    {
        // El kernel no pudo crear el proceso por cualquier razon
        perror("Error al ejecutar fork");
        return -1;
        
    } 

    else {
        // Caso del proceso padre
        // Registramos la información vital en nuestro dashboard global
        dashboard[index].pid = pid;
        dashboard[index].state = STATE_RUNNING;

        // Retornamos el PID para cualquier uso futuro
        return pid;
    }
}
