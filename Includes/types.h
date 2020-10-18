#ifndef TYPES
#define TYPES

/**
 * Define token types
*/
#define WORD 0
#define OPERATOR 1
#define QUOTED 2
#define SINGLEQUOTED 3
#define NONE -1

//  definir tipos de encadenamientos
#define AND_CHAIN 0
#define OR_CHAIN 1
#define CHAIN 2

//  definir tipos de operacion
#define OP_IF 0
#define OP_PIPE 1
#define OP_CHAIN 2
#define OP_CMD 3

#define OPERATION 0
#define COMMAND 1

//  Custom types defines
#define FALSE 0
#define TRUE 1
#define OPSLEN 9

typedef int t_type;             // Tipo de token (WORD, OPERATOR, QUOTED, SINGLEQUOTED)
typedef char* t_literal;        // texto del token
typedef int t_chain_type;       // tipo de encadenamiento (AND_CHAIN, OR_CHAIN)
//typedef char* t_chain_symbol;   // simbolo del encadenamiento (&&, ||)

typedef char* t_cmd_literal;    // orden del comando
typedef int t_cmd_argc;         // cantidad de argumentos de un comando
typedef int t_has_out;          // TRUE si el comando tiene redireccion de salida
typedef int t_has_append;       // TRUE si el comando tiene redireccion de salida con append
typedef int t_has_in;           // TRUE si el comando tiene redireccion de entrada
typedef char* t_cmd_argv;      // Argumentos del comando
typedef char* t_cmd_inpath;     // Direccion para redireccion de entrada
typedef char* t_cmd_outpath;    //  Direccion para redireccion de salida

typedef int t_is_bg;

typedef int t_has_else;

typedef int t_expr_type;        // Guarda el tipo de expression (OP_IF, OP_PIPE, OP_CHAIN, OP_CMD)


#endif