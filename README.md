# 🏗️ Proyecto 2: ULA-Cloud (Micro-Orchestrator)

**ULA-Cloud** es un orquestador de microservicios minimalista diseñado para sistemas Linux. El objetivo de este proyecto es gestionar el ciclo de vida de múltiples procesos en paralelo, monitorear su estado de salud mediante hilos de ejecución y restringir su consumo de recursos (memoria) utilizando las capacidades del kernel.

Este proyecto sirve como puente entre la gestión de memoria (Proyecto 1) y el desarrollo de aplicaciones concurrentes en red (Proyecto 3).

## 🎯 Objetivos de Aprendizaje
* **Gestión de Procesos:** Dominar el flujo `fork() -> setup -> exec()`.
* **Concurrencia:** Implementar un modelo de monitoreo multi-hilo con `pthreads`.
* **Sincronización:** Proteger estados compartidos mediante **Mutex** para evitar condiciones de carrera.
* **Control de Recursos:** Utilizar la syscall `setrlimit` para definir cuotas de memoria virtual.
* **Señales de Linux:** Interpretar códigos de salida y señales de terminación del kernel.


## 📂 Estructura del Proyecto

```text
ula-cloud/
├── bin/                # Binarios compilados
├── include/            # Cabeceras (.h)
│   ├── orchestrator.h  # Estructuras globales y tipos
│   └── resources.h     # Prototipos para límites de recursos
├── src/                # Implementación principal
│   ├── main.c          # Punto de entrada y parseo
│   ├── orchestrator.c  # Gestión de fork/exec
│   ├── monitor.c       # Hilos watchdog
│   └── resources.c     # Implementación de setrlimit
├── services/           # Microservicios de prueba (.c)
│   ├── logger.c        # Servicio estable
│   ├── chaos.c         # Servicio con fallos aleatorios
│   └── leak.c          # Servicio con fuga de memoria
└── Makefile            # Automatización de construcción
```


## 🛠️ El Reto Técnico

### 1. Despliegue Paralelo
El orquestador debe lanzar todos los servicios definidos de forma simultánea. No se permite la ejecución secuencial. Cada servicio debe vivir en su propio espacio de direccionamiento como un proceso independiente.

### 2. La "Jaula" de Memoria
Se debe implementar una restricción de recursos utilizando **`setrlimit(RLIMIT_AS, ...)`**. 
* El límite debe aplicarse en el proceso hijo después del `fork()` pero antes del `execvp()`.
* Si un servicio intenta exceder este límite, el kernel debe intervenir.

### 3. Monitoreo Watchdog
Por cada proceso hijo, el orquestador debe crear un hilo dedicado (`pthread`). Este hilo permanecerá bloqueado en `waitpid()` esperando cualquier cambio de estado del hijo.
* El hilo debe reportar si el proceso terminó normalmente, si falló por un error de programación o si fue exterminado por el kernel debido a un exceso de memoria.

### 4. Dashboard de Estado (Thread-Safe)
El estado de todos los servicios se almacena en una estructura global. Debido a que múltiples hilos de monitoreo pueden intentar actualizar esta estructura al mismo tiempo, es obligatorio el uso de **Mutex** para garantizar la integridad de los datos.


## 🚀 Instrucciones de Uso

### Requisitos previos
* Sistema operativo Linux o WSL 2.
* Compilador `gcc` y herramienta `make`.

### Compilación
Para compilar el orquestador y todos los servicios de prueba:
```bash
make all
```

### Ejecución
Para iniciar el orquestador con un límite de memoria global (ejemplo 50MB):
```bash
./bin/ula-cloud --mem 50M
```

---

## 📊 Criterios de Evaluación y Robustez

| Criterio | Descripción |
| :--- | :--- |
| **Paralelismo** | Los procesos deben ejecutarse al mismo tiempo (validado con `htop`). |
| **Aislamiento** | Un fallo en un servicio no debe tumbar al orquestador ni a otros servicios. |
| **Precisión del Límite** | El servicio `leak.c` debe ser detenido exitosamente por el kernel al alcanzar la cuota. |
| **Sincronización** | Ausencia de condiciones de carrera en el dashboard de estado. |
| **Limpieza** | Al cerrar el orquestador (Ctrl+C), todos los hijos deben ser terminados (`SIGTERM`). |


## 📑 Entrega
La entrega se realizará exclusivamente mediante un **Pull Request** en el repositorio asignado. Asegúrate de leer el archivo `CONTRIBUTING.md` para conocer los estándares de código y el formato de los mensajes de commit.

> **Nota:** Se penalizará el uso de variables globales no protegidas por mutex y la presencia de procesos "zombie" al finalizar la ejecución.


