#include <stdio.h> 
#include <stdio_ext.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 


#define max_lon 100 

char command [max_lon] ; 

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
                char * arr [10]; 
                arr [0] = command ;
                arr [1] = NULL ; 
                execvp (arr [0] , arr);

                printf("%s", "this should not print "); 
            }
            else 
            {
                pid_t wc  = wait(NULL) ; 
                //printf("%s" ,"test") ;
            }
            
        }        
    }
    return 0 ; 
}

