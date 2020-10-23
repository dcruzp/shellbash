# Proyecto de Sistemas Operativos


### Preambulo

La idea del proyecto es hacer un ejecutable capaz de simular el lenguaje bash . El proposito general  es  tener conocimientos acerca del manejo de los procesos del sistema operativo . Para eso voy a listar algunos de los apartados en los que tuve que profundizar para pode hacer este proyecto 

 * creacion de nuevos procesos con la funcion fork()
 * comunicacion entre procesos (manejo de senales )
 * manejo de descriptores de fichero y redireccionamiento de la salida y entrada estandar de datos de un proceso
 * lectura y esritura de ficheros usando las funciones open () , read () , lseek () 


### Requerimientos 

El programa esta hecho completamente en el lenguaje c . Por lo que si tenemos el compilador gcc 
instaldo en nuestro sistema podemos compilarlo y correrlo 

 * pagina para tener mas informacion del compilador y poder instalarlo correctamente  https://gcc.gnu.org/


### Como ejecuto el programa

Una vez en el directorio ejecutar la sentencia siguiente:

```
./run
```

### Idea general de como funciona

* Primero tenemos el fichero mishell.c donde esta el main , Aqui tenemos un ciclo que termina cuando se introduce el coanod exit  

* En el fichero include.c se encuentra practicamente todo el codigo para que funcione el programa . Las funciones para el parser , el execute del shell

* En el history.c tenemos todo lo relacionado al comanod history . Este funciona de manera independiente del prgrama principal . De hecho cuando se ejecuta , es porque es llamado con una funcion exec() para ser ejecutado el ejecutable que esta genera

* En el directorio help/ esta todo lo relacionado el help . este comando es ejcutado de manera similar al history 



### Auda

Aqui esta la informacion de algunos commandos 

1. help

    * Integrantes: Daniel de la Cruz y Victor M. Lantigua

        - Funcionalidades:
	        + basic: Funcionalidades básicas. (3 ptos)
	        + multi-pipe: Admitir múltiples tuberías. (1 pto)
	        + spaces: Admitir cualquier cantidad de espacios entre los comandos y parámetros. (0.5 ptos)
	        + history: Imprimir los últimos 10 comandos. (0.5 ptos)
	        + chain: Encadenar varios comandos. (0.5 ptos)
	        + if: Expresiones condicionales. (1 pto)
	        + multi-if: Múltiples condicionales en una sola línea. (0.5 ptos)
	        + help: Imprimir una ayuda. (1 pto)



        - Comandos built-in:
	        + cd: Cambia de directorio.
	        + exit: Sale del shell.
	        + history: Imprime los últimos 10 comandos.
	        + again: Ejecuta un comando del history.
	        + true: Se evalua como true.
	        + false: Se evalua como false.
	        + if: Comando para control de flujo.
	        + help: Imprimir una ayuda.

2. multipipe

    * Este shell permite el uso de múltiples tuberías, de la forma: 
      cmd [arg]* [| cmd [arg]*]*

    * Para lograr esto tratamos todo tipo de expressiones entre chains  como una expression
    de tipo Pipe_S que no es más que una cola de expressiones, las cuales se iran ejecutando
    a medidad que salgan de la cola y redirigiendo sus salidas y entradas a través de pipes.

    * Un comando simple también será tratado como un Pipe_S pero al ser el último comando en la
    cola su salida no será redirigida a través de un pipe, a su vez es también el primer
    comando de la cola por lo que su entrada tampoco será dirigida a través de un pipe.

    * Si entre pipes se encuentra algún redirecionamiento este será tomado como la salida del
    comando correspondiente y no sera redireccionado hacia el pipe, por lo que el siguiente
    comando tendrá una entrada vacía. Si el comando después del pipe tiene redireccionamineto
    de entrada ocurrirá parecido a lo anterior, no se tomara el pipe com entrada, sino, el
    redireccionamiento correspondiente al comando.

 3. basic 

    * Funcionalidades básicas del shell:

    * -Al ejecutar el shell se imprime un pront terminado con el signo $ y un espacio a contnuación
    del cual el usuario puede insertar una línea de comando.

    * -Al insertar un comando y presionar la tecla Enter este comando será ejecutado y su salida será
    mostrada(en la salida estandar pro defecto).

    *  -El shell es capaz de ejecutar el comando cd y cambiar el directorio de trabajo actual.
    (Para más información sobre cd ejecutar: help cd).

    * -El shell es capaz de redirigir la entrada y la salida con los operadores <, >, >>.
        + "<" - redirecciona la entrada del comando al fichero que se especifique a continuación.
        + ">" - redirecciona la salida del comando al fichero que se especifique a continuación,
        si este fichero existe entonces será sobreescrito y en caso contrario será creado.
        +  ">>" - funciona parecido a > pero si el fichero existe no lo sobreescribe, sino que agrega
        el nuevo contenido al final de este.

    * -Se puede hacer uso de tuberías con el operardor | , el cual conecta la salida del comando
    a su izquierda con la entrada del comando a su derecha.

    * -Los elementos de la línea de comandos a ejecutar deben estar separados por espacios, en el
    caso específico de este shell la cantidad no importa.

4. chain 

    * Los chains permiten encadenar varias expresiones mediante los operadores ';', '&&' y '||'.

        + ';' Permite ejecutar varias expresiones independientes en una misma linea, o sea, el resultado
        de las expresiones separadas por este operador no tienen efecto ninguno sobre la otra, al menos
        no directamente.

        + '&&' Permite poner 2 (o más) expresiones unidas de forma que, el comando N solo se ejecuta si 
el comando N-1 tiene un exit status satisfactorio (cero). O sea, funcionacomo el operador 
AND lógico en corto circuito. Luego el resultado de la cadena será true solo si ambas
expresiones concluyeron satisfactoriamente.

        + '||' Funciona como el operador OR lógico en corto circuito. Si la primera expresion concluye de forma
satisfactoria, no hace falta ejecutar la 2da.Y sólo si la 1ra expression “falla” (exit status distinto
 de cero) es que se ejecuta la 2da expresion. Siendo el valor de retorno de la cadena, el valor de 
 retorno de la última expresion que se ejecuta.

    * En nuestra implementación los chains son tratados como expresiones al igual que los pipes, de esta
    forma ejecutamos toda una linea de forma secuencial mientras esto sea posible(mientras la lógica del
    shell permita continuar la ejecución de la línea), esto se logra auxiliandonos de flags para saber el
    último exit status y que tipo de chain estamos evaluando, en caso de no ser ningún chain, la flag NONE
    lo indica.

5. exit

    *  El comando exit termina inmediatemente la ejecución del shell, esto lo implementamos enviando una
    señal SIGINT al shell, de modo que esta no pueda ser capturada.

6. false

    * Este comando especial solo devuelve false(exit status 1).

7. true

   * Este comando especial solo devuelve true(exit status 0).

8. spaces

    * Todos los espacios en shell serán ignorados a no ser que estos se encuentre entre algún
    tipo de comillas.
    Esto se logra al tomar la linea de entrada del shell y tokenizarla, o sea, convertirla en tokens
    validos analizandola caracter a caracter e ignorando en este proceso todos los espacios que no estén
    dentro de una expresion de comillas.

9. if

    * El if permite ejecutar comandos o secuencias de comandos de forma condicional en una sola linea.

    * La instrucción if siempre debe estar acompañada de su respectivo then y end. Los comandos depués
    de la instrucción end se ejecutarán solo si la expresión que se encuentra después del if resulta
    true, o sea, exit status 0. Opcionalmente se puede agragar una sentencia else entre then y end,
    esta se ejecutará en caso de que la expresión después del if resulte con exit status 1.

    * La sentecia if a su vez proporcionará también un exit status, que será el de la expresion que
    ejecute, then o else según corresponda. En caso de que la expresión después del if retorne exit
    status 1 y esta carezca de una sentencia else que ejecutar el exit status general de la expresión
    if será exit status 0 por defecto.

10. multi if

    * El shell permite la ejecución de múltiples if en una sola línea, de forma que estos pueden incluso
    estar anidados.

#### direccion del repositorio

* direccion donte esta actualmente el repositorio  https://github.com/dcruzp/shellbash