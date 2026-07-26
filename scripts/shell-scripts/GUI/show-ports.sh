#!/usr/bin/env bash 

SUCCESS=0



# -----------------------------------------------------------------------------------
# Como su nombre indica esta función lo que hace es mostrar los puertos que esten 
# activos, incluyendo conexiones relevantes, lo muestra con colores y
# de manera amigable al usuario.
#
# Usamos 'ss' para poder ver lo sockets del sistema y listarlos de manera bonita
# después del parseo que se le hace con 'cut', 'grep', 'sort' y 'uniq' para
# mostrarlos ordenados y legibles para el usuario.
# -----------------------------------------------------------------------------------
show_ports() {
    sudo ss -tuln -H | awk '{print $5}' | cut -d: -f2 | grep -E '^[0-9]+$' | sort -n | uniq | while read port; do

        service=$(getent services "$port" | cut -d' ' -f1)

        if ! [ -n "$service" ]; then
            echo -e "\e[1;32m| [+]\e[0m \e[1;36m$port (desconocido)\e[0m"
            continue
        fi

        echo -e "\e[1;32m| [+]\e[0m \e[1;36m$port $service\e[0m"
    done
}


echo -e "\e[36m|-----------------------------------|\e[0m"

show_ports

echo -e "\e[36m|-----------------------------------|\e[0m"

