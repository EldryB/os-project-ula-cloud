# 🤝 Guía de Contribución: ULA-Cloud

Para que tu proyecto sea aceptado y evaluado, debes seguir estrictamente este flujo de trabajo. En esta cátedra operamos bajo estándares de industria: **el código que no se puede explicar, no existe.**


## 🤖 Política de Uso de IA
Se permite el uso de herramientas de Inteligencia Artificial (ChatGPT, Gemini, Claude, Copilot, etc.) bajo las siguientes condiciones:
1.  **Documentación:** Si usaste IA para generar una función compleja o resolver un bug, debes indicarlo en un comentario en el código o en la descripción del Pull Request.
2.  **Comprensión:** La IA es un asistente, no el autor. Si utilizas código generado por IA que no puedes explicar durante la consulta técnica, se asumirá como **plagio** y la nota será **0**.


## 🛠️ Flujo de Entrega
1.  **Fork & Branch:** Crea una rama con el formato `feature/nombre-apellido`.
2.  **Commits:** Usa mensajes descriptivos (ej: `feat: implement logic for resource limits in resources.c`).
3.  **Pull Request (PR):** Al finalizar, abre un PR hacia la rama `main` del repositorio original. Completa la plantilla de descripción del PR detallando qué implementaste.


## ⚖️ El Proceso de Evaluación 

Tu proyecto pasará por cuatro filtros obligatorios. Si falla en cualquiera de los tres primeros, no se avanza al siguiente.

### 1. Tests Automáticos (CI)
Al abrir el PR, se ejecutarán scripts de prueba automáticos.
* **Compilación:** El código debe compilar sin *warnings* usando el `Makefile`.
* **Prueba de Fuego:** El sistema verificará si los 4 servicios arrancan y si el `service_leak` es terminado correctamente.

### 2. Prueba Manual del Profesor
Ejecutaré tu binario en un entorno controlado y aislado (WSL 2 / Linux Nativo).
* Se evaluará la estabilidad ante señales (`Ctrl+C`).
* Se verificará que no queden procesos "zombie" tras el cierre.
* Se comprobará la veracidad del dashboard de estado.

### 3. Revisión de Código (Code Review)
Analizaré la calidad de tu implementación:
* **Uso de Mutex:** ¿Están protegidas todas las regiones críticas?
* **Gestión de Memoria:** ¿Liberaste los recursos de los hilos? ¿Hay fugas en el orquestador?
* **Modularidad:** ¿Respetaste la estructura de archivos definida en el `README.md`?

### 4. La Consulta Técnica (Defensa Oral)
Si lo considero necesario, serás convocado a una consulta presencial o virtual.
* Deberás explicar líneas de código específicas al azar.
* Deberás justificar tus decisiones de diseño (ej: ¿Por qué usaste `waitpid` de forma bloqueante en el hilo?).
* **⚠️ REGLA DE ORO:** Si no sabes explicar qué hace tu código o por qué sucede un comportamiento específico, tu nota será **0**, sin excepciones.


## 📋 Estándares de Código
* **Lenguaje:** C puro (Estándar C11).
* **Formato:** Indentación consistente y un estilo legible en general.
* **Comentarios:** Documenta el "por qué", no el "qué". El código limpio se explica solo; la lógica compleja necesita contexto.
* **Robustez:** Valida siempre el retorno de las syscalls (`fork`, `pthread_create`, `setrlimit`).

Esta plantilla es el toque final para profesionalizar la entrega. Obliga al estudiante a reflexionar sobre lo que hizo y a ser honesto sobre el uso de herramientas externas. La puedes guardar como `.github/pull_request_template.md` en el repositorio para que aparezca automáticamente cuando abran un PR.


## 📝 Plantilla de Pull Request: ULA-Cloud

```markdown
## 🚀 Descripción del Proyecto
*¿Qué hace tu implementación de ULA-Cloud y cómo abordaste el reto de la orquestación?*

## 🛠️ Detalles Técnicos
Marca con una `x` las funcionalidades que lograste implementar:
- [ ] **Despliegue Paralelo:** Uso correcto de `fork()` y `execvp()` para los 4 servicios.
- [ ] **Jaula de Memoria:** Implementación de `setrlimit` en el proceso hijo.
- [ ] **Watchdog Multi-hilo:** Un hilo por servicio usando `pthread_create` y `waitpid`.
- [ ] **Sincronización:** Uso de `pthread_mutex_t` para proteger el dashboard global.
- [ ] **Manejo de Señales:** Limpieza de procesos hijos al recibir `SIGINT` (Ctrl+C).

## 🤖 Transparencia en el uso de IA
*¿Utilizaste herramientas de IA (ChatGPT, Claude, Github Copilot, etc.) durante el desarrollo?*
- [ ] No utilicé IA.
- [ ] Sí, utilicé IA para: _________________________________________________
*Nota: Recuerda que durante la consulta técnica deberás ser capaz de explicar cada línea de código, independientemente de su origen.*

## 🧪 Evidencia de Pruebas
*Copia y pega aquí la salida de tu terminal donde se vea al orquestador detectando la muerte del servicio `leak.c` por límite de memoria.*

```text
(Pega aquí tu log de salida)
```

## 🏁 Checklist de Robustez (Autoevaluación)
- [ ] Mi código compila sin warnings con `make all`.
- [ ] No quedan procesos "zombie" tras cerrar el orquestador.
- [ ] Todos los hilos se liberan correctamente.
- [ ] El dashboard muestra el estado real (RUNNING, CRASHED, KILLED) de cada servicio.
- [ ] Estoy preparado para explicar cualquier parte del código en una consulta técnica.
```
