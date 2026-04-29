#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include "orchestrator.h"

/**
 * TODO: Implementar la rutina del hilo monitor (Watchdog).
 * * Requisitos:
 * 1. Recuperar la estructura del servicio desde el argumento 'arg'.
 * 2. Utilizar 'waitpid' de forma BLOQUEANTE para esperar al proceso hijo.
 * 3. Analizar el estado de terminación (éxito, error o señal del sistema).
 * 4. Actualizar el Dashboard Global de forma SEGURA (evitar condiciones de carrera).
 */
void* monitor_service(void *arg) {
    // TODO: Castear el argumento al tipo de dato correcto.
    service_t* serv = (service_t*)arg;
    int status;
    waitpid(serv->pid, &status, 0);
    pthread_mutex_lock(&dashboard_mutex);
    if(WIFEXITED(status))
    {
        int ret = WEXITSTATUS(status);
        serv->exit_status = ret;
        if(ret == 0)
        {
            serv->state = STATE_STOPPED;
        }
        else
        {
            serv->state = STATE_CRASHED;
        }
    }

    if(WIFSIGNALED(status))
    {
        serv->exit_status = WTERMSIG(status);
        serv->state = STATE_KILLED;
    }
    pthread_mutex_unlock(&dashboard_mutex);
    return NULL;
}
