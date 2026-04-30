#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "orchestrator.h"

/* --- Variables Globales --- */
service_t dashboard[MAX_SERVICES];
int num_services = 0;
pthread_mutex_t dashboard_mutex;

/**
 * Función de utilidad para limpiar la terminal.
 */
void clear_screen() {
    printf("\033[H\033[J");
}

/**
 * TODO: Implementar la visualización del estado actual de los servicios.
 * Se debe garantizar una lectura consistente de los datos compartidos.
 */

const char* get_state_string(service_state_t s) {
    
    switch (s) 
    {
        case STATE_IDLE:    return "IDLE";
        case STATE_RUNNING: return "RUNNING";
        case STATE_CRASHED: return "CRASHED";
        case STATE_KILLED:  return "KILLED";
        case STATE_STOPPED: return "STOPPED";
        default:            return "UNKNOWN";
    }
}

void print_dashboard() {
    clear_screen();
    printf("==============================================================\n");
    printf("                ULA-CLOUD MONITORING DASHBOARD               \n");
    printf("==============================================================\n");
    printf("%-15s %-10s %-15s %-10s\n", "SERVICIO", "PID", "ESTADO", "EXIT/SIG");
    printf("--------------------------------------------------------------\n");

    //Protegemos que mas de un hilo no se metan con los datos
    pthread_mutex_lock(&dashboard_mutex);

    for (int i = 0; i < num_services; i++) 
    {
        printf("%-15s %-10d %-15s %-10d\n", 
               dashboard[i].name, dashboard[i].pid, 
               get_state_string(dashboard[i].state), dashboard[i].exit_status);
    }
    pthread_mutex_unlock(&dashboard_mutex);


    printf("==============================================================\n");
}

/**
 * TODO: Gestión de finalización del orquestador.
 * Implementar una estrategia para evitar la proliferación de procesos huérfanos.
 */
void handle_shutdown(int sig) {
    printf("\n Iniciando apagado\n");

    //Identificamos quien lo mato
    if (sig == SIGINT) 
    {
        printf("\nApagando por Ctrl+C\n");
    } 
    else if (sig == SIGTERM) 
    {
        printf("\nApagando porque el sistema lo pidio\n");
    }
    
    for (size_t i = 0; i < num_services; i++)
    {
        if (dashboard[i].state == STATE_RUNNING)
        {
            kill(dashboard[i].pid, SIGTERM);//Evitamos que queden procesos vivos y que se conviertan en huerfanos al parar la ejecucion del programa
        }
        
    }
    
    exit(0);
}

int main(int argc, char *argv[]) {
    // 1. Inicialización de mecanismos de sincronización
    if (pthread_mutex_init(&dashboard_mutex, NULL) != 0) {
        perror("Error inicializando mutex");
        return 1;
    }

    // 2. Captura de interrupciones del sistema
    signal(SIGINT, handle_shutdown);

    // 3. Configuración de la carga de trabajo (Servicios de prueba)
    num_services = 3;
    
    strcpy(dashboard[0].name, "Logger");
    strcpy(dashboard[0].path, "./bin/logger");
    dashboard[0].mem_limit = DEFAULT_MEM_LIMIT;

    strcpy(dashboard[1].name, "Chaos");
    strcpy(dashboard[1].path, "./bin/chaos");
    dashboard[1].mem_limit = DEFAULT_MEM_LIMIT;

    strcpy(dashboard[2].name, "Leak");
    strcpy(dashboard[2].path, "./bin/leak");
    dashboard[2].mem_limit = 20 * 1024 * 1024; // Límite de 20MB

    // 4. Activación del ecosistema
   printf("[ULA-Cloud] Inicializando %d microservicios...\n", num_services);
    
    for (int i = 0; i < num_services; i++) 
    {
        // Lanzamos el proceso
        spawn_service(i);
        // Lanzamos el hilo vigilante para ese proceso
        pthread_create(&dashboard[i].monitor_thread, NULL, monitor_service, &dashboard[i]);
    }

    // 5. Ciclo de monitoreo principal
    while (1) {
        print_dashboard();
        sleep(1); 
    }

    pthread_mutex_destroy(&dashboard_mutex);
    return 0;
}
