#include <stdio.h> 
#include <stdio_ext.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdlib.h>
#include <fcntl.h>
#include "parser.h"

#define max_lon 1000

char input [max_lon] ; 

/*Variables de ambiente */
char SHELL [max_lon];
char PATH [max_lon]; 
char HOME [max_lon]; 
char PWD [max_lon];  

/*bandera de control del ciclo principal */ 
int flag = 1; 
command **cmd;
int numb ;