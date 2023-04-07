## Requisitos:
- Tener instalado:
- - El editor [VSCode](https://code.visualstudio.com/). Viene en la VM pero se recomienda actualizar a [la ultima version son soporte a 32bit](https://code.visualstudio.com/updates/v1_35)
- - La extensión [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) de VSC
- - La [commons library de la cátedra](https://github.com/sisoputnfrba/so-commons-library)
- - (Altamente recomendado) [CUnit](https://mysnippets443.wordpress.com/2015/03/07/ubuntu-install-cunit/) para no desvelarse retesteando a mano el dia anterior a la entrega :smile:
- - GNOME Terminal (__sudo apt-get install gnome-terminal__)

## Importante:


Cuando lo queremos abrir, vamos a hacer **Archivo->Abrir Área de Trabajo**, seleccionando ese .code-workspace, y nos va a abrir todas las carpetas de una con sus configuraciones.


En __src__ van a estar nuestros __.c__, en __include__ nuestros __.h__, en __cfg__ nuestras __configuraciones y logs__, y en __obj__ van a estar los __.o__ (no vamos a interactuar con éstos).



### Explicación:
Los makefiles constan de macros y reglas.
Las macros tienen el formato:

    MACRO=valor 
 
 y podemos poner paths, comandos, nombres, includes y mucho más, evitando repetirlo después y referenciándolo con el formato $(MACRO).
A su vez, los makefiles tienen *reglas*, que tienen el formato:
    
    target: dependencias
        acción

Donde acción debe tener un __TAB__ sí o sí. __Target__ es el nombre de la regla que podemos usar en la terminal (make clean, make install, etc), __Dependencias__ son las que necesito para ejecutar la acción, como los .o que necesito para armar el ejecutable. __Acción__ es el comando que quiero ejecutar, como gcc 
con los argumentos y las dependencias. Este makefile esta hecho con -> [esta guía](https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/) y si querés modificarlo a tu gusto te invito a leer [la documentación](https://www.gnu.org/software/make/manual/make.html).    

__LIBS__ es un macro donde van las librerías que vamos a usar. __-lcommons__ es la de la cátedra, __-cunit__ es para los tests unitarios (optativo), y los demás los necesité en su momento. Agregá o sacá de acuerdo a tus necesidades.

__CFLAGS__ es un macro donde van los flags al compilar. __-g es necesario para ejecutar en VSCode en modo debug__. __-Wall__ es para mostrar todos los warnings. Te recomiendo no sacar ninguno.

Este makefile nos va a permitir que el procesoN compile utilizando los .c y .h del proceso más los que están en la shared. Acá está la trampita; no es una biblioteca compartida sino un lugar donde los procesos saben que tienen que ir a buscar el resto de los archivos.


Si hacemos _make_ vamos a ver que se nos creó el ejecutable. __Hurra!__

    >make && ./proceso1
    make: 'procesoAlgo' is up to date.
    [INFO] 17:14:17:572 PROCESO1/(16977:16977): Soy el proceso Algo! Hice uso de la shared!

## Parte 3: Scripts de uso común

A partir de esto, en la terminal podemos escribir __./exec__ o __./vexec__ o __./hexec__ para Compilar y Ejecutar el proceso en un solo paso, ya sea normalmente o con valgrind. Optativamente podemos agregar un script para helgrind:


__Si queremos que esté más ordenado, podemos agregar --log-file=a.out antes de ./$FILE y así lo importante va a irse a un archivo aparte__ 

__Para ejecutar estos scripts, pero vamos a tener que darnos permiso de ejecución. Para esto vamos a escribir__ 

    chmod 777 exec

y así con vexec y hexec. Pero en VSC todavía nos faltan algunas cosas. **Nota: No hacer esto fuera de la VM o del TP**


## Parte 4.2: Configurando VSCode (Debug, Tareas, Shortcuts)


Vamos a la parte de __Debug__ de VSC (bichito a la izquierda en el IDE), y arriba donde aparece "DEPURAR" vamos a abrir el combo box y seleccionar *Agregar Configuración (procesoN)*. Seleccionamos C++ (GDB/LLDB), y "Launch".

Ahora ya podemos Debuggear nuestro proyecto! Si vas al bichito nuevamente y seleccionas *ProcesoN*, apretando en el triángulo verde o f5 vas a poder debuggear. Podés poner breakpoints para ver lo que se imprime por consola y demás. Tené en cuenta que debuggear **no compila tu aplicación, así que lo vas a tener que hacer manualmente**. Despues del proximo paso vamos a ver como arreglar eso


Pero eso no es todo...  Para agregar una nueva tarea para *helgrind* podemos hacer ->

Vamos a ir a Tareas-> Configurar Tareas->Crear archivo tasks.json desde plantilla (*proceso1*)->Others

Se nos abre un tasks.json donde vamos a poder decirle a VSCode qué queremos hacer. Vamos a agregar tres tareas: una para compilar, una para ejecutar normal y otra para valgrind (4 si querés hacer para helgrind). Reemplazamos los labels por el nombre de cada tarea, shell se queda igual, y en command vamos a poner ./exec o ./vexec respectivamente. Nos queda algo así:

    {
        "version": "2.0.0",
        "tasks": [
            {
                "label": "Make",
                "type": "shell",
                "command": "make",
                "problemMatcher": [
                    "$gcc"
                ]
            },
            {
                "label": "Proceso1 Normal",
                "type": "shell",
                "command": "./exec"
            },
            {
                "label": "Proceso1 Valgrind",
                "type": "shell",
                "command": "./vexec"
            }
        ]
    }

Ahora, podemos ir a **Tareas->Ejecutar Tarea->ProcesoN Normal->(gcc)** para ejecutarlo directo desde el IDE, o Proceso1 Valgrind para ejecutarlo con Valgrind! Ah y **se compila automáticamente cada vez que lo corrés** :smile:

>*Qué fiaca. son como 3 clicks.*

Vamos a ir a __Archivo->Preferencias->Métodos abreviados de teclado__ y apenas abajo del buscador hacemos click en *keybindings.json*.

Ahora vamos a linkear nuestras tareas creadas a keybinds. Copiamos lo siguiente:

    [
        {
            "key": "ctrl+h",
            "command": "workbench.action.tasks.runTask",
            "args": "ProcesoN Normal"
        },
        {
            "key": "ctrl+j",
            "command": "workbench.action.tasks.runTask",
            "args": "ProcesoN Valgrind"
        },      
    ]

En vez de ctrl+h y ctrl+j ponés tu combinación favorita. Command se queda como está, y **en args ponemos el nombre *exacto* de la tarea, tal como aparece en label dentro de tasks.json**.

Pumba! Ahora con una combinación de teclas podemos ejecutar el proceso las veces que queramos, desde el IDE. No más terminal, no más Eclipse, todos felices. 

Podés hacer esto mismo con el resto de los procesos y decirle a tus compañeros que se hagan sus propios keybinds (ya que es probable que una persona se dedique a un sólo proceso en el TP)

Ahora tambien podemos hacer que se ejecute la tarea de compilacion que creamos cuando empezamos a debuggear. Abrimos para todos los procesos el archivo launch.json y agregamos esta linea:

            "preLaunchTask": "Make",

## Parte 5: Testeo Unitario y el retorno de PdeP


Nuestro makefile ya tiene para usar CUnit. 

De esta forma podemos correr **./procesoN -test** para correr nuestros tests. [**Te recomiendo empezar testeando que se serialicen / deserialicen bien tus mensajes**](https://docs.google.com/document/d/190LLBTzHkFmD_yS8OWcUsJoIOQVKxJkUxIFtcnChr-g/edit) :smile:

De la misma forma, podemos ir a nuestro __launch.json__ y en __args__ agregar "-test" para correr nuestros tests en debug. También podemos crear tareas como en las ejecuciones normales y agregarle shortcuts. Lo que te haga feliz.

    >./proceso1 -test


    CUnit - A unit testing framework for C - Version 2.1-3
    http://cunit.sourceforge.net/


    Suite: PROCESO1 Suite
    Test: Probar Suma ...passed

    Run Summary:    Type  Total  Ran Passed Failed Inactive
                suites     1      1    n/a      0        0
                tests      1      1      1      0        0
                asserts    1      1      1      0      n/a

## Parte 6: Deployment para entrega final


Adaptado un script de [acá](https://github.com/sisoputnfrba/so-deploy) para que sea más fácil de usar. 

##### ./deploy.sh

Listo el pollo. Ejecutas esto en la VM nueva y ya estás listo para arrancar.
