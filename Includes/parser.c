#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "token.h"
#include "operators.h"

void Parse(TokenList* tl, ExpressionList* superExpr)
{
    superExpr = InitExpressionList();
    int expectWord = TRUE;
    int wasBG = FALSE;
    while (tl->Count > 0)
    {
        Expression* currExpr = InitExpression();
        Token* currToken = InitToken();
        currToken = tl->Head->Tok;

        if(expectWord && currToken->Type != WORD)
        {
            // Unexpected token, cmd or if expected
            exit(EXIT_FAILURE);
        }
        if(!expectWord && currToken->Type == WORD)
        {
            // Unexpected token, cmd or if expected
            exit(EXIT_FAILURE);
        }

        if(expectWord && currToken->Type == WORD)
        {
            currExpr->ExprType = OP_PIPE;
            BuildPipe(tl, currExpr);
            if(currExpr->PipeS->bg)
            {
                wasBG = TRUE;
            }
            ExpressionListInsert(superExpr, currExpr);
            expectWord = FALSE;
            continue;
        }

        if(currToken->Type == OPERATOR)
        {
            currExpr->ExprType = OP_CHAIN;
            if (!strcmp(currToken->Literal, ";"))
            {                
                currExpr->_Chain = END_CHAIN;
                wasBG = FALSE;
            }
            else if(wasBG)
            {
                // Invalid expression after &
                exit(EXIT_FAILURE);
            }
            else if (!strcmp(currToken->Literal, "&&"))
            {
                currExpr->_Chain = AND_CHAIN;
            }
            else if (!strcmp(currToken->Literal, "||"))
            {
                currExpr->_Chain = OR_CHAIN;
            }
            else
            {
                // Invalid token in expression, expected &&, || or ;
                exit(EXIT_FAILURE);
            }

            ExpressionListInsert(superExpr, currExpr);
            currToken = GetHeadToken(tl);
            free(currToken);
            expectWord = TRUE;
            continue;
        }

        //  Invalid input token, expected &&, || or ;
        exit(EXIT_FAILURE);
    }    

}

void BuildIf(TokenList* tl, Expression* ifExpr)
{
    if (tl == NULL || tl->Count == 0)
    {
        // Bad call to BuildIf
        exit(EXIT_FAILURE);
    }

    Token* currToken = GetHeadToken(tl);
    IFConditional* _if = InitIFConditional();
    
    if (currToken == NULL || strcmp(currToken->Literal, "if"))
    {
        // Bad call to buildIf
        exit(EXIT_FAILURE);
    }

    free(currToken);
    Expression* _Pipe = InitExpression();
    _Pipe->ExprType = OP_PIPE;
    
    // lo que va entre el if y el then
    BuildPipe(tl, _Pipe);
    if (_Pipe->PipeS->bg)
    {
        // Invalid expression after &
        exit(EXIT_FAILURE);
    }
    _if->_If = _Pipe;

    currToken = tl->Head->Tok;

    if(currToken == NULL || strcmp(currToken->Literal, "then"))
    {
        // Invalid syntax, 'then' expected
        exit(EXIT_FAILURE);
    }
    
    free(currToken);
    _Pipe = InitExpression();
    _Pipe->ExprType = OP_PIPE;
    
    //Lo que va despues del then
    BuildPipe(tl, _Pipe);
    if (_Pipe->PipeS->bg)
    {
        // Invalid expression after &
        exit(EXIT_FAILURE);
    }
    _if->_Then = _Pipe;

    currToken = tl->Head->Tok;

    if(currToken != NULL && !strcmp(currToken->Literal, "else"))
    {
        free(currToken);
        _if->HasElse = TRUE;
        _Pipe = InitExpression();
        _Pipe->ExprType = OP_PIPE;
        BuildPipe(tl, _Pipe);
        if (_Pipe->PipeS->bg)
        {
            // Invalid expression after &
            exit(EXIT_FAILURE);
        }
        _if->_Else = _Pipe;
        currToken = tl->Head->Tok;
    }

    if(currToken == NULL || strcmp(currToken->Literal, "end"))
    {
        // Invalid syntax, 'end' expected
        exit(EXIT_FAILURE);
    }

    ifExpr->_IfCond = _if;

}

void BuildPipe(TokenList* tl, Expression* pipeExpr)
{
    if(tl == NULL || tl->Count == 0)
    {
        // Bad call to BuildPipe
        exit(EXIT_FAILURE);
    }

    Token* currToken = tl->Head->Tok;
    if (currToken->Type != WORD)
    {
        // Invalid syntax, if or command expected
        exit(EXIT_FAILURE);
    }

    Pipe_S* _pipe = InitPipe_S();    
    
    while (tl->Count > 0)
    {
        Expression* currExpr = InitExpression();
        if(!strcmp(currToken->Literal, "if"))
        {
            currExpr->ExprType = OP_IF;
            BuildIf(tl, currExpr);
        }
        else
        {
            currExpr->ExprType = OP_CMD;
            BuildCmd(tl, currExpr);
        }

        Pipe_SInsert(_pipe, currExpr);
        currToken = tl->Head->Tok;

        if(currToken == NULL || strcmp(currToken->Literal, "|"))
        {
            break;
        }
        currToken = GetHeadToken(tl);
        free(currToken);
        currToken = tl->Head->Tok;
    }

    if(currToken != NULL && !strcmp(currToken->Literal, "&"))
    {
        currToken = GetHeadToken(tl);
        free(currToken);
        _pipe->bg = TRUE;
    }
    
    pipeExpr->PipeS = _pipe;
    
}

void BuildCmd(TokenList* tl, Expression* cmdExpr)
{
    if (tl == NULL || tl->Count == 0)
    {
        //Invalid action trying to build command
        exit(EXIT_FAILURE);
    }

    Token* currToken = GetHeadToken(tl);

    if (currToken->Type != WORD)
    {
        // Syntax error command expected
        exit(EXIT_FAILURE);
    }

    Cmd* cmd = InitCmd();
    cmd->CmdLiteral = currToken->Literal;
    
    int inpath = FALSE;
    int outpath = FALSE;
    int arg = TRUE;
    int expectOperator = FALSE;
    
    //cargar argumentos
    while (tl->Count > 0)
    {
        currToken = tl->Head->Tok;
        if (IsReserved(currToken->Literal))
        {
            cmdExpr->_Cmd = cmd;
            return;
        }
        if (currToken->Type == WORD || currToken->Type == QUOTED)
        {
            InsertArg(cmd, currToken->Literal);
            currToken = GetHeadToken(tl);
            free(currToken);
            continue;
        }
        
        break;
    }
    
    expectOperator = TRUE;
    //cargar redirecciones
    while (tl->Count > 0)
    {
        currToken = tl->Head->Tok;
        if(currToken->Type == OPERATOR)
        {
            if(!expectOperator)
            {
                // Invalid syntax, redirection arg expected
                exit(EXIT_FAILURE);
            }
            if(currToken->Literal == ">")
            {
                cmd->HasOut = TRUE;
                cmd->HasAppend = FALSE;
                outpath = TRUE;
                inpath = FALSE;
            }
            else if(currToken->Literal == ">>")
            {
                cmd->HasOut = TRUE;
                cmd->HasAppend = TRUE;
                outpath = TRUE;
                inpath = FALSE;
            }
            else if(currToken->Literal == "<")
            {
                cmd->HasIn = TRUE;
                inpath = TRUE;
                outpath = FALSE;
            }
            else
            {
                break;
            }
            currToken = GetHeadToken(tl);
            free(currToken);
            currToken = GetHeadToken(tl);

            if (currToken == NULL || (currToken->Type != WORD && currToken->Type != QUOTED))
            {
                // Invalid syntax, redirection argument expected
                exit(EXIT_FAILURE);
            }

            if(inpath)
            {
                cmd->CmdInPath = currToken->Literal;
                inpath = FALSE;
            }
            else if(outpath)
            {
                cmd->CmdOutPath = currToken->Literal;
                outpath = FALSE;
            }
            else
            {
                //oh oh! something went wrong
                exit(EXIT_FAILURE);
            }

            free(currToken);
            continue;
        }
        else if(IsReserved(currToken->Literal))
        {
            break;
        }
        else
        {
            // Operator expected
            exit(EXIT_FAILURE);
        }
        
    }
    
    cmdExpr->_Cmd = cmd;

}