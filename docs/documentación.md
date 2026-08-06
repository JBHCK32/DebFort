

# Proyecto de Hardening en Debian(DebFort):

## Intruducción:

Este proyecto fue creado para hacer un hardening basico donde el usuario que lo usa y lo ejecute
esta aceptando tener lo minimo he indispensable en su servidor y de paso eliminar area de ataque en le mismo.

La idea es que DebFort aplique scripts estandarizados y funcionalidades que son estandar en la seguridad de sistemas.

## Scripts:

Los scripts de DebFort aplican configuraciones estandarizadas para aumentar la seguirdad del servidor en diferentes ambitos,
ya sean internos y de red. Un ejemplo de ello logra ser `ssh-conf.sh` en el cual aplica una configuración totalmente 
automatizada en la cual edita un archivo de configuración de el protocolo ssh y aplica solo cambios estandar
en acceso y seguirdad del mismo protocolo.

Tambien los scripts llegan a en globar una seguridad interna del servidor al aplicar el script ` cleaning-services.sh `
donde se desactivan y enmascaran puertos innecearios que un servidor son solo terminal en debian no vaya a utilizar,
igualmente se dejan los servicios utiles para servidores desactivados de manera automatica, el usuario puede
usarlos cuando guste y los servicios que se dejaron desactivados estan en el archivo `disable-services[Spanish].md`.

Se habla de algunos servicios pero en general todos se dejaron desactivados por lo mismo nada más que no puede profundizar
en las implicaciones de los demás en terminos de seguridad.

El script `conf-firewall.sh` tiene su razón de estar ahi, además que es muy util para reducir superficie de ataque en red,
tambien deja al usuario con lo minimo indispensable, en donde le da una muy buena base en donde el propio usuario puede
iniciar sus propios puertos y reglas de iptables. 

De modo general, todos los scripts estan hechos para que solo lo ejeute el super usuario con permisos necesarios, 
esto esta hecho de esta manera para que el root o grupo admin pueda ejecutar estos scripts a la hora de usar el binario
de DebFort. 

Estos scripts son utiles en el proyecto no solo porque automatizan acciones simples y evitan sobre-ingenieria en el proyecto.

## Binario:

Este esta dividido en 4 partes que se unen en un solo binario y que se comunican todo a traves de librerias internas.

Todo el codigo en sí esta auto-documentado ya que el propio codigo trae comentarios explicativos de desiciones de
diseño y explicaciones "tecnicas" en donde se explica de manera simple y detallada como funcionan las partes más
importantes del mismo.

### close-unused-ports:

Aqui recide el sistema de clausula de puertos, contiene varios archivos que conforman la libreria en si.
Pero los importantes son el que cierra una lista de puertos de manera automatica de los servicios que aparecen
en `cleaning-services.sh`, uso esa lista y lo unico que hago es cerrar los puertos que usaban esos servicios.

El archivo `close-ports.c` tiene unicamente 2 funciones que sirven como API's para el usuario y otra API
para el archivo que describimos anteriormente que fue `automatic-list-ports.c`. Estas 2 funciones simplemente
son un intermediario a una función privada que ejecuta codigo con para abrir un socket, conectarlo al puerto
que el usuario quiera abrir y cerrar el socket y el puerto. Todo esto lo hace por medio de la libreria `<sys/socket>` de linux
y otras más.

Por ultimo el .h unicamente otorga las librerias ensenciales para que todo el codigo se utilize correctamente
como lo puede ser `<stdbool.h>` o `<stdint.h>` que son cosas basicas que necesito para trabajar yo de manera comoda.


### services-uninstall:

En esta libreria contiene el codigo en C que ayuda al sistema a poder desactivar servicios, solo desactivarlos aunque
en el futuro podria ser bueno agregarle una función para tambien enmascarar servicios pero por ahora a fecha `5/8/26` es
más que suficiente.

El archivo `disable-services.c` hace lo que su nombre indica, otorga API's para que el programa pueda desactivar
servicios de manera sencilla en donde solo tenga que verificar que el proceso de desactivación haya ocurrido de manera
correcta, asi son todas las API's ya que cada libreria en sí tiene si propio codigos y gestor de errores en donde 
se le muestra un mensaje de error especifico he informativo para cada uno; este sistema de errores tambien aplica para cada libreria
del proyecto.

El archivo `get-services.c` es un archivo simple que ayuda a la interfaz grafica a poder mostrar los servicios que tiene activos.
La función privada llamada `Get_Services` manipula una lista de cadenas de texto en la cual pone en el orden que 
devuelve el comando para poder asi mostrarselo de manera simple en la interfaz grafica.

Además de la API grafica que tiene para poder mostrar los servicios, tiene otra que manipula de manera directa la cadena
de cadenas de texto que le pasen y solo te deja con la lista de servicios y no hace nada más(solo algunas verificaciones
pero es algo basico). Esta función es útil porque se necesita en la verificación de que lo que escriba el usuario
realmente sea un servicio existente que no este desactivado o enmascarado.

Y  por utlimo la libreria que tiene solo librerias basicas para poder trabajar con comodidad en el proyecto.

### main-UX:

Esto se puede resumir en que es la parte con la cual el binario interactua con el usuario, solo que esta un poco incompleta
al no tener el `errs.c` totalmente desarrollado pero en generla la interacción con el usuario es correcta y eso solo afecta
en que se le muestren errores al usuario o no.

El archivo `main.c` es el menú principal donde simplemente le da al usuario una visión general de lo que puede configurar.
Me gustaria incluso añadir en el banner algo que diga de manera resumida que hace cada cosa, pero supongo que los mensajes
el selecionar una opción son suficiente, pero igual es una idea que tal vez tenga en cuenta en el futuro.

El archivo `menu.c` es simplemente el ejecutor principal, donde se muestra el mensaje informativo de lo que hace cada cosa
y después de procesar la desición del usuario ejecuta el menú interactivo o no.

El archivo `input-controller.c` es un poco extraño, porque solo contiene una función que es para procesar el input del usuario
de manera segura y general, sirve unicamente para cadenas de texto pero lo decidi hacer asi porque no veia en donde meterlo
sin que quedara raro o desordenado, por eso mismo le hice un archivo en donde guarde esa función y que es una de las más
importantes de la libreria ya que permite que todo el programa pueda obtener y verificar el input del usuario de manera totalmente
segura y sin desbordamientos de memoria.

El archivo `interactive-side.c` contiene los menús y su parte interactiva, donde se verifican rangos, opciones y demás.
En general es una de las más importantes y de las más documentadas que tengo simplemente porque es el hilo que conecta las demás
librerias que acabamos de ver con el usuario.

Y la libreria final donde todas en general siempre tiene casi lo mismo donde se declaran macros de errores y codigos de salida, 
declaración de librerias para todo el proyecto y declaración de funciones que los demás quieran usar de la libreria.


### Conclusión:

En general tome la desición de hacer asi el proyecto tomandeme tiempo para organizar, no pensaba en codigo al inicio sino
que más bien pensaba en estructura del proyeto y logre esta estructura por mi cuenta por primera vez.

Fue un proyecto que me ayudo a mejorar como programador, no tan solo en calidad de codigo sino en estructura de proyectos,
documentación con criterio y aprender a orednar el codigo por librerias en vez de tener todo en un solo archivo hace
que trabajar en el mismo sea más comodo no solo para mi sino tambien a cualquiera que participe en el mismo.

Pues al final esto es una aplicación/sistema que se compila y se utiliza en conjunto, pero no deberia ejecutarse por separado
los scripts, además de que se ejecutan con superusuario por seguridad son scripts muy especificos y el como usarlo esta
dentro del script como un comentario donde se explica como usarlo. Mi recomendación del uso normal es copiar el repositorio,
instalar las dependencias que son muy pocas y ejecutar script de compilación y ejecución del programa.
