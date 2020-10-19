#include "execute.h"


void execute ( command *cmd ) 
{
    int pid , status , pid_aux ; 

    if ((pid = fork()) == -1 )
    {
        perror (NULL) ; 
        exit(-1);
    }
    else if (pid == 0 )
    {
        if (cmd->input != NULL)
        {
            pid_aux = dup(0);
            close(0);

            if (open(cmd->input , O_RDONLY,S_IRUSR)==-1)
            {
                dup(pid_aux);
                close(pid_aux);

                perror(NULL);
                exit(-1);
            }
        }
        

        if (cmd->output != NULL)
        {
            pid_aux = dup(1);
            close(1);
            if (cmd->outappend)
            {
                if (open (cmd->output ,O_CREAT|O_WRONLY|O_APPEND, S_IRWXU)==-1) 
                {
                    dup(pid_aux);
                    close(pid_aux);

                    perror(NULL);
                    exit(-1);
                }
            }
            else
            {
                if (open (cmd->output ,O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU)==-1)
                {
                    dup (pid_aux);
                    close(pid_aux);

                    perror(NULL);
                    exit(-1);
                }                
            }
        }
        if (execvp(cmd ->cmd , cmd ->argv)==-1)
        {
            printf("%s -> %s\n" , "hubo un error al ejecutar el comando " ,*cmd->argv );
            exit(-1);
        }
    }
    else
    {
        waitpid(pid,&status,0);
    }
}


void execute1  ( command * cmd1 , command *  cmd2)
{
    int tuberia[2]; 
    int pid1 , pid2; 
    int status1 , status2; 
    int fd_aux;

    if (pipe (tuberia) == -1 )
    {
        perror ("Error al crear la tuberia ") ;
        exit (-1) ; 
    }

    if ((pid1 = fork()) == -1 ) 
    {
        perror ("Error al crear el proceso hijo  ") ;
        exit (-1) ; 
    }
    else if (pid1 == 0)
    {
        //estoy en el hijo 

        close (tuberia [0]);  /*cierro la parte de la tuberia que no necesito*/ 

        /*pongo la salida de este proceso por 
        la parte de escritura de la tuberia*/
        {
        close (1);
        dup (tuberia [1]) ; 
        close (tuberia [1]);
        }

        /*si la entrada a este comando no es la entrada estandar
        porque esta redirigida entonces la cambio */
        if (cmd1->input != NULL)
        {
            fd_aux = dup (0);
            close(0);
            if (open(cmd1->input , O_RDONLY,S_IRUSR)==-1)
            {
                dup (fd_aux);
                close(fd_aux);

                perror(NULL);
                exit(-1);
            }
        }

        /*si la salida a este comanod no es la salida estandar 
        porque seta redirigida entonces la cambio */
        if (cmd1->output != NULL)
        {
            fd_aux = dup(1);
            close(1);  /*cierro la salida estandar */ 

            if (cmd1->outappend)  /*si la salida esta redirigida  con >> entonces 
                                    el fichero lo abro con el flag O_APPEND para 
                                    que me escriba la salida al final del fichero 
                                    que se crea o se abre para escrbir la salida */
            {
                if (open (cmd1->output ,O_CREAT|O_WRONLY|O_APPEND, S_IRWXU)==-1)   /*si hubo un problema abriendo el fichero 
                                                                                    entonces vuelvo a poner la salida estandar */
                {
                    dup (fd_aux);
                    close(fd_aux);

                    perror(NULL);
                    exit(-1);
                } 
            }
            else
            {
                if (open (cmd1->output ,O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU) == -1)  /*si hubo un problema abriendo el fichero 
                                                                                    entonces vuelvo a poner la salida estandar */
                {
                    dup (fd_aux);
                    close(fd_aux);

                    perror(NULL);
                    exit(-1);
                }
            }
        }    

        /*Ya con los file descriptors correctos de entrada y de 
        salida mando a ejecutar  el comando  dando exit status -1 en
        caso de que el comando no se pueda ejecutar  */
        if (execvp(cmd1->argv[0], cmd1->argv)==-1)
        {
            printf("%s -> %s\n" , "hubo un error al ejecutar el comando " ,*cmd1->argv );
            exit(-1); 
        }

    }
    else 
    {
        //estoy en el padre 

        /*cierro el file descriptor de escritura de la tuberia */
        close (tuberia[1]);   


        if ((pid2 = fork()) == -1 ) 
        {
            perror ("Error al crear el proceso hijo  ") ;
            exit (-1) ; 
        }
        else if (pid2 == 0)
        {

            /*pongo la entrada estandar de este programa
            por el file descriptor de lectura de la tuberia */
            {
            close (0);
            dup(tuberia[0]);             
            close(tuberia[0]);  /*cierro el file descriptor de lectura de la tuberia*/
            }
            
            /*si la estandar de este comando esta redirigida 
            entonces cambio la entrada al file descriptor  */
            if (cmd2->input != NULL)
            {
                fd_aux = dup(0);
                close (0);

                if (open(cmd2->input , O_RDONLY,S_IRUSR) == -1 )
                {
                    dup(fd_aux);
                    close(fd_aux);

                    perror(NULL);
                    exit(-1);
                }
            }


            /*si la salida del segundo comando esta redirigida 
            (no es la salida estandar  ) esntonces la cambio  */
            if (cmd2->output != NULL)
            {
                fd_aux = dup(1);
                close(1);  /*cierro la salida estandar */ 

                if (cmd2->outappend)  /*si la salida esta redirigida  con >> entonces 
                                        el fichero lo abro con el flag O_APPEND para 
                                        que me escriba la salida al final del fichero 
                                        que se crea o se abre para escrbir la salida */
                {
                    if (open (cmd2->output ,O_CREAT|O_WRONLY|O_APPEND, S_IRWXU)==-1)   /*si hubo un problema abriendo el fichero 
                                                                                        entonces vuelvo a poner la salida estandar */
                    {
                        dup (fd_aux);
                        close(fd_aux);

                        perror(NULL);
                        exit(-1);
                    } 
                }
                else
                {
                    if (open (cmd2->output ,O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU) == -1)  /*si hubo un problema abriendo el fichero 
                                                                                        entonces vuelvo a poner la salida estandar */
                    {
                        dup (fd_aux);
                        close(fd_aux);

                        perror(NULL);
                        exit(-1);
                    }
                }
            }

            /*una vez que se establece que file descriptors 
            va a utilizar este comando entonces lo mando a ejecutar 
            y en caso de que no se pueda ejecutar mato el proceso  con 
            exit status -1 */
            if (execvp(cmd2->argv[0] , cmd2->argv)==-1)
            {
                printf("%s -> %s\n" , "hubo un error al ejecutar el comando " ,*cmd2->argv );
                exit (-1); 
            }           
        }
        else 
        {
            waitpid (pid1 , &status1,0);
            waitpid (pid2, &status2, 0); 
        }        
    }
}


//int main () {return 0 ; }  