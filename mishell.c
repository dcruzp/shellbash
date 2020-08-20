#include <stdio.h> 
#include <stdio_ext.h>
#include <string.h> 


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
            //printf ("%s" , "exit");
            flag = 0 ;  
        }
    }

    return 0 ; 
}

