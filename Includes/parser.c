#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "token.h"
#include "operators.h"

void Parse(TokenList* tl, ExpressionList* superExpr)
{
    superExpr = InitExpressionList();
    int expectWord = TRUE;
    while (tl->Count > 0)
    {
        Expression* currExpr = InitExpression();
        Token* currToken = InitToken();
        currToken = tl->Head->Tok;

        if(expectWord && currToken->Type != WORD)
        {
            // Enexpected token, cmd or if expected
            exit(EXIT_FAILURE);
        }

        if(expectWord && !strcmp(currToken->Literal, "if"))
        {
            BuildIf(tl, currExpr);
            ExpressionListInsert(superExpr, currExpr);
            expectWord = FALSE;
            continue;
        }

        if(expectWord && currToken->Type == WORD)
        {
            BuildPipe(tl, currExpr);
            ExpressionListInsert(superExpr, currExpr);
            expectWord = FALSE;
            continue;
        }

        if(currToken->Type == OPERATOR)
        {
            if (strcmp(currToken->Literal, ";"))
            {
                currToken = GetHeadToken(tl);
            }
            else if (strcmp(currToken->Literal, "&&"))
            {
                currExpr->ExprType = AND_CHAIN;
            }
            else if (strcmp(currToken->Literal, "||"))
            {
                currExpr->ExprType = OR_CHAIN;
            }
            else
            {
                // Invalid token in expression, expected &&, || or ;
                exit(EXIT_FAILURE);
            }
            
            free(currToken);
            free(currExpr);
            expectWord = TRUE;
            continue;
        }

        //  Invalid input token, expected &&, || or ;
        exit(EXIT_FAILURE);
    }
    

    exit(EXIT_SUCCESS);
}

void BuildIf(TokenList* tl, Expression* ifExpr);

void BuildPipe(TokenList* tl, Expression* pipeExpr);

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
        //sigue aqui
    }
    

}