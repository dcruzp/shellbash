#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

#define fileHistory  ".history.txt"
#define lenHistory  10

char * hist [10] ; 


void appendCommand (char * cmd) 
{
    FILE *stream ; 
    ssize_t nread ,i ;  
    size_t len = 0 ; 
    int count ; 

    for (i=0 ; i< lenHistory ; i++ )
        hist [i] = NULL ;
    

    stream = fopen(fileHistory , "r");
    if (stream == NULL) 
    {
        perror ("fopen") ;
        exit (EXIT_FAILURE); 
    } 

    for (i=0; i < lenHistory ; i++)
    {
        nread = getline(&hist[i] , &len , stream) ;
        if (nread == -1 ) break ;
    }
    count = i ;

    fclose (stream) ;
    
    if (count >= lenHistory )
    {
        for (i =0 ; i < lenHistory -1 ; i++)
        {
            hist[i] = hist[i+1];
        }
        hist [i] = cmd ; 
    }
    else 
    {
        hist[count] = cmd ;
        count ++ ;  
    }
    
    fopen(fileHistory , "w") ;
    for (i=0 ; i <count ; i++)
    {
        int j =0 ; 
        while (hist[i][j]!='\0' && hist[i][j] != '\n')
        {
            fputc(hist[i][j],(FILE*)stream);
            j++;
        }
        fputc('\n' ,(FILE*)stream);
    }
     
    fclose(stream); 
} 

void readHistory ()
{
    FILE *stream ; 
    char * line = NULL ;
    size_t len = 0 ; 
    ssize_t nread ,i ;


    stream = fopen(fileHistory , "r");
    if (stream == NULL) 
    {
        perror ("fopen") ;
        exit (EXIT_FAILURE); 
    } 
    
    i=0;
    while ((nread = getline(&line , & len , stream)) != -1)
    {
        printf( "%li: %s" , ++i ,line );
        
    }
    free (line); 
    fclose(stream); 
    exit(EXIT_SUCCESS);
} 

char * cmdagain (int numc)
{
    FILE *stream ; 
    ssize_t nread ,i ;  
    size_t len = 0 ; 
    int count ; 

    for (i=0 ; i< lenHistory ; i++ )
        hist [i] = NULL ;

    stream = fopen(fileHistory , "r");
    if (stream == NULL) 
    {
        perror ("fopen") ;
        exit (EXIT_FAILURE); 
    } 

    for (i=0; i < lenHistory ; i++)
    {
        nread = getline(&hist[i] , &len , stream) ;
        if (nread == -1 ) break ;
    }
    count = i ;

    if (numc > count || numc < 1)
    {
        return NULL ; 
    }
    else
    {
        return hist[numc-1];
    }

}

char * cmd ; 
int main(int argc , char **argv ) {
    
    if (argc == 1)
    {
        readHistory();
    }
    else if (argc ==2 )
    {
        appendCommand(argv[1]); 
    }
    return 0;
}