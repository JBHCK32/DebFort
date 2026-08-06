#!/usr/bin/env bash

# ---------------------------------------------------------------------------------------------------------------------------
# COnfiguración automatica de SSH:
#
# 1. Deshabilitar el permiso root por acceso mediante ssh.
# 2. Deshabilitar la autenticación por contraseña vacia.
# 3. Deshabiliater el reenvio por X11.
# 4. Deshabilitar eñ reenvio de agente SSH.
# 5. Establecer tiempo de inactividad (3000 segundos o 5 minutos maximo de inactividad y 3 intentos para iniciar sesión.)
# ---------------------------------------------------------------------------------------------------------------------------
SSH_TIME=300
SSH_SESSION=3

main() {
    echo -e "\e[32m[INFO/SSH]: Configurando SSH...\e[0m"

    echo -e "\e[32m[1/6] Deshabilitando el acceso root por medio de SSH...\e[0m"

    sudo sed -i 's/^PermitRootLogin yes/PermitRootLogin no/' /etc/ssh/sshd_config

    echo -e "\e[32,[2/6] Deshabilitando la autenticación con contraseña vacia...\e[0m"

    sudo sed -i 's/^PermitEmptyPasswords yes/PermitEmptyPasswords no/' /etc/ssh/sshd_config

    echo -e "\e[32m[3/6] Deshabilitando el reenvio por X11...\e[0m"

    sudo sed -i 's/^X11Forwarding yes/X11Forwarding no/' /etc/ssh/sshd_config

    echo -e "\e[32m[4/6] Deshabilitando el reenvio por el agente SSH...\e[0m"

    sudo sed -i 's/^AllowAgentForwarding yes/AllowAgentForwarding no/' /etc/ssh/sshd_config

    echo -e "\e[32m[5/6] Configurando el tiempo de inactividad a $SSH_TIME segundos...\e[0m"
    
    sudo sed -i "s/^#\?ClientAliveInterval .*/ClientAliveInterval $SSH_TIME/" /etc/ssh/sshd_config

    echo -e "\e[32m[6/6] Configurando los intentos de sesión a $SSH_SESSION intentos...\e[0m"
    
    sudo sed -i "s/^#ClientAliveCountMax 3/ClientAliveCountMax $SSH_SESSION/" /etc/ssh/sshd_config

    # Reiniciar SSH para aplicar todos los cambios.
    
    echo -e "\e[32m[INFO/SSH]: Reiniciando SSH para aplicar los cambios...\e[0m"
    sudo systemctl restart sshd

    exit 0
}

main
