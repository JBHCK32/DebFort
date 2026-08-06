#!/usr/bin/env bash

# --------------------------------------------------------------
# build-and-run.sh - Compila y ejecuta DebFort
# Uso: ./build-and-run.sh
# --------------------------------------------------------------

echo -e "\n\x1b[1;36m[DEBFORT] Compilando el proyecto...\x1b[0m\n"

# Compilar con make
if make clean && make; then
    echo -e "\n\x1b[1;32m[OK] Compilación exitosa.\x1b[0m"
else
    echo -e "\n\x1b[1;31m[ERROR] Falló la compilación.\x1b[0m"
    exit 1
fi

# Verificar que el binario existe
if [ ! -f "./build/debfort" ]; then
    echo -e "\n\x1b[1;31m[ERROR] No se encontró el binario en ./build/debfort\x1b[0m"
    exit 1
fi

# Ejecutar con sudo
echo -e "\n\x1b[1;36m[DEBFORT] Ejecutando el programa (sudo)...\x1b[0m\n"
sudo ./build/debfort

# Salir con el código de retorno del programa
exit $?
