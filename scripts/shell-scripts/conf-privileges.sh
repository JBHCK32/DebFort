#!/usr/bin/env bash

# ---------------------------------------------------------------------------
# Script de configuración de políticas de contraseñas y privilegios
# Recibe parámetros para personalizar la configuración.
# Debe ejecutarse como root.
# ---------------------------------------------------------------------------

local SUCCESS=0

# Verificar que se ejecuta como root
if [[ $EUID -ne 0 ]]; then
    echo -e "\e[1;31m[ERROR] Este script debe ejecutarse como root (con sudo).\e[0m"
    return $SUCCESS
fi

# Valores por defecto (si no se pasan parámetros)
MIN_LEN=${1:-12}
MAX_DAYS=${2:-90}
WARN_DAYS=${3:-7}
HISTORY=${4:-5}
MIN_CLASS=${5:-3}
MIN_DIGITS=${6:-1}
MIN_UPPER=${7:-1}
MIN_LOWER=${8:-1}
MIN_SPECIAL=${9:-1}



# ---------------------------------------------------------------------------
# Aplicar políticas de contraseñas en /etc/login.defs
# ---------------------------------------------------------------------------
echo -e "\e[32m[INFO] Configurando políticas de contraseñas...\e[0m"

# Longitud mínima
sed -i "s/^PASS_MIN_LEN .*/PASS_MIN_LEN $MIN_LEN/" /etc/login.defs

# Caducidad máxima (días)
sed -i "s/^PASS_MAX_DAYS .*/PASS_MAX_DAYS $MAX_DAYS/" /etc/login.defs

# Advertencia antes de caducar (días)
sed -i "s/^PASS_WARN_AGE .*/PASS_WARN_AGE $WARN_DAYS/" /etc/login.defs

# ---------------------------------------------------------------------------
# Historial de contraseñas (evitar reutilización)
# Se añade remember=HISTORY en /etc/pam.d/common-password
# ---------------------------------------------------------------------------
echo -e "\e[32m[INFO] Configurando historial de contraseñas (remember=$HISTORY)...\e[0m"
sed -i "s/pam_unix.so/pam_unix.so remember=$HISTORY/" /etc/pam.d/common-password

# ---------------------------------------------------------------------------
# Calidad de contraseñas (requiere libpam-pwquality)
# Si no está instalado, se instala automáticamente
# ---------------------------------------------------------------------------
echo -e "\e[32m[INFO] Configurando calidad de contraseñas...\e[0m"

# Instalar libpam-pwquality si no está presente
if ! dpkg -l | grep -q libpam-pwquality; then
    echo -e "\e[33m[INFO] Instalando libpam-pwquality...\e[0m"
    apt update && apt install -y libpam-pwquality
fi

# Configurar /etc/security/pwquality.conf
cat > /etc/security/pwquality.conf << EOF
minlen = $MIN_LEN
dcredit = -$MIN_DIGITS
ucredit = -$MIN_UPPER
lcredit = -$MIN_LOWER
ocredit = -$MIN_SPECIAL
minclass = $MIN_CLASS
maxrepeat = 2
maxsequence = 3
EOF

# ---------------------------------------------------------------------------
# Gestión de privilegios
# 1. Bloquear acceso directo a root
# 2. Crear grupo admin y darle permisos sudo completos
# ---------------------------------------------------------------------------
echo -e "\e[32m[INFO] Configurando privilegios...\e[0m"

# Bloquear root (deshabilitar login directo)
passwd -l root 2>/dev/null || echo -e "\e[33m[INFO] Root ya está bloqueado.\e[0m"

# Crear grupo admin si no existe
groupadd -r admin 2>/dev/null || echo -e "\e[33m[INFO] El grupo admin ya existe.\e[0m"

# Configurar sudo para el grupo admin
echo "%admin ALL=(ALL) ALL" > /etc/sudoers.d/admin
chmod 440 /etc/sudoers.d/admin

# ---------------------------------------------------------------------------
# Finalizar
# ---------------------------------------------------------------------------
echo -e "\e[32m[OK] Configuración de privilegios y contraseñas completada.\e[0m"
return $SUCCESS
