#include "includes.h" 
#include <stdio_ext.h>


int flag = 1 ; 
#define max_lon 1000
char input [max_lon] ; 


void guardarcmd (char * cmd )
{
    int pid ; 

    if ((pid = fork())== -1 )
    {
        perror ("Error al guardar el comando en el history ");
        exit (-1);
    }
    else if (pid == 0) 
    {
        execlp("./history.o" , "./history.o" , cmd , NULL) ; 
    }
    else 
    {
        wait (NULL); 
    }
    
}

/*
void createexecutehistory()
{
    int pid ;
    if( pid = fork()== -1)
    {
        perror("Error fork ");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
         
        char buf [PWD_LEN] = "\0";
        strcat(buf, pwd); 
        strcat(buf, "./history.sh");
        //printf("%s\n" , buf) ;
        execlp(buf , buf , NULL);
    }
    else
    {
        wait (NULL);
    }
    
}
*/
/*
void createexecutehelp()
{
    int pid ;
    if( pid = fork()== -1)
    {
        perror("Error fork ");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
         
        char buf [PWD_LEN] = "\0";
        strcat(buf, pwd); 
        strcat(buf, "/help/help.sh");
        execlp(buf , buf , NULL) ;
    }
    else
    {
        wait (NULL);
    }
    
}*/

int main () {

    signal(SIGINT, SIG_IGN);
    getcwd (pwd ,1000); 
    //createexecutehistory();
    //createexecutehelp();

    while (flag)
    {

        printf("%s$ " , "Shell"); 
        __fpurge (stdin); 

        memset (input , '\0' , max_lon) ; 
        scanf("%[^\n]s" , input) ;

        guardarcmd(input);
        
        
        
        //inicializar una lista de tokens
        TokenList* tl = InitTokenList();

        //tokenizar comando y devolver los tokens en la lista de tokens tl
        Tokenize(input, tl);
    
        //Inicializar una lista de ExpressionNode
        ExpressionList* exprL = InitExpressionList();
    
        //Convertir la lista de tokens tl en la lista de ExpressionNode ejecutables exprL
        int parseSuccess = Parse(tl, exprL);

        if(parseSuccess)
        {    
            //Ejecutar la lista de ExpressionNode exprL
            Execute(exprL); 
        }
    }
    
    
}