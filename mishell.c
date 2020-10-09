#include <stdio.h> 
#include <stdio_ext.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdlib.h>


#define max_lon 100 

char command [max_lon] ; 

/*Variables de ambiente */
char SHELL [max_lon];
char PATH [max_lon]; 
char HOME [max_lon]; 
char PWD [max_lon];  


char * ptr [10]  , *token ; 
char delimit [] = " "  ;  
int flag = 1  , i ; 

char *redir [3] ; 

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


        token = strtok (command , delimit) ; 

        i=0; 
        while (token != NULL) 
        {
            ptr [i++ ] = token ; 
            token = strtok (NULL , delimit) ;  
        }  
        ptr[i] = NULL; 

        if (strcmp (command , "exit") == 0 )
        {            
            flag =  0;  
        }
        else if (strcmp(ptr[0], "cd")==0)
        {
            if (ptr[1]) 
            {
                if (chdir (ptr[1])!=0)
                {
                    printf ("Error : %s  no existe o no se puede cambiar a este directorio ", ptr[1] );    
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
                execvp (ptr[0] ,ptr ) ;  

                printf("%s", "this should not print "); 
            }
            else 
            {
                pid_t wc  = wait(NULL) ; 
            }
        }        
    }
    return 0 ; 
}


void parser (char command [] ) 
{

} 

char *str1;   

void splitstr (char cadena []  , char delimit [] ) 
{
    
    for ( i=0 , str1 = cadena  ; ; i++ , str1 = NULL ) 
    {
        token = strtok (str1 , delimit); 
        ptr [i]  = token ; 
        if (token == NULL ) break ;  
    }
}
