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

    //Con esto Eliminamos al hilo 
    pthread_detach(pthread_self()); 
    
    service_t* serv = (service_t*)arg;//Casteo para poder usar el parametro de la funccion
    int status;

    //Llamamos a la funcion bloqueante 
    waitpid(serv->pid, &status, 0);
    //Protegemos a que otro hilo no se pueda meter con los datos
    pthread_mutex_lock(&dashboard_mutex);

    if(WIFEXITED(status))//Verificamos si el proceso termino por su cuenta
    {
        int ret = WEXITSTATUS(status);//Guardamos el valor numerico del exit_status
        serv->exit_status = ret;
        if(ret == 0)//Aqui verificamos el estado con el cual termino
        {
            serv->state = STATE_STOPPED;
        }
        else
        {
            serv->state = STATE_CRASHED;
        }
    }

    if(WIFSIGNALED(status))//Si el proceso lo cortaron
    {
        serv->exit_status = WTERMSIG(status);//Guardamos la señal que lo mato
        serv->state = STATE_KILLED;//Estado con el que murio
    }
    pthread_mutex_unlock(&dashboard_mutex);
    return NULL;
}
