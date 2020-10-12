
/*
#include <stdio.h> 




int main (void )
{

    return 0 ; 
} 
*/

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    char *str1, *str2, *token, *subtoken;
    char *saveptr1, *saveptr2;
    int j;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s string delim subdelim\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    for (j = 1, str1 = argv[1]; ; j++, str1 = NULL) 
    {
        token = strtok_r(str1, argv[2], &saveptr1);
        if (token == NULL)
            break;
        printf("%d: %s\n", j, token);

        for (str2 = token; ; str2 = NULL)
        {
            subtoken = strtok_r(str2, argv[3], &saveptr2);
            if (subtoken == NULL)
                break;
            printf(" --> %s\n", subtoken);
        }
    }
    exit(EXIT_SUCCESS);
}
*/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 


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

void printcommand (struct command * cmd )
{
    printf ("comando-> %s\n" , cmd->cmd.b);
    printf ("input  -> %s\n" , cmd->input.b);
    printf ("output -> %s\n" , cmd->output.b); 
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


int main (void) 
{
    /*
    struct  abuf input  = ABUF_INIT ; 

    struct command cmd  = {ABUF_INIT,ABUF_INIT,ABUF_INIT};

    abAppend (&input , "ls -l < file1 > file2" , 23) ;      
    
    parser (input.b , &cmd) ;

    printcommand (&cmd);  

    parsercommand (&cmd) ; 
    */

    char **code = NULL ; 
    int numstr = 0 ; 

    char *cadena1  = "ls"; 
    char *cadena2 = "-l";

    code = realloc (code , sizeof(char* ) * (numstr + 1 ));

    * (code + numstr ++ ) = strdup (cadena1);   

    code = realloc (code , sizeof (char*) * (numstr + 1)); 

    * (code + numstr ++ ) = strdup (cadena2); 
    
    printf ("%s\n%s\n" , code [0], code [1]) ;  

 


    return 0 ; 

} 