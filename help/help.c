#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define lenpath 10000


void showcmdhelp (char * path , char * cmd )
{
    FILE *stream ;
    char *line = NULL ; 
    size_t len = 0; 
    ssize_t nread ;

    
    char dir [lenpath] = "\0";
    strcat(dir,path);
    strcat(dir,"/help/doc/") ;
    strcat(dir , cmd);
    strcat(dir, ".txt");

    printf("%s\n" , dir);


    stream = fopen(dir , "r");
    if (stream == NULL)
    {
        perror ("fopen") ;
        exit(EXIT_FAILURE); 
    } 

    while ((nread = getline(&line , & len , stream))!=-1)
    {
        fwrite(line, nread ,1, stdout); 
    }

    free (line);
    fclose(stream);

}

int main (int argc , char ** argv )
{
    
    if (argc == 2 )
    {
        showcmdhelp(argv[1], "help");
    }
    else 
    {
        showcmdhelp(argv[1],argv[2]);
    }

    return 0  ; 
}