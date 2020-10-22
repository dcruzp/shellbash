#include "includes.h" 
#include <stdio_ext.h>


int flag = 1 ; 
#define max_lon 1000
char input [max_lon] ; 

int main () {


    while (flag)
    {

        printf("%s$ " , "Shell"); 
        __fpurge (stdin); 

        memset (input , '\0' , max_lon) ; 
        scanf("%[^\n]s" , input) ;

        //printf("%i\n", getpid());

        if (strcmp (input, "exit")== 0 )
        {
            flag = 0  ; 
        }
        else
        {
            //inicializar una lista de tokens
            TokenList* tl = InitTokenList();

            //tokenizar comando y devolver los tokens en la lista de tokens tl
            Tokenize(input, tl);
    
            //Inicializar una lista de ExpressionNode
            ExpressionList* exprL = InitExpressionList();
    
            //Convertir la lista de tokens tl en la lista de ExpressionNode ejecutables exprL
            Parse(tl, exprL);
    
            //Ejecutar la lista de ExpressionNode exprL
            Execute(exprL); 
        }
    }
    
    
}