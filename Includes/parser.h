#ifndef PARSER
#define PARSER

#include "types.h"
#include "token.h"
#include "operators.h"

void Parse(TokenList* tl, ExpressionList* superExpr);

void BuildExpression(TokenList* tl, Expression* expr);

void BuildIf(TokenList* tl, Expression* ifExpr);

void BuildPipe(TokenList* tl, Expression* pipeExpr);

void BuildCmd(TokenList* tl, Expression* cmdExpr);

int IsReserved(char* word);

#endif