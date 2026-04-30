#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include "orchestrator.h"

/**
 * TODO: Implementar la restricción de recursos del sistema.
 * * Requisitos:
 * 1. Definir los límites (soft y hard) para el espacio de direccionamiento virtual.
 * 2. Aplicar el límite utilizando la syscall 'setrlimit'.
 * 3. Garantizar que si la configuración falla, el proceso no continúe su ejecución.
 * * Ayuda: Revisar la constante RLIMIT_AS en la documentación de Linux.
 */
void apply_resource_limits(size_t mem_limit) {

    struct rlimit limits;

    limits.rlim_cur = mem_limit;//Limite soft
    limits.rlim_max = mem_limit;//Limite Hard, en nuestro caso ambos iguales

    //El primer argumento es el recurso que queremos limitar, el segundo la dir de memoria de limits
    int val = setrlimit(RLIMIT_AS, &limits);

    if(val != 0)//setrlimit fallo
    {
        perror("error en setrlimit");
        exit(1);
    }
    
}
