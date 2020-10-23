#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void showcmdhelp (char * cmd )
{
    FILE *stream ;
    char *line = NULL ; 
    size_t len = 0; 
    ssize_t nread ;

    char dir [1000] = "doc/"; 
    strcat(dir , cmd);
    strcat(dir, ".txt");


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
    
    if (argc == 1 )
    {
        showcmdhelp("help");
    }
    else 
    {
        showcmdhelp(argv[1]);
    }

    return 0  ; 
}