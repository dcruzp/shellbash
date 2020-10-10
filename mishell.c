#include <stdio.h> 
#include <stdio_ext.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdlib.h>
#include <fcntl.h>


#define max_lon 100 


#define ABUF_INIT {NULL ,0}

struct abuf 
{
    char *b ; 
    int len ; 
}abuf;


struct command 
{
    struct abuf cmd ; 
    struct abuf input ; 
    struct abuf output ; 
    struct abuf *argv ;
    int numArgv ; 
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


void abAppend (struct abuf *ab , const char *s , int len) 
{
    char *new = realloc(ab->b , ab->len + len);
    
    if (new == NULL) return ; 
    memcpy (&new [ab->len], s, len ) ; 
    ab->b = new ; 
    ab ->len += len; 
} 

void abFree (struct abuf * ab )
{
    free (ab->b) ; 
}




void parser (char * input ,struct  command *cmd )
{
    int i  ;  

    for (i=0;*(input + i ) != '\0' && *(input + i ) != '>' && *(input +i) != '<' ;i++)
        abAppend (&cmd->cmd ,(input+i),1);

    for ( ; *(input + i ) != '\0' ; )
    {   
        if (*(input + i ) == '>')
        {
            
            if (*(input + i + 1 ) == '>')
            {
                printf ("%s\n" , "check") ; 
                i++;
            }
            for (i++ ; *(input + i ) != '\0' && *(input + i ) != '>' && *(input +i ) != '<' ; i++)
            {
                if (*(input + i ) == ' ')continue; 
                abAppend (&cmd->output, (input +i ) , 1) ;  
            }
        }
        if (*(input+i) == '<')
        {
            for (i++ ; *(input + i ) != '\0' && *(input + i ) != '>'  && *(input +i ) != '<' ; i++)
            {
                if (*(input + i ) == ' ')continue; 
                abAppend (&cmd->input, (input +i ) , 1) ;  
            }
            
        }
    }
} 

void parsercommand (struct command * cmd)
{
    char * token ,delimit [] = " "  ; 
    cmd->numArgv = 0 ;
    token = strtok( cmd->cmd.b  , delimit);
    
    
    while (token != NULL)
    {
        cmd->argv = realloc (cmd->argv ,sizeof(abuf) * (cmd->numArgv +1 )) ; 
        abAppend(&cmd->argv[cmd->numArgv], token , strlen (token)); 
        token = strtok(NULL , delimit);
        cmd->numArgv ++ ;  
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

        /*
        token = strtok (command , delimit) ; 

        i=0; 
        while (token != NULL) 
        {
            ptr [i++ ] = token ; 
            token = strtok (NULL , delimit) ;  
        }  
        ptr[i] = NULL; 
        */


        struct command cmd = {ABUF_INIT,ABUF_INIT ,ABUF_INIT , NULL , 0  }; 

        parser (command , &cmd); 

        parsercommand (&cmd); 

        if (strcmp (command , "exit") == 0 )
        {            
            flag =  0;  
        }
        else if (strcmp(cmd.cmd.b, "cd")==0)
        {
            if (cmd.argv[1].b) 
            {
                if (chdir (cmd.argv[1].b)!=0)
                {
                    printf ("Error : %s  no existe o no se puede cambiar a este directorio ", cmd.argv[1].b );    
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

                if (cmd.output.b )
                {
                    
                    close (STDOUT_FILENO);
                    open (cmd.output.b ,O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU); 
                }

                char * arg [cmd.numArgv+1] ; 
                int i ; 
                for (i = 0 ; i< cmd.numArgv ; i++ )
                {
                    arg [i] = cmd.argv[i].b; 
                }
                arg[i] = NULL ;



                execvp (arg[0] , arg ) ;  

                exit (1) ; 
            }
            else 
            {
                pid_t wc  = wait(NULL) ; 
            }
        }        
    }
    return 0 ; 
}

