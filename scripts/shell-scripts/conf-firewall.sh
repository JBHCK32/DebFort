#!/usr/bin/env bash 


# ---------------------------------------------------------------------------
# Estas son reglas basicas de trafico donde denegamos el trafico entrante,
# el reenvio y solo permitimos el saliente.
# ---------------------------------------------------------------------------
Basic_Trafic_Conf() {
    iptables -P INPUT DROP
    iptables -P FORWARD DROP
    iptables -P OUTPUT ACCEPT

    ip6tables -P INPUT DROP 2>/dev/null
    ip6tables -P FORWARD DROP 2>/dev/null
    ip6tables -P OUTPUT ACCEPT 2>/dev/null
}

# ----------------------------------------------------------
# Permitir respuestas a conexiones que el servidor inició.
# ----------------------------------------------------------
Conf_Init_Trafic() {
    iptables -A INPUT -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT 
    ip6tables -A INPUT -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT 2>/dev/null
}

# --------------------------------------------------------
# Con esto permitimos todo el trafico de interfaz local.
# --------------------------------------------------------
Init_Local_Trafic() {
    iptables -A INPUT -i lo -j ACCEPT 
    ip6tables -A INPUT -i lo -j ACCEPT 2>/dev/null
}

Init_Esencial_Ports() {
    # SSH (puerto 22)
    iptables -A INPUT -p tcp --dport 22 -m conntrack --ctstate NEW -j ACCEPT
    ip6tables -A INPUT -p tcp --dport 22 -m conntrack --ctstate NEW -j ACCEPT 2>/dev/null

    # HTTP (puerto 80)
    iptables -A INPUT -p tcp --dport 80 -m conntrack --ctstate NEW -j ACCEPT
    ip6tables -A INPUT -p tcp --dport 80 -m conntrack --ctstate NEW -j ACCEPT 2>/dev/null

    # HTTPS (puerto 443)
    iptables -A INPUT -p tcp --dport 443 -m conntrack --ctstate NEW -j ACCEPT
    ip6tables -A INPUT -p tcp --dport 443 -m conntrack --ctstate NEW -j ACCEPT 2>/dev/null
}

# ----------------------------------------------------------------------------------------
# Con esto limtamos las conexiones nuevas de ssh y evitamos los ataques de fuerza bruta.
# ----------------------------------------------------------------------------------------
Basic_Protection_SSH() {
    iptables -A INPUT -p tcp --dport 22 -m conntrack --ctstate NEW -m recent --set --name SSH
    iptables -A INPUT -p tcp --dport 22 -m conntrack --ctstate NEW -m recent --update --seconds 60 --hitcount 4 --name SSH -j DROP

    ip6tables -A INPUT -p tcp --dport 22 -m conntrack --ctstate NEW -m recent --set --name SSH 2>/dev/null
    ip6tables -A INPUT -p tcp --dport 22 -m conntrack --ctstate NEW -m recent --update --seconds 60 --hitcount 4 --name SSH -j DROP 2>/dev/null
}

# ---------------------------------------------------------------------------
# Esta función hace varias cosas pero es importante. Primero
# crea el archivo de configuración en donde vamos a guardar las reglas
# que acabamos de configurar. Segundo, carga la configuración que hicimos
# en un servicio en donde le dice donde esta la configuración que hicimos,
# que hacer con ellas y que hacer cuando el servicio se inicia.
#
# Lo ultimo es que le decimos a systemd que vuelva a leer
# todos los archivos de configuración de todos los servicios.
# Ya al final iniciamos iptables-restore.service para cargar y tener
# de manera permanente las configuraciones del firewall que hizo el script.
# ---------------------------------------------------------------------------
Save_Conf_IpTables() {
    echo -e "\e[1;32m[INFO/FIREWALL]: Guardando reglas de iptables...\e[0m"

    mkdir -p /etc/iptables

    iptables-save > /etc/iptables/rules.v4
    ip6tables-save > /etc/iptables/rules.v6

    echo -e "\e[32m[INFO/FIREWALL]: Configurando carga automática de reglas...\e[0m"

    cat > /etc/systemd/system/iptables-restore.service << 'EOF'
[Unit]
Description=Restore iptables rules
Before=network.target

[Service]
Type=oneshot
ExecStart=/sbin/iptables-restore /etc/iptables/rules.v4
ExecStart=/sbin/ip6tables-restore /etc/iptables/rules.v6

[Install]
WantedBy=multi-user.target    
EOF

    systemctl daemon-reload

    systemctl enable iptables-restore.service
    systemctl start iptables-restore.service
}

Clear_Rules() {
    iptables -F
    iptables -X
    ip6tables -F
    ip6tables -X
}

local SUCCESS=0
local ERR_NOT_ROOT=1

# -----------------------------------------------------------------------------------------------------
# Ahora el main ejecuta todo de manera ordenada, el script no creo que falle
# ya que los errores de IPv6 se silencian y de paso el script siempre se ejecuta con
# sudo desde el programa de menu que esta hecho en C, entonces no creo que ocurran muchos
# problemas con este script.
#
# Al final lo que hace es:
#
# 1. Verifica si el script se ejecuto con root.
# 2. Limpia la configuración anterior de iptables.v4 y v6 para empezar a configurar desde cero.
# 3. Hace una configuración basica de trafico.
# 4. Configura el permiso para que el servidor pueda recibir conexiones con maquinas conocidas.
# 5. Permite las comunicaciones internas atraves de lookback.
# 6. Deja al usuario con los minimos puertos indispensables para que el elija cuales necesita.
# 7. Configura una protección bsica en el intento de conexiones con el puerto SSH(22),
# 8. Guarda las configuraciones y vuelve a cargarlas de manera inmediata sin reiniciar el equipo.
# 
# Es algo simple pero necesario para tener una buena base de un servidor seguro y empezar
# a configurarlo a tu gusto.
# -----------------------------------------------------------------------------------------------------
main() {
    local ROOT_ACCESS=0

    if [[ "$EUID" -ne "$ROOT_ACCESS" ]]; then
        echo -e "\e[1;31mEste script debe ejecutarse como root (con sudo).\e[0m"
        return ERR_NOT_ROOT
    fi

    Clear_Rules

    Basic_Trafic_Conf

    Conf_Init_Trafic

    Init_Local_Trafic

    Init_Esencial_Ports

    Basic_Protection_SSH

    Save_Conf_IpTables

    return $SUCCESS
}
