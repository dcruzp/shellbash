#include <stdio.h> 
#include <stdio_ext.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdlib.h>
#include <fcntl.h>


#define max_lon 100 


struct command 
{
    char *cmd ; 
    char *input ; 
    char *output ; 
    char **argv ;
    int numArgv ; 
    int outappend ; 
};


char command [max_lon] ; 

/*Variables de ambiente */
char SHELL [max_lon];
char PATH [max_lon]; 
char HOME [max_lon]; 
char PWD [max_lon];  

/*Otras variables necesarias */ 
char * ptr [10]  , *token ; 
char delimit [] = " "  ;  
int flag = 1  , i ; 

char *redir [3] ; 



int matchcharacter (char c , char * str) 
{
    char str1 [] = {c, '\0'};
    if (strstr (str , str1)!=NULL)
    {
        return 1; 
    }
    return 0 ; 
}

void parser (char * input ,struct  command *cmd )
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


void initCommand (struct command * cmd )
{
    cmd-> cmd = NULL; 
    cmd-> input = NULL ; 
    cmd-> output = NULL; 
    cmd-> argv = NULL ; 
    cmd-> numArgv = 0 ; 
    cmd-> outappend = 0 ; 
} 






void printcommand (struct command * cmd )
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


int main (void) {
    

    getcwd (PWD , max_lon);
    strcpy (PATH , getenv ("PATH")); 
    strcpy (HOME , PWD) ; 
    strcpy (SHELL , PWD ) ;    

    while (flag)
    {
            
        printf("%s$ " , "Shell"); 
        __fpurge (stdin); 

        memset (command , '\0' , max_lon) ; 
        scanf("%[^\n]s" , command) ;


        struct command cmd;
        initCommand (&cmd); 

        parser (command , &cmd); 


        //printcommand(&cmd);

        if (strcmp (command , "exit") == 0 )
        {            
            flag =  0;  
        }
        else if (strcmp(cmd.cmd, "cd")==0)
        {
            if (cmd.argv[1]) 
            {
                if (chdir (cmd.argv[1])!=0)
                {
                    printf ("Error : %s  no existe o no se puede cambiar a este directorio ", cmd.argv[1] );    
                }
                else 
                {
                    getcwd (PWD ,max_lon);
                }
            } 
        }
        else
        {

            pid_t process ; 

            process = fork () ; 

            if (process < 0 ) 
            {
                perror ("fork") ; 
                return 2; 
            }
            else if (process == 0 )
            {
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
                
                execvp (cmd.argv[0] , cmd.argv ) ;  

                exit (1); 
            }
            else 
            {
                pid_t wc  = wait(NULL) ; 
            }
        }        
    }
    return 0 ; 
}

