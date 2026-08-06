#!/usr/bin/env bash

# show-ports.sh - Lista los puertos en escucha en el sistema
# Uso: sudo ./show-ports.sh


sudo ss -tuln -H | awk '{print $5}' | cut -d: -f2 | sort -n | uniq | while read port; do
    if [[ "$port" =~ ^[0-9]+$ ]] && [ "$port" -ne 0 ]; then
        service=$(getent services "$port" | cut -d' ' -f1)
        if [ -n "$service" ]; then
            echo -e "\e[36m- [+] $port $service\e[0m"
        else
            echo -e "\e[36m- [+] $port (desconocido)\e[0m"
        fi
    fi
done

