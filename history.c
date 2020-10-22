#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

#define fileHistory  ".history.txt"
#define lenHistory  10

void appendHistory (char * cmd) 
{
    int  fd ; size_t len;   

    len = strlen(cmd);
    char * buf = malloc (sizeof(char) * (len+1)); 
    if (buf == NULL)
    {
        perror("Error al reservar momoria"); 
        exit (-1) ;
    }

    strncpy(buf,cmd,len);
    *(buf + (len )) = '\n';

    if ((fd = open(fileHistory , O_CREAT|O_WRONLY|O_APPEND, S_IRWXU ))==-1)
    {
        perror ("Error al abrir el fichero que contiene el history para escribir"); 
        exit(-1) ;
    }
    
    if (!write(fd , buf , len +1))
    {
        printf("No se puo escribir en el fichero "); 
    }

    close (fd) ;
    free(buf) ;
    
}


char * hist [10] ; 
void readHistory ()
{
    int fd , r , i , len ,last ;
    off_t offset;
    char *buf , *buf2;

    
    if ((fd = open(fileHistory,O_RDONLY,S_IRWXU))==-1)
    {
        perror ("Error al abrir el fichero que contiene el history para leer "); 
        exit(-1) ;
    } 
    offset = 0 ; 
    offset = lseek(fd ,offset,SEEK_END);
    
    last = offset;  
    len = 0 ; 

    buf = malloc(sizeof(char));


    
    while ((offset = lseek(fd , -1 , SEEK_CUR))>=0)
    {
        if ((r=read(fd,buf,1))!=1)
        {
            perror("error");
            exit (-1);
        }
        else if ( (strcmp(buf,"\n")==0)||(offset = lseek(fd,0,SEEK_CUR)) == 1)
        {
            offset = lseek(fd, 0,SEEK_CUR);
            //printf("%ld  - %ld\n", offset , last);
            if (offset == 1 && strcmp(buf,"\n") !=0 )
                offset = lseek(fd , -1 , SEEK_CUR);

            buf2 = malloc(sizeof(char) * (last - offset +1));
            r = read(fd,buf2,last - offset); 
            if (r!=last - offset)
            {
                perror("error al leer el comando i esimo del history ");
                exit(-1);
            }
            printf("%s\n" , buf2);
            free(buf2);
            offset = lseek(fd, -last + offset -1,SEEK_CUR);
            last = offset ;
        }
        else
        {
            offset = lseek(fd, -1,SEEK_CUR);
        }
       
    }

   

    free(buf); 

    close(fd); 
}


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

void readHistory1 ()
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
        printf( "%li :%s" , ++i ,line );
        
    }
    free (line); 
    fclose(stream); 
    exit(EXIT_SUCCESS);
} 


char * cmd ; 
int main(int argc , char **argv ) {
    
    if (argc == 1)
    {
        //printf("%s\n", argv[0]);
        readHistory1();
    }
    else if (argc ==2 )
    {
        //printf("%s\n" , argv[1]);
        appendCommand(argv[1]); 
    }
    //appendCommand("alfredo"); 
    return 0;
}