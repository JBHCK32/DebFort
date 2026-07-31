# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -fstack-protector-strong \
         -D_FORTIFY_SOURCE=3 -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE \
         -fPIE
LDFLAGS = -pie -Wl,-z,relro,-z,now -Wl,-z,noexecstack


# Directorios
SRC_DIR = scripts/C-scripts
BUILD_DIR = build
TARGET = $(BUILD_DIR)/debfort

# Recopilar todos los archivos fuente (.c) recursivamente
SOURCES = $(shell find $(SRC_DIR) -type f -name '*.c')

# Generar lista de objetos: mantener estructura dentro de build/
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# Directorios de inclusión para cabeceras
INCLUDES = -I$(SRC_DIR) \
           -I$(SRC_DIR)/utils \
           -I$(SRC_DIR)/close-unused-ports \
           -I$(SRC_DIR)/services-uninstall \
           -I$(SRC_DIR)/main-UX

# Regla por defecto
all: $(TARGET)

# Enlazar todos los objetos en el binario final
$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compilar cada archivo .c a .o, manteniendo la estructura de directorios
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Limpiar todo lo compilado
clean:
	rm -rf $(BUILD_DIR)

# Mostrar los archivos fuente (para depuración)
list-sources:
	@echo "Fuentes encontrados:"
	@echo "$(SOURCES)" | tr ' ' '\n'

.PHONY: all clean list-sources
