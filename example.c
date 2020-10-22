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

int main () {


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
        Parse(tl, exprL);

       

        if (strcmp (input, "exit")== 0 )
        {
            flag = 0  ; 
        }
        else
        {    
            //Ejecutar la lista de ExpressionNode exprL
            Execute(exprL); 
        }
    }
    
    
}