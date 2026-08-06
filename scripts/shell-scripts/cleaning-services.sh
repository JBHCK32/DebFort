#!/usr/bin/env bash

# Important: (---)
# Context:   (+++)

# -------------------------------
# Variables globales de error:
# -------------------------------
SUCCESS=0
ERR_ROOT=1


# -------------
# Servicios:
#
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#- Avahi-daemon	avahi-daemon.service	Descubrimiento de servicios mDNS.
#- CUPS	cups.service	Servidor de impresión.
#- RPC / Portmap	rpcbind.service o portmap.service	Mapeo de puertos RPC. rpcbind es el nombre moderno.
#- NFS (servidor)	nfs-server.service	Servidor de archivos NFS.
#- Samba (SMB/CIFS)	smbd.service y nmbd.service	smbd para el servicio de archivos, nmbd para NetBIOS.
#- NIS	nis.service	Servicio de información de red (legacy).
#
#- Telnet	Depende del servidor (ej. telnetd.service o xinetd.service)	No tiene un nombre estándar único.
#- FTP	Depende del servidor (ej. vsftpd.service, proftpd.service, etc.)	El nombre varía según el servidor FTP instalado.
#- TFTP	Depende del servidor (ej. tftpd-hpa.service)	Otros posibles: tftp.service o wapttftpserver.service.
#- RSH / rlogin	Generalmente rsh-server (paquete) o xinetd.service	No suelen tener un servicio systemd directo.
#
#- NTP (servidor)	ntp.service o systemd-timesyncd.service	systemd-timesyncd es el cliente SNTP por defecto en Debian.
#- Chrony (servidor)	chrony.service	Alternativa moderna a NTP.
#- ModemManager	ModemManager.service	Gestión de módems (móviles).
#- Bluetooth	bluetooth.service	Pila de Bluetooth.
#- cups-browsed	cups-browsed.service	Descubrimiento automático de impresoras.
#- rpc.statd	rpc-statd.service	Monitor de estado NFS (para NFSv2/v3).
#- nfs-idmapd	nfs-idmapd.service	Mapeo de nombres para NFSv4.
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# -------------------------------------------------------------------------------------------------------
# Esta es la lista de servicios que unicamente se va a deshabilitar, se deja asi por si algun proces
# los necesitara y solo se tuvieran que encender.
#
# Cuande se enmascare uno se dara una justificación del porqué, pero estos solo 
# se desactivan por estandarización.
# -------------------------------------------------------------------------------------------------------
DISABLE_SERVICES=(
    "cups.service"          # Impresión
    "avahi-daemon.service"  # Descubrimiento mDNS
    "postfix.service"       # Correo 
    "cups-browsed.service"  # Descubrimiento automático de impresoras
    "rpcbind.service"       # RPC 
    "nfs-server.service"    # NFS 
    "smbd.service"          # Samba 
    "nmbd.service"          # Samba 
    "telnetd.service"       # Telnet
    "vsftpd.service"        # FTP
    "proftpd.service"       # FTP
    "tftpd-hpa.service"     # TFTP
    "tftp.service"          # TFTP
    "wapttftpserver.service"# TFTP
)


# -----------------------------------------------------------------------------------------------
# Esta función recorre la lista de los servicios que solo se van a desactivar
# y los va desactivando uno por uno, la lista es estandarizada y se desactivan
# unicamente para no corromper dependencias o preferencias que tenga el usuario.
# Si el servicio que esta en la lista no se llegara a encontrar entonces directamente
# se le avisara al usuario sobre ello.
#
# De cualquier manera si desea enmascarar servicios o eliminarlos de manera permanente
# el mismo main.c le dara la opción al usuario para elegir de la lista de servicios completa 
# para eliminar.
# -----------------------------------------------------------------------------------------------
disable-services() {

    for SERVICE in "${DISABLE_SERVICES[@]}"; do
        
        echo -e "\e[32m[INFO/CLEAN]: Desactivando el servicio $SERVICE...\e[0m"
    
        if ! sudo systemctl stop "$SERVICE" && sudo systemctl disable "$SERVICE"; then
            echo -e "\e[1;31m[ERROR]: Servicio $SERVICE no encontrado...\e[0m"
            continue
        fi

        echo -e "\e[32m[OK]: $SERVICE desactivado correctamente.\e[0m"
    done

    return SUCCESS
}

# -------------------------------------------------------------------------
# Es la función general de la limpieza del servicios del servidor.
# Sirve como gestor general y tambien como enmascarador de servicios
# especificos.
# -------------------------------------------------------------------------
clean-basic-services () {

    echo -e "\e[32m- [INFO/CLEAN]: ELiminando servicios inecesarios del servidor... -\e[0m"

    # +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    # Lo que va hacer la función es: 
    #
    # pausa y desactiva el servicio para que no se utilize durante el proceso
    # de desactivación.
    #
    # - Ex: sudo systemctl stop <nombre-del-servicio> && sudo systemctl disable <nombre-del-servicio>
    #
    #
    # Después del proceso seguro se en mascaran los procesos como medida extrema
    # para que el servicio no se encienda aunque otro proceso lo llame.
    #
    # - Ex: sudo systemctl mask <nombre-del-servicio>
    # 
    #
    # Sino simplemente se apaga y desactiva.
    # +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    

    # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    # Este proceso se ejecutara con procesos basicos que un servidor no utilizara de manera normal.
    #
    # Ejemplo:
    #
    # - bluetooth.service (Gestion de dispositivos Bluetooth [mask]).
    #
    # - cups.service (Es un servicio para servidores de impresoras [stop + disable]).
    #
    # - avahi-daemon.service (Permite Descubrir otros dispositivos en redes locales [stop + disable]).
    # 
    # - ModemManager.service (Gestiona conexiones de internet por modem [mask]).
    # 
    # - postfix.service (Es un servicio para servidores de correos[stop + disable]).
    #
    # Algunos servicios son necesarios para un servidor, asi que para ser generales
    # unicamente se apagaran y no se enmascararan por terminos de usabilidad.
    #
    # Pero esos ejemplos que mostre son los basicos que se ejecutaran, después se hara
    # una busqueda activa para detener y desactivar servicios que simplemente no se utilizen.
    # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    # --------------------------------------------------------------------------------------------
    # Eliminando el bluetooth service y enmascarandolo precisamente por ser algo innecesario
    # para un servidor que solo se maneja por terminal.
    # --------------------------------------------------------------------------------------------
    echo -e "\e[32m[INFO/CLEAN]: Descativando y enmascarando el servidor de bluetooth.service de manera automatica...\e[0m"

    if ! sudo systemctl stop bluetooth.service && sudo systemctl disable bluetooth.service; then
        echo -e "\e[1;31m[ERROR]: Ocurrio un error tratando de desactivar el servicio bluetooth.service...\e[0m"
    fi

    if ! sudo systemctl mask bluetooth.service; then
        echo -e "\e[1;31m[ERROR]: Ocurrio un error tratando de enmascarar el servicio bluetooth.service...\e[0m"
    fi

    # --------------------------------------------------------------------
    # Igualmente con ModemManager, es innecesario en un servidor debian.
    # --------------------------------------------------------------------
    echo -e "\e[32m[INFO/CLEAN]: Descativando y enmascarando el servidor de ModemManager.service de manera automatica...\e[0m"

    if ! sudo systemctl stop ModemMananger.service && sudo systemctl disable ModemManager.service; then
        echo -e "\e[1;31m[ERROR]: Ocurrio un error tratando de desactivar el servicio de ModemManager.service...\e[0m"
    fi

    if ! sudo systemctl mask bluetooth.service; then
        echo -e "\e[1;31m[ERROR]: Ocurrio un error tratando de enmascarar el servicio de ModemManager.service...\e[0m"
    fi

    # --------------------------------------------------------------------
    # Después de enmascarar los verdaderamente inutiles
    # haremos una limpieza y desactivaremos de manera
    # estandarizada servicios que no necesite pero que si los llegara
    # a necesitar estaran ahi, no eliminamos nada, solo los apagamos.
    #
    # La eliminación es total desición del usuario.
    # --------------------------------------------------------------------
    disable-services
    
    return SUCCESS
}



main () {

    local ROOT_PRIVILEGE=0

    # ------------------------------------------------------------------------
    # Verificando que el usuario haya ejecutado con permisos privilegiados
    # de su usuario el script.
    #
    # No se muestra niingun mensaje de error porque eso lo hace el programa
    # que esta en el lenguaje de programación C que interactua con el 
    # usuario en interfaz de terminal
    # ------------------------------------------------------------------------
    if [ "$EUID" -ne "$ROOT_PRIVILEGE" ]; then
        return ERR_ROOT
    fi

    clean-basic-services

    return SUCCESS
}

main


