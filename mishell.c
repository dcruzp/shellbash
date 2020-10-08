#include <stdio.h> 
#include <stdio_ext.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 


#define max_lon 100 

char command [max_lon] ; 

char * ptr [10]  , *token ; 
char delimit [] = " "  ;  
int i ; 

int flag  = 1 ; 

int main () {
    

    while (flag)
    {
            
        printf("%s$ " , "shell" ); 
        __fpurge (stdin); 

        memset (command , '\0' , max_lon) ; 
        scanf("%[^\n]s" , command) ;

        if (strcmp (command , "exit") == 0 )
        {            
            flag =  0;  
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
                
                token = strtok (command , delimit) ; 

                i=0; 
                while (token != NULL) 
                {
                    ptr [i++ ] = token ; 
                    token = strtok (NULL , delimit) ;  
                }  
                ptr [i] = NULL  ; 

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

