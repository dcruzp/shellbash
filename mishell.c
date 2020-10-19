#include "mishell.h"


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
        else if (numb<2)
        {
            execute(cmd[0]);
        }
        else
        {
            execute1 (cmd[0] , cmd[1]) ; 
        }        
    }
    return 0 ; 
}

