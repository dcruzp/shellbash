#include "mishell.h"


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
            execute (cmd[0] , cmd[1]) ; 
        }        
    }
    return 0 ; 
}

