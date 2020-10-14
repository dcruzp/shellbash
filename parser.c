#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 


typedef struct  
{
    char *cmd ; 
    char *input ; 
    char *output ; 
    char **argv ;
    int numArgv ; 
}command;


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
    char * str = "<>\0"; 

    for (i=0 , len =0 ;! matchcharacter (*(input +i ) , str);i++ )
    {
        cmd->cmd = realloc (cmd ->cmd, sizeof (char ) * (len +1)); 
        cmd->cmd [len] = *(input + i ) ;
        len++;
    }
         
    for ( ; *(input + i ) != '\0' ; )
    {   
        if (*(input + i ) == '>')
        {
            
            if (*(input + i + 1 ) == '>')
            {
                printf ("%s\n" , "check") ; 
                i++;
            }
            for (i++ ,len =0 ; ! matchcharacter (*(input +i) ,str) ; i++ )
            {
                if (*(input + i ) == ' ')continue; 
                cmd->output =  realloc (cmd->output , sizeof (char) * (len + 1));
                cmd->output [len] = * (input+i) ; 
                len ++ ; 
            }
        }
        if (*(input+i) == '<')
        {
            
            for (i++ , len = 0 ; ! matchcharacter (*(input +i ),str ); i++ )
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
    
    
    while (token != NULL)
    {
        cmd->argv = realloc (cmd->argv , sizeof (char * ) * (cmd ->numArgv + 1 ));
        cmd->argv [cmd->numArgv] = token ; 
        cmd->numArgv ++ ; 
        token = strtok (NULL , delimit);
    }

    cmd->argv = realloc (cmd->argv , sizeof (char*)* (cmd->numArgv + 1));
    cmd->argv [cmd->numArgv] = NULL ; 

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


void parsercommand (command * cmd)
{
    char * token ,delimit [] = " "  ; 
    token = strtok( cmd->cmd  , delimit);
    
    
    while (token != NULL)
    {
        cmd->argv = realloc (cmd->argv , sizeof (char * ) * (cmd ->numArgv + 1 ));
        cmd->argv [cmd->numArgv] = token ; 
        cmd->numArgv ++ ; 
        token = strtok (NULL , delimit);
    }

    cmd->argv = realloc (cmd->argv , sizeof (char*)* (cmd->numArgv + 1));
    cmd->argv [cmd->numArgv] = NULL ; 
}

void initCommand ( command * cmd )
{
    cmd-> cmd = NULL; 
    cmd-> input = NULL ; 
    cmd-> output = NULL; 
    cmd-> argv = NULL ; 
    cmd-> numArgv = 0 ; 
} 



command ** partirredireccion  (char * line , command ** cmds  , int * numb)
{   
    char **comandos = NULL ; 
    char *token = NULL; 
    char delimit [] = "|";
    int num = 0 , i ; 
    command * aux ;

    token = strtok (line , delimit); 
     
    while (token != NULL) 
    {
        comandos = realloc (comandos , (sizeof (char * )) *(num +1 ) ); 
        comandos [num] = token ; 
        token = strtok (NULL , delimit); 
        num ++ ; 
    }

    for (i=0 ; i< num ; i++)
    {
        aux = NULL ; 
        aux = realloc (aux , sizeof (command) ) ; 
        initCommand (aux); 
        
        parser( *(comandos + i ) , aux); 
        
        cmds = realloc (cmds , sizeof (command) * (i+1)); 
        cmds[i] = aux ; 
    }

    printf("%s\n" , cmds[0]->cmd); 

    *numb = num ;  

    return cmds ;   
} 

int main (void) 
{
    
    /*
    char *command = " ls -l /usr/bin < input.txt > output.txt"; 
    
    struct command cmd ;
    initCommand (&cmd); 

    parser (command , &cmd) ;

    parsercommand (&cmd) ; 
    
    //printcommand (&cmd);  
    */


    char line[] = "grep foo file | wc -l" ;
    command ** list; 
    int numb ; 
    
   
    list  = partirredireccion (line, list , &numb); 

    if (list == NULL ) printf ("es NULL"); 
    printf ("%s\n" , list[1] -> cmd ) ; 
    return 0 ; 

} 