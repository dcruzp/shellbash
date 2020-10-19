#include <stdlib.h> 
#include <string.h> 

/*
Estructura donde guardo un comando 
*/
typedef struct  
{
    char *cmd ; 
    char *input ; 
    char *output ; 
    char **argv ;
    int numArgv ; 
    int outappend ; 
}command;



/*
ARGUMENTOS: 
1-  c es el caracter que quieres machear 
2 - str es la cadena que puede o no contener el caracter 

DESCRIPCION:  
Esto lo que hace es comprobar si el caracter c esta en la 
cadena de caracteres str 
*/
int matchcharacter (char c , char * str);



/*
ARGUMENTOS:
1- un comando (un solo camando con su redirecciones y todo )  
2- esturctura donde se quiere guardar todo los datos de un comando 

DESCRIPCION:
Esto lo que hace en escencia es coger una cadena de caracteres (comando) 
y parcearlo guardando toda la informacion referente a un comando en la 
estructura command  
*/
void parser (char * input , command *cmd );



/*
ARGUMENTOS: 
1- una estuctura comando (que se quiere inicializar)

DESCRIPCION: 
Esto lo que hace es inicializar los valores en la estuctura comando 

Nota: 
para ver en que valores lo inicializa , revisa la implementacion que 
esta en  en el fichero parser.c 
*/
void initCommand ( command * cmd );




/*
ARGUMENTOS:
1- linea de comando entera (la que entra el usuario  del shell) 
2- arary de estucturas "command" donde se almacenana todos comandos parseados 
3- numero de comandos parseados 


DESCRIPCION:
Esto lo que hace basicamante por ahora es coger la cadena de caracteres -line- 
y la "pica" en pedazos , primero lo hace por el caracter '|' usando la funcion 
de la biblioteca string.h  strtok() y luego a cada una de las subcadenas que serian 
los "comandos" se los pasa a la funcion parcer() para que esta guarde en una estructura 
command la informacion de ese "comando" , luego esa estructura es pusheada al array 
de de estructura command -cmds- 

NOTA: 
Aclarar que -cmds- es un puntero a un array de commnds 
*/
command ** listcommand  (char * line , command ** cmds  , int * numb);