#include "Includes/operators.h"
#include "Includes/types.h"
#include <stdlib.h>

Cmd* InitCmd()
{
    Cmd* cmd = (Cmd*)malloc(sizeof(Cmd));

    cmd->CmdArgc = 0;
    cmd->CmdInPath = "";
    cmd->CmdLiteral = "";
    cmd->CmdOutPath = "";
    cmd->HasAppend = FALSE;
    cmd->HasIn = FALSE;
    cmd->HasOut = FALSE;

    return cmd;
}

Pipe_S* InitPipe_S()
{
    Pipe_S* _pipe = (Pipe_S*)malloc(sizeof(Pipe_S));

    _pipe->Head = NULL;
    _pipe->Tail = NULL;
    _pipe->Count = 0;
    _pipe->bg = FALSE;

    return _pipe;
}

/*
Chain* InitChain()
{
    Chain* chain = (Chain*)malloc(sizeof(Chain));

    chain->ChainSymbol = "";
    chain->ChainType = NONE;

    return chain;
}
*/

IFConditional* InitIFConditional()
{
    IFConditional* _ifCond = (IFConditional*)malloc(sizeof(IFConditional));

    _ifCond->_If = NULL;
    _ifCond->_Then = NULL;
    _ifCond->_Else = NULL;
    _ifCond->HasElse = FALSE;

    return _ifCond;
}

Expression* InitExpression()
{
    Expression* expr = (Expression*)malloc(sizeof(Expression));

    expr->ExprType = NONE;
    expr->_Chain = NONE;
    expr->PipeS = NULL;
    expr->_IfCond = NULL;
    expr->_Cmd = NULL;

    return expr;
}

ExpressionNode* InitExrpessionNode()
{
    ExpressionNode* exprNode = (ExpressionNode*)malloc(sizeof(ExpressionNode));

    exprNode->Expr = NULL;
    exprNode->Next = NULL;
    exprNode->Prev = NULL;

    return exprNode;
}

ExpressionList* InitExpressionList()
{
    ExpressionList* exprL = (ExpressionList*)malloc(sizeof(ExpressionList));

    exprL->Head = NULL;
    exprL->Tail = NULL;
    exprL->Count = 0;

    return exprL;
}

void Pipe_SInsert(Pipe_S* pipeS, ExpressionNode* exprNode)
{
    if (pipeS->Count == 0)
    {
        pipeS->Head = exprNode;
        pipeS->Tail = exprNode;
    }
    else
    {
        pipeS->Tail->Next = exprNode;
        exprNode->Prev = pipeS->Tail;
        pipeS->Tail = exprNode;
    }
    
    pipeS->Count++;
}

void Pipe_SInsert(Pipe_S* pipeS, Expression* expr)
{
    ExpressionNode* exprNode = InitExpressionNode();
    exprNode->Expr = expr;
    Pipe_SInsert(pipeS, exprNode);
}

ExpressionNode* Pipe_SGet(Pipe_S* pipeS)
{
    if (pipeS->Count == 0)
    {
        return NULL;
    }

    ExpressionNode* auxExpr = InitExpressionNode();
    auxExpr = pipeS->Head;
    pipeS->Head = auxExpr->Next;
    pipeS->Head->Prev = NULL;
    auxExpr->Next = NULL;
    pipeS->Count--;

    return auxExpr;
} 

void ExpressionListInsert(ExpressionList* exprL, ExpressionNode* node)
{
    if (exprL->Count == 0)
    {
        exprL->Head = node;
        exprL->Tail = node;
    }
    else
    {
        exprL->Tail->Next = node;
        node->Prev = exprL->Tail;
        exprL->Tail = node;
    }

    exprL->Count++;
    
}

void ExpressionListInsert(ExpressionList* exprL, Expression* expr)
{
    ExpressionNode* auxExprNode = InitExpressionNode();
    auxExprNode->Expr = expr;
    ExpressionListInsert(exprL, auxExprNode);
}

ExpressionNode* ExpressionListGet(ExpressionList* exprL)
{
    if (exprL->Count == 0)
    {
        return NULL;
    }

    ExpressionNode* auxNode = InitExpressionNode();
    auxNode = exprL->Head;
    exprL->Head = auxNode->Next;
    auxNode->Next = NULL;
    exprL->Head->Prev = NULL;
    exprL->Count--;

    return auxNode;
}

void InsertArg(Cmd* cmd, char* arg)
{
    if (cmd->CmdArgc >= 1024)
    {
        //to many arguments, are they necessary?... Are they?
        exit(1);
    }

    cmd->CmdArgv[cmd->CmdArgc] = arg;
    cmd->CmdArgc++;
    cmd->CmdArgv[cmd->CmdArgc] = NULL;
}

