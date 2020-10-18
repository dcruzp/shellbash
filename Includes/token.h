#ifndef TOKEN
#define TOKEN
#include "types.h"


typedef struct Token
{
    t_type Type;
    t_literal Literal;
} Token;

typedef struct TokenNode{
    Token* Tok;
    struct TokenNode * Prev;
    struct TokenNode * Next;
} TokenNode;

typedef struct TokenList
{
    TokenNode* Head;
    TokenNode* Tail;
    int Count;
}TokenList;

int IsOperator(char* buf);
TokenList* InitTokenList();
void InsertTokenFront(TokenList* tl, Token* newTok);
void InsertTokenBack(TokenList* tl, Token* newTok);
Token* GetHeadToken(TokenList* tl);
Token* GetTailToken(TokenList* tl);
void* DelHeadToken(TokenList* tl);
void* DelTailToken(TokenList* tl);
void Tokenize(const char * buf, TokenList* tl);
Token* InitToken();
char * ConcatChar(char * str1, char _char);

#endif