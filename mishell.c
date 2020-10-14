#include <stdio.h> 
#include <stdio_ext.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdlib.h>
#include <fcntl.h>


#define max_lon 100 


typedef struct  
{
    char *cmd ; 
    char *input ; 
    char *output ; 
    char **argv ;
    int numArgv ; 
    int outappend ; 
}command;


char input [max_lon] ; 

/*Variables de ambiente */
char SHELL [max_lon];
char PATH [max_lon]; 
char HOME [max_lon]; 
char PWD [max_lon];  

/*bandera de control del ciclo principal */ 
int flag = 1; 

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

    input = strtok (input , "#"); 

    for (i=0 ; !matchcharacter(*(input+i), str) && *(input+i)== ' ' ; i++ ); 

    for ( len =0 ;! matchcharacter (*(input +i ) , str);i++ )
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
                cmd->outappend ++;
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


void initCommand ( command * cmd )
{
    cmd-> cmd = NULL; 
    cmd-> input = NULL ; 
    cmd-> output = NULL; 
    cmd-> argv = NULL ; 
    cmd-> numArgv = 0 ; 
    cmd-> outappend = 0 ; 
} 



command ** listcommand  (char * line , command ** cmds  , int * numb)
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

    cmds = realloc (cmds ,sizeof(command) * (num +1) ); 
    cmds [num] = NULL;
    *numb = num ;  

    return cmds ;   
} 

void printcommand ( command * cmd )
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


void execute  ( command * cmd1 , command *  cmd2)
{
    int tuberia[2]; 
    int pid1 , pid2; 
    int status1 , status2; 

    if (pipe (tuberia) == -1 )
    {
        perror ("Error al crear la tuberia ") ;
        exit (-1) ; 
    }

    if ((pid1 = fork()) == -1 ) 
    {
        perror ("Error al crear el proceso hijo  ") ;
        exit (-1) ; 
    }
    else if (pid1 == 0)
    {
        //estoy en el hijo 

        close (tuberia [0]);
        close (1);
        dup (tuberia [1]) ; 
        close (tuberia [1]);

        execvp(cmd1->argv[0], cmd1->argv); 

        exit(-1); 
        
    }
    else 
    {
        //estoy en el padre 

        close (tuberia[1]);

        if ((pid2 = fork()) == -1 ) 
        {
            perror ("Error al crear el proceso hijo  ") ;
            exit (-1) ; 
        }
        else if (pid2 == 0)
        {
            close (0);
            dup(tuberia[0]); 
            close(tuberia[0]);

            if (cmd2 == NULL)
            {
                execlp ("cat" , "cat" , NULL); 
                exit(-1); 
            } 

            execvp(cmd2->argv[0] , cmd2->argv);

            exit (-1); 
        }
        else 
        {
            close (tuberia [0]) ;
            waitpid (pid1 , &status1,0);
            waitpid (pid2, &status2, 0); 
        }        
    }
}

command **cmd;
int numb ;

int main (void) {
    

    getcwd (PWD , max_lon);
    strcpy (PATH , getenv ("PATH")); 
    strcpy (HOME , PWD) ; 
    strcpy (SHELL , PWD ) ;    

    while (flag)
    {
            
        printf("%s$ " , "Shell"); 
        __fpurge (stdin); 

        memset (input , '\0' , max_lon) ; 
        scanf("%[^\n]s" , input) ;

        cmd = NULL ;
        cmd = listcommand (input , cmd , &numb ); 
       
        
        if (numb == 1 && strcmp (cmd[0]-> cmd , "exit") == 0 )
        {            
            flag =  0;  
        }
        
        else if (numb == 1 && strcmp(cmd[0] -> cmd, "cd")==0)
        {
            
            if (cmd[0]->argv[1]) 
            {
                if (chdir (cmd[0]->argv[1])!=0)
                {
                    printf ("Error : %s  no existe o no se puede cambiar a este directorio ", cmd[0]->argv[1] );    
                }
                else 
                {
                    getcwd (PWD ,max_lon);
                }
            } 
            
        }
        else
        {
            /*
            pid_t process ; 

            process = fork () ; 

            if (process < 0 ) 
            {
                perror ("fork") ; 
                return 2; 
            }
            else if (process == 0 )
            {
                     
                /*******************esto es para la entrada y la salida del proceso *********************/    
                /*
                if (cmd.input)
                {
                    close(STDIN_FILENO);
                    open(cmd.input , O_RDONLY,S_IRUSR);
                }

                if (cmd.output)
                {
                    close (STDOUT_FILENO);

                    if (cmd.outappend) 
                    {
                        open (cmd.output ,O_CREAT|O_WRONLY|O_APPEND, S_IRWXU); 
                    }
                    else
                    {
                        open (cmd.output ,O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU); 
                    }
                        
                }
                    
                execvp (cmd[0]->argv[0] , cmd[0]->argv ) ;  

                exit (1); 
            }
            else 
            {
                pid_t wc  = wait(NULL) ; 
            }
            */
    
            execute (cmd[0] , cmd[1]) ; 
        }        
    }
    return 0 ; 
}

