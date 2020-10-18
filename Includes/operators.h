#ifndef OPERATORS
#define OPERATORS

#include "types.h"
/***
 * Esta estructura guarda todo lo referente a un comando
*/
typedef struct Cmd{
    t_cmd_literal CmdLiteral;       //orden del comando
    t_cmd_argc CmdArgc;             //cantidad de argumentos del comando
    t_has_out HasOut;               //TRUE si tiene redireccion de salida 
    t_has_append HasAppend;         //TRUE si tiene redireccion de salida con append
    t_has_in HasIn;                 //TRUE si tiene redireccion de entrada
    t_cmd_argv CmdArgv[1024];       //Lista de argumentos
    t_cmd_inpath CmdInPath;         //Direccion para redireccion de entrada
    t_cmd_outpath CmdOutPath;       //Direccion para redireccion de salida
} Cmd;


typedef struct CmdNode{
    Cmd* cmd;
    struct CmdNode* Next;
} CmdNode;

/***
 * Guarda una lista de expresiones enlazadas por pipes 
 */
typedef struct Pipe_S{
    ExpressionNode* Head;
    ExpressionNode* Tail;
    int Count;
    t_is_bg bg;
} Pipe_S;

/*
typedef struct Chain{
    t_chain_type ChainType;
    t_chain_symbol ChainSymbol;
} Chain;
*/
/***
 * Guarda las partes de la sentencia if
*/
typedef struct IfConditional{
    Expression* _If;
    Expression* _Then;
    Expression* _Else;
    t_has_else HasElse;
} IfConditional;

/***
 * Esta estructura es para tratar a todos los
 * operadores como una expresion y faciltar su
 * manipulacion.
 */
typedef struct Expression{
    t_expr_type ExprType;
    t_chain_type _Chain;
    Pipe_S* PipeS;
    IfConditional* _IfCond;
    Cmd* _Cmd;
} Expression;

typedef struct ExpressionNode{
    Expression* Expr;
    struct ExpressionNode* Prev;
    struct ExpressionNode* Next;
} ExpressionNode;

typedef struct ExpressionList{
    ExpressionNode* Head;
    ExpressionNode* Tail;
    int Count;
} ExpressionList;



Cmd* InitCmd();
CmdNode* InitCmdNode();
Pipe_S* InitPipe_S();
//Chain* InitChain();
IfConditional* InitIFConditional();
Expression* InitExpression();
ExpressionNode* InitExpressionNode();
ExpressionList* InitExpressionList();

void Pipe_SInsert(Pipe_S* pipeS, ExpressionNode* exprNode);//Insert a commnad into pipe queue
ExpressionNode* Pipe_SGet(Pipe_S* pipeS);              //Get next command in pipe queue

void ExpressionListInsert(ExpressionList* exprL, ExpressionNode* node);
void ExpressionListInsert(ExpressionList* exprL, Expression* expr);
ExpressionNode* ExpressionListGet(ExpressionList* exprL);

void InsertArg(Cmd* cmd, char* arg);


#endif
