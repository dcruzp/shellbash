#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "parser.h"


/*
ARGUMENTOS:
1- comando que se quiere ejecutar 

DESCRIPCION: 
esta funcion ejecuta un comando en especifico (un solo coamndo )
se crea un fork y ahi mando a ejecutar el comando , en caso de que
no se pueda ejecutar entonces retorna exit status 0 
*/
void execute (command *cmd);



/*
ARGUMENTO:
1- comando que se quiere ejecutar 

DESCIPCION:
esta funcion ejecuta dos comando que que estan unidos por un pipe 
en caso de que alguno de los dos no se pueda ejecutar se terminan con 
exit status -1 , 
si alguna de las redirecciones de los comando no se puede abrir entonce 
se muesta el error correspondiente y se termina igual con exit status (-1)  
*/
void execute1 (command * cmd1 , command *cmd2);