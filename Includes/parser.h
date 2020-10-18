#ifndef PARSER
#define PARSER

#include "types.h"
#include "token.h"
#include "operators.h"

void Parse(TokenList* tl, ExpressionList* superExpr);

void BuildIf(TokenList* tl, Expression* ifExpr);

void BuildPipe(TokenList* tl, Expression* pipeExpr);

void BuildCmd(TokenList* tl, Expression* cmdExpr);

#endif