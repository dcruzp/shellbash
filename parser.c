#include "parser.h"


void initCommand ( command * cmd )
{
    cmd-> cmd = NULL; 
    cmd-> input = NULL ; 
    cmd-> output = NULL; 
    cmd-> argv = NULL ; 
    cmd-> numArgv = 0 ; 
    cmd-> outappend = 0 ; 
} 

int matchcharacter (char c , char * str) 
{
    char str1 [] = {c, '\0'};
    if (strstr (str , str1)!=NULL)
    {
        return 1; 
    }
    return 0 ; 
}

void parser (char * input , command *cmd )
{
    int i  , len ;  
    char * str = "#<>\0"; 

    for (i=0 ; !matchcharacter(*(input+i), str) && *(input+i)== ' ' ; i++ );  /*correr el indice hasta encontrar el primer
                                                                                caracter distinto del caracte ' ' */


    for ( len =0 ;! matchcharacter (*(input +i ) , str);i++ )    /*leo el comando y lo voy almacenando en -cmd- caracter 
                                                                    a caracter*/
    {
        cmd->cmd = realloc (cmd ->cmd, sizeof (char ) * (len +1)); 
        cmd->cmd [len] = *(input + i ) ;
        len++;
    }
         
    for ( ; *(input + i ) != '\0' ; )  /*ahora recorro todos los caracteres hasta encontrarme 
                                        el caracter de fin de linea*/ 
    {   
        if (*(input + i ) == '>') 
        {
            
            if (*(input + i + 1 ) == '>')   /*cuando llege aqui es que se encontro un 
                                            redireccionamiento de la salida que que se 
                                            debe concatenar el final del fichero al que 
                                            se redirecciona la salida del comando */
            {
                cmd->outappend ++;
                i++;
            }
            for (i++ ,len =0 ; ! matchcharacter (*(input +i) ,str) ; i++ )  /*aqui leo el nombre del fichero al que 
                                                                            se quiere redireccionar la salida */ 
            {
                if (*(input + i ) == ' ')continue; 
                cmd->output =  realloc (cmd->output , sizeof (char) * (len + 1));
                cmd->output [len] = * (input+i) ; 
                len ++ ; 
            }
        }
        if (*(input+i) == '<')
        {
            
            for (i++ , len = 0 ; ! matchcharacter (*(input +i ),str ); i++ )/*aqui leo el nombre del fichero del que  
                                                                            se quiere redireccionar la entrada el programa */ 
            {
                
                if (*(input + i ) == ' ') continue ; 
                cmd->input =  realloc (cmd->input , sizeof (char) * (len + 1 ));
                cmd->input [len] = * (input+i) ;
                len ++ ;  
            }
        }
    }


    char * token ,delimit [] = " "  ;     
    
    token = strtok( cmd->cmd  , delimit);  
    while (token != NULL)                 /*en este ciclo separo todos lso argumentos y los pongo en el array 
                                            el array de string al que apunta argv  
                                            NOTA : Aqui el primer elemento de este array es el nombre del comando 
                                            y seguido estan los argumentos de este  */
    {
        cmd->argv = realloc (cmd->argv , sizeof (char * ) * (cmd ->numArgv + 1 ));
        cmd->argv [cmd->numArgv] = token ; 
        cmd->numArgv ++ ; 
        token = strtok (NULL , delimit);
    }

    /*
    Al final del array de string  -argv-  pongo NULL para que se pueda 
    ejecutar correctamente con execvp() pasandole los argumentos 
    */
    cmd->argv = realloc (cmd->argv , sizeof (char*)* (cmd->numArgv + 1));
    cmd->argv [cmd->numArgv] = NULL ; 

} 

command ** listcommand  (char * line , command ** cmds  , int * numb)
{   
    /*
    Nota:
    Aqui no se si guardar lo que sigue dando strtok (), que el lo que 
    va despues del caracter '#' (eso no se usa ) , pero se puede volver
    a llamar a strtok (NULL , '\0') y esto te va a devolver un puntero a 
    la cadena que le sigue al caracater '#'  
    */
    line = strtok (line , "#"); 


    /*estas son las variables necesarias*/
    char **comandos = NULL ; 
    char *token = NULL; 
    char delimit [] = "|";
    int num = 0 , i ; 
    command * aux ;


    /*
    en -comandos- pongo todos los "comandos" , (es decir : lo que es para mi un comando ) 
    "pico" la cadena de caracteres -line- por el caracter '|'  
    Nota : Aqui -comando- es un puntero a un array de array de caracteres 
    */
    token = strtok (line , delimit);      
    while (token != NULL) 
    {
        comandos = realloc (comandos , (sizeof (char * )) *(num +1 ) ); 
        comandos [num] = token ; 
        token = strtok (NULL , delimit); 
        num ++ ; 
    }


    /*
    ahora por cada "comando" que hay en el array de string -comandos-
    reservo memoria para un puntero a una estuctura comando y llamo a 
    la funcion parser para tener todo bien almacenado en un array de 
    estuctura -command-    
    */
    for (i=0 ; i< num ; i++)
    {
        aux = NULL ; 
        aux = realloc (aux , sizeof (command) ) ; 
        initCommand (aux); 
        
        parser( *(comandos + i ) , aux); 
        
        cmds = realloc (cmds , sizeof (command) * (i+1)); 
        cmds[i] = aux ; 
    }

    /*
    al final de array de -commands- que es el que esta apuntado por el puntero 
    -cmds- creo una referencia mas a NULL 
    */
    cmds = realloc (cmds ,sizeof(command) * (num +1) ); 
    cmds [num] = NULL;
    *numb = num ;  

    return cmds ;   
} 

void printcommand (command * cmd )
{
    printf ("comando-> %s\n" , cmd->cmd );
    printf ("input  -> %s\n" , cmd->input );
    printf ("output -> %s\n" , cmd->output); 

    printf ("Arguments:\n");
    int i ; 
    for ( i= 0 ; i< cmd ->numArgv ; i++)
    {
        printf ("%s\n", cmd->argv [i]); 
    }
} 


/*
int main (void) 
{
    char line[] = "grep foo file | wc -l" ;
    command ** list; 
    int numb = 0 ; 
    
   
    list  = listcommand (line, list , &numb); 

    int i ; 
    for ( i = 0 ; i < numb ; i++)
    {
        printcommand(*(list +i));
    }
    return 0 ; 
} */