# Configuración del Compilador
CC      = gcc
CFLAGS  = -Wall -Wextra -pthread -Iinclude -g
LDFLAGS = -pthread

# Directorios
SRC_DIR      = src
INC_DIR      = include
SRV_DIR      = services
BIN_DIR      = bin
OBJ_DIR      = obj

# Objetivos Finales
TARGET       = $(BIN_DIR)/ula-cloud
SERVICES_SRC = $(wildcard $(SRV_DIR)/*.c)
SERVICES_BIN = $(patsubst $(SRV_DIR)/%.c, $(BIN_DIR)/%, $(SERVICES_SRC))

# Archivos del Orquestador
ORCH_SRC     = $(wildcard $(SRC_DIR)/*.c)
ORCH_OBJ     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.c.o, $(ORCH_SRC))

# --- REGLAS PRINCIPALES ---

.PHONY: all clean directories services

# Construir todo: Directorios, Orquestador y Servicios
all: directories $(TARGET) services
	@echo "✅ [ULA-Cloud] Compilación completada con éxito."

# Crear directorios necesarios
directories:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

# Compilación del Orquestador (Enlace final)
$(TARGET): $(ORCH_OBJ)
	@echo "🔗 Enlazando orquestador: $@"
	@$(CC) $(ORCH_OBJ) -o $@ $(LDFLAGS)

# Compilación de archivos objeto del Orquestador
$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c
	@echo "📦 Compilando objeto: $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compilación de los Microservicios (Independientes)
services: $(SERVICES_BIN)

$(BIN_DIR)/%: $(SRV_DIR)/%.c
	@echo "🚀 Compilando servicio de prueba: $<"
	@$(CC) $(CFLAGS) $< -o $@

# Limpieza de binarios y objetos
clean:
	@echo "🧹 Limpiando proyecto..."
	@rm -rf $(BIN_DIR) $(OBJ_DIR)
	@echo "✨ Proyecto limpio."

# Ayuda
help:
	@echo "Uso del Makefile para ULA-Cloud:"
	@echo "  make all         - Compila el orquestador y todos los servicios."
	@echo "  make clean       - Elimina archivos temporales y binarios."
	@echo "  make services    - Compila solo los microservicios de prueba."
