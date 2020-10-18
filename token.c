#include "./Includes/token.h"
#include "./Includes/types.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

const char * OPERATORS[OPSLEN] = {"<", ">", "&", "|",  ";", "`", ">>", "&&", "||"};

void Tokenize(const char* buf, TokenList* tl)
{
    //  Initial state of the Tokenize method variables
    int delimiter = FALSE;
    int quoted = FALSE;
    char prevChar = ' ';
    t_type prev = NONE;
    Token * currToken = InitToken();
    int len = strlen(buf);
    
    for(int i = 0; i < len; i++)
    {
        char currChar = buf[i];
        char * concat = currToken->Literal;
        concat = ConcatChar(concat, currChar);
        if(!quoted && prev == OPERATOR && IsOperator(concat))
        {
            currToken->Literal = concat;
            InsertTokenBack(tl, currToken);
            currToken = InitToken();
            prev = NONE;
            continue;
        }
        if (prev == OPERATOR && !IsOperator(concat))
        {
            InsertTokenBack(tl, currToken);
            currToken = InitToken();
            prev = NONE;concat = currToken->Literal;
            concat = ConcatChar(concat, currChar);
        }
        if(currChar == '"') // fix the quoted recognition
        {
            if(quoted) 
            { 
                InsertTokenBack(tl, currToken);
                currToken = InitToken();
                quoted = FALSE;
                prev = NONE;
            }
            else 
            { 
                if(prev != NONE)
                {
                    InsertTokenBack(tl, currToken);
                    currToken = InitToken();                 
                }
                currToken->Type = QUOTED;
                prev = QUOTED;
                quoted = TRUE;
            }
            
            continue;
        }
        if(quoted)
        {
            currToken->Literal = concat;
            continue;
        }
        if(!quoted && IsOperator(ConcatChar("", currChar)))
        {
            if(prev != NONE)
            {
                InsertTokenBack(tl, currToken);
                currToken = InitToken();
            }
            concat = currToken->Literal;
            concat = ConcatChar(concat, currChar);
            currToken->Literal = concat;
            currToken->Type = OPERATOR;
            prev = OPERATOR;
            continue;
        }
        if(!quoted && currChar == ' ')
        {
            if(prev != NONE)
            {
                InsertTokenBack(tl, currToken);
                currToken = InitToken();
                prev = NONE;
            }
            continue;
        }
        if(prev == WORD)
        {
            currToken->Literal = concat;
            continue;
        }
        if(currChar == '#')
        {
            break;
        }
        currToken->Literal = concat;
        currToken->Type = WORD;
        prev = WORD;
        
    }
    
    
    if(prev != NONE)
    {
        InsertTokenBack(tl, currToken);
    }
    
}

int IsOperator(char* buf)
{
    for(int i = 0; i < OPSLEN; i++)
    {
        if (!strcmp(buf ,OPERATORS[i]))
        {
            return TRUE;
        }
    }
    return FALSE;
}

TokenList* InitTokenList()
{
    TokenList* tl = (TokenList*)malloc(sizeof(TokenList));
    tl->Head = NULL;
    tl->Tail = NULL;
    tl->Count = 0;
    return tl;
}

void InsertTokenFront(TokenList* tl, Token* newTok)
{
    TokenNode* tn = (TokenNode*)malloc(sizeof(TokenNode));
    tn->Next = NULL;
    tn->Prev = NULL;
    tn->Tok = newTok;
    
    if (tl->Count == 0)
    {
        tl->Head = tn;
        tl->Tail = tn;
    }
    else if (tl->Count == 1)
    {
        tl->Head = tn;
        tl->Head->Next = tl->Tail;
        tl->Tail->Prev = tn;
    }
    else
    {
        TokenNode* auxTn = tl->Head;
        tl->Head = tn;
        tn->Next = auxTn;
        auxTn->Prev = tn;        
    }
    tl->Count++;
}

void InsertTokenBack(TokenList* tl, Token* newTok)
{
    TokenNode* tn = (TokenNode*)malloc(sizeof(TokenNode));
    tn->Next = NULL;
    tn->Prev = NULL;
    tn->Tok = newTok;

    if (tl->Count == 0)
    {
        tl->Head = tn;
        tl->Tail = tn;
    }
    else if(tl->Count == 1)
    {
        tl->Tail = tn;
        tn->Prev = tl->Head;
        tl->Head->Next = tn;
    }
    else
    {
        TokenNode* auxTn = tl->Tail;
        tl->Tail = tn;
        tn->Prev = auxTn;
        auxTn->Next = tn;
    }

    tl->Count++;
}

Token* InitToken()
{
    Token* tok = (Token*)malloc(sizeof(Token));
    tok->Literal = "";
    tok->Type = NONE;
    return tok;
}

char * ConcatChar(char * str1, char _char)
{
    char aux[] = " \0";    
    aux[0] = _char;
    char * strAux = (char*)malloc(strlen(str1) + 1);
    strcat(strAux, str1);
    strcat(strAux, aux);
    return strAux;
}