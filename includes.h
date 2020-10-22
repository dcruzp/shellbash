
/**
 * @brief Ejemplo de uso:
 * // comando a ejecutar
 * char* comando;
 * 
 * //inicializar una lista de tokens
 * TokenList* tl = InitTokenList();
 * 
 * //tokenizar comando y devolver los tokens en la lista de tokens tl
 * Tokenize(comando, tl);
 * 
 * //Inicializar una lista de ExpressionNode
 * ExpressionList* exprL = InitExpressionList();
 * 
 * //Convertir la lista de tokens tl en la lista de ExpressionNode ejecutables exprL
 * Parse(tl, exprL);
 * 
 * //Ejecutar la lista de ExpressionNode exprL
 * Execute(exprL); 
 */

/**
 * Includes 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>


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
#define END_CHAIN 2

//  definir tipos de operación
#define OP_IF 0
#define OP_PIPE 1
#define OP_CHAIN 2
#define OP_CMD 3

// Para saber cuando se espera algo dentro del BuildPipe
#define IFEXPECTED 0
#define THENEXPECTED 1
#define ENDEXPECTED 2

#define OPERATION 0
#define COMMAND 1

//  definir tipos personalizados para valores de true o false.
#define FALSE 0
#define TRUE 1
#define OPSLEN 9    // longitud del array constante de operadores que se encuentra al inicio de includes.c
#define BUILTINLEN 5   // longitud del array constante de built-ins que se encuentra al inicio de includes.c


typedef int t_type;             // Tipo de token (WORD, OPERATOR, QUOTED, SINGLEQUOTED).
typedef char* t_literal;        // Texto del token.
typedef int t_chain_type;       // Tipo de encadenamiento (AND_CHAIN, OR_CHAIN, END_CHAIN).
//typedef char* t_chain_symbol;   // Símbolo del encadenamiento (&&, ||).

typedef char* t_cmd_literal;    // Orden del comando.
typedef int t_cmd_argc;         // Cantidad de argumentos de un comando.
typedef int t_has_out;          // TRUE si el comando tiene redirección de salida.
typedef int t_has_append;       // TRUE si el comando tiene redirección de salida con append.
typedef int t_has_in;           // TRUE si el comando tiene redirección de entrada.
typedef char* t_cmd_argv;       // Argumentos del comando.
typedef char* t_cmd_inpath;     // Dirección para redirección de entrada.
typedef char* t_cmd_outpath;    // Dirección para redirección de salida.

typedef int t_is_bg;    // TRUE si la lista de comandos en un pipe se va a ejecutar en background.

typedef int t_has_else; //  TRUE si la sentencia if actual tiene un else.

typedef int t_expr_type;    // Guarda el tipo de Expression (OP_IF, OP_PIPE, OP_CHAIN, OP_CMD)

typedef int t_expected; // Para saber que esperar de condicionales dentro de BuildPipe (IFEXPECTED, THENEXPECTED, ENDEXPECTED)

/**
 * @brief En token guarda el tipo de token (WORD, OPERATOR, QUOTED, SINGLEQUOTED)
 * y su texto en Literal.
 */
typedef struct Token
{
    t_type Type;
    t_literal Literal;
} Token;

/**
 * @brief Los nodos de la lista de Tokens.
 * 
 */
typedef struct TokenNode{
    Token* Tok;
    struct TokenNode * Prev;
    struct TokenNode * Next;
} TokenNode;

/**
 * @brief Una lista enlazada de tokens.
 * 
 */
typedef struct TokenList
{
    TokenNode* Head;
    TokenNode* Tail;
    int Count;
}TokenList;

int IsOperator(char* buf);  //  Dice si buf es un operador reconocido.
TokenList* InitTokenList(); //  Inicializa una lista de Tokens.
void InsertTokenFront(TokenList* tl, Token* newTok);    //  Inserta un token al frente de la lista de tokens.
void InsertTokenBack(TokenList* tl, Token* newTok);     //  Inserta un token al final de la lista de tokens.
Token* GetHeadToken(TokenList* tl); //  Devuelve el token al frente de la lista de tokens y lo elimina de esta.
Token* GetTailToken(TokenList* tl); //  Devuelve el token al final de la lista de tokens y lo elimina de esta.
//void* DelHeadToken(TokenList* tl);  //  
//void* DelTailToken(TokenList* tl);

/**
 * @brief Toma el buffer de la entrada y lo convierte en una lista de Token.
 * 
 * @param buf linea a tokenizar.
 * @param tl Lista de tokens inicializada donde se devolverá la lina buf tokenizada.
 */
void Tokenize(const char * buf, TokenList* tl);

Token* InitToken(); //  Inicializa un struct tipo Token.

/**
 * @brief Concatena un caracter al final de un string y devuelve el nuevo string concatenado.
 * 
 * @param str1 string a concatenar.
 * @param _char caracter a concatenar.
 * @return char* str1 concatenado con _char
 */
char * ConcatChar(char * str1, char _char);

/**
 * @brief Esta estructura guarda todo lo referente a un comando.
*/
typedef struct Cmd{
    t_cmd_literal CmdLiteral;       //Orden del comando.
    t_cmd_argc CmdArgc;             //Cantidad de argumentos del comando.
    t_has_out HasOut;               //TRUE si tiene redirección de salida.
    t_has_append HasAppend;         //TRUE si tiene redirección de salida con append.
    t_has_in HasIn;                 //TRUE si tiene redirección de entrada.
    t_cmd_argv CmdArgv[1024];       //Lista de argumentos.
    t_cmd_inpath CmdInPath;         //Dirección para redirección de entrada.
    t_cmd_outpath CmdOutPath;       //Dirección para redirección de salida.
} Cmd;

/**
 * @brief Nodos para guardar comandos en la pila de Pipe_S
 * 

typedef struct CmdNode{
    Cmd* cmd;                       //  Comando guardado en el nodo.
    struct CmdNode* Next;           //  Referencia al próximo nodo en la pila de Pipe_S
} CmdNode;
 */
/**
 * @brief Guarda una cola de expresiones enlazadas por pipes.
 */
typedef struct Pipe_S{
    struct ExpressionNode* Head;    // Primara Exppression en la cola de Expression.
    struct ExpressionNode* Tail;    // Última Expression enla cola de Expression.
    int Count;                      // Cántidad de Expressions en la cola.
    t_is_bg bg;                     // TRUE si la cola de Expressions debe ejecutarse en background.
} Pipe_S;

/*
typedef struct Chain{
    t_chain_type ChainType;
    t_chain_symbol ChainSymbol;
} Chain;
*/
/**
 * @brief Guarda las partes de la sentencia if
*/
typedef struct IFConditional{
    struct Expression* _If;     // Guarda la Expression que va entre el if y el then.
    struct Expression* _Then;   // Guarda la Expression a ejecutar si el if es TRUE.
    struct Expression* _Else;   // Guarda la Expression a ejecutar si el if es False.
    t_has_else HasElse;         // TRUE si contiene Expression en _Else.
} IFConditional;

/**
 * @brief Esta estructura es para tratar a todos los
 * operadores como una Expression y faciltar su
 * manipulación.
 */
typedef struct Expression{
    t_expr_type ExprType;       // Guarda el tipo de Expression (OP_IF, OP_PIPE, OP_CHAIN, OP_CMD).
    t_chain_type _Chain;        // Guarda el tipo de chain (AND_CHAIN, OR_CHAIN, END_CHAIN).
    Pipe_S* PipeS;              // Guarda una cola Pipe_S si es una Expression de ese tipo.
    IFConditional* _IfCond;     // Guarda un IFCondicional si es una Expression de ese tipo.
    Cmd* _Cmd;                  // Guarda un Cmd si es una Expression de ese tipo.
} Expression;

/**
 * @brief Nodos para formar la lista enlazada de Expressions.
 * 
 */
typedef struct ExpressionNode{
    Expression* Expr;               //  Expression guardada en este nodo.
    struct ExpressionNode* Prev;    //  Referencia al nodo anterior.
    struct ExpressionNode* Next;    //  Referencia al nodo siguiente.
} ExpressionNode;

/**
 * @brief Lista enlazada de Expressions
 * 
 */
typedef struct ExpressionList{
    struct ExpressionNode* Head;    // Nodo al frente de la lista enlazada.
    struct ExpressionNode* Tail;    // Nodo al final de la lista enlazada.
    int Count;                      //Cantidad de nodos en la lista enlazada
} ExpressionList;


/**
 * @brief Inicializa un Comando.
 * 
 * @return Cmd* Comando inicializado.
 */
Cmd* InitCmd();
//CmdNode* InitCmdNode();

/**
 * @brief Inicializa una cola de pipes Pipe_S.
 * 
 * @return Pipe_S* La pila de pipes inicializada.
 */
Pipe_S* InitPipe_S();
//Chain* InitChain();
/**
 * @brief Inicializa un struct de tipo IFConditional.
 * 
 * @return IFConditional* 
 */
IFConditional* InitIFConditional();

/**
 * @brief Inicializa un struct de tipo Expression.
 * 
 * @return Expression* 
 */
Expression* InitExpression();

/**
 * @brief Inicializa un struct de tipo ExpressionNode.
 * 
 * @return ExpressionNode* 
 */
ExpressionNode* InitExpressionNode();

/**
 * @brief Inicializa una lista de Expressions.
 * 
 * @return ExpressionList* 
 */
ExpressionList* InitExpressionList();

/**
 * @brief Inserta una Expression en una pila de Pipe_S.
 * 
 * @param pipeS Pila para insertarle la Exrpession.
 * @param exprNode Nodo a insertar en la pila.
 */
void Pipe_SInsert(Pipe_S* pipeS, ExpressionNode* exprNode);

/**
 * @brief Inserta una Expression en una pila de Pipe_S.
 * 
 * @param pipeS Pila para insertarle la Exrpession.
 * @param expr Expression a insertar en la pila.
 */
void Pipe_SInsertE(Pipe_S* pipeS, Expression* expr);

/**
 * @brief Devuelve la próxima Expression en la cola de pipes.
 * 
 * @param pipeS La cola a utilizar.
 * @return ExpressionNode* 
 */
ExpressionNode* Pipe_SGet(Pipe_S* pipeS);

/**
 * @brief Inserta una Expression en la lista de Expressions.
 * 
 * @param exprL Lista de Expressions.
 * @param expr Expression a insertar en la lista.
 */
void ExpressionListInsertE(ExpressionList* exprL, Expression* expr);

/**
 * @brief Inserta una NodeExpression en la lista de Expressions.
 * 
 * @param exprL Lista de Expressions.
 * @param expr NodeExpression a insertar en la lista.
 */
void ExpressionListInsert(ExpressionList* exprL, ExpressionNode* node);

/**
 * @brief Devuelve NodeExpression al frente de la lista.
 * 
 * @param exprL Lista a operar.
 * @return ExpressionNode* 
 */
ExpressionNode* ExpressionListGet(ExpressionList* exprL);

/**
 * @brief Inserta un argumento en un Comando.
 * 
 * @param cmd Comnado a operar.
 * @param arg Argumento a insertar.
 */
void InsertArg(Cmd* cmd, char* arg);

/**
 * @brief Convierte un lista de tokens en una lista de Expressions.
 * 
 * @param tl Lista de Tokens
 * @param superExpr Lista de ExpressionNode a devolver.
 */
void Parse(TokenList* tl, ExpressionList* superExpr);

/**
 * @brief Convierte tokens en Expressions.
 * 
 * @param tl Lista de Expressions.
 * @param expr Expression a devolver
 */
void BuildExpression(TokenList* tl, Expression* expr);

/**
 * @brief Construye una Expression de tipo IfCondition de una lista de tokens.
 * 
 * @param tl Lista de Tokens
 * @param ifExpr La Expression a devolver
 */
void BuildIf(TokenList* tl, Expression* ifExpr);

/**
 * @brief Construye una Expression de tipo Pipe_S de una lista de tokens.
 * 
 * @param tl Lista de Tokens
 * @param pipeExpr La Expression a devolver
 */
void BuildPipe(TokenList* tl, Expression* pipeExpr, t_expected expect);

/**
 * @brief Construye una Expression de tipo Cmd de una lista de tokens.
 * 
 * @param tl Lista de Tokens
 * @param ifcmdExpr La Expression a devolver
 */
void BuildCmd(TokenList* tl, Expression* cmdExpr);

/**
 * @brief Devuelve TRUE si el string es una palabra reservada.
 * 
 * @param word String a analizar.
 * @return int 
 */
int IsReserved(char* word);

/**
 * @brief Ejecuta todas las expresiones en una linea de comando.
 * 
 * @param exprL La lista de Expressions a ejecutar
 */
void Execute(ExpressionList* exprL);

/**
 * @brief Sabe como ejecuter una expression de tipo condicional.
 * 
 * @param IfExpr Expression de tipo IfCondition a ejecutar.
 * @param inFd File descriptor de entrada.
 * @param outFd File descriptor de salida.
 */
void ExecuteIf(Expression* IfExpr, int inFd, int outFd);

/**
 * @brief Sabe como ejecutar una Expression de tipo Pipe_S.
 * 
 * @param pipeExpr Expression de tipo Pipe_S a ejecutar.
 */
void ExecutePipe(Expression* pipeExpr); 

/**
 * @brief Sabe ejecutar un Expression de tipo Cmd.
 * 
 * @param cmdExpr Expression de tipo Cmd a ejecutar.
 * @param inFd File descriptor de entrada.
 * @param outFd  File descriptor de salida.
 */
void ExecuteCmd(Expression* cmdExpr, int inFd, int outFd);

/**
 * @brief Ejecuta comandos Built-in.
 * 
 * @param cmdExpr Expression de tipo Cmd a ejecutar.
 */
void ExecuteBuiltIn(Expression* cmdExpr);

int GetInfd(Expression* cmdExpr);
int GetOutfd(Expression* cmdExpr);


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Built-in commands

/**
 * @brief Devuelve TRUE si buf es un comando Built-in.
 * 
 * @param buf String a analizar.
 * @return int TRUE or FALSE.
 */
int IsBuiltIn(char* buf);

/**
 * @brief exit(EXIT_SUCCESS);
 * 
 */
void True();
/**
 * @brief exit(EXIT_FAILURE);
 * 
 */
void False();