#include <stdio_ext.h>
#include "execute.h"

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