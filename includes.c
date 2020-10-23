#include "includes.h"


//#define EXIT_FAILURE 1

const char *OPERATORS[OPSLEN] = {"<", ">", "&", "|", ";", "`", ">>", "&&", "||"};

const char *BUILTINS[BUILTINLEN] = {"true", "false" , "cd" , "exit" , "history" , "help"};

pid_t shellId;


void Tokenize(const char *buf, TokenList *tl)
{
    //  Initial state of the Tokenize method variables
    int delimiter = FALSE;
    int quoted = FALSE;
    char prevChar = ' ';
    t_type prev = NONE;
    Token *currToken = InitToken();
    int len = strlen(buf);

    for (int i = 0; i < len; i++)
    {
        char currChar = buf[i];
        char *concat = currToken->Literal;
        concat = ConcatChar(concat, currChar);
        //printf("Concat : %s\t", concat);
        if (!quoted && prev == OPERATOR && IsOperator(concat))
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
            prev = NONE;
            concat = currToken->Literal;
            concat = ConcatChar(concat, currChar);
        }
        if (currChar == '"') // fix the quoted recognition
        {
            if (quoted)
            {
                InsertTokenBack(tl, currToken);
                currToken = InitToken();
                quoted = FALSE;
                prev = NONE;
            }
            else
            {
                if (prev != NONE)
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
        if (quoted)
        {
            currToken->Literal = concat;
            continue;
        }
        if (!quoted && IsOperator(ConcatChar("", currChar)))
        {
            if (prev != NONE)
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
        if (!quoted && currChar == ' ')
        {
            if (prev != NONE)
            {
                InsertTokenBack(tl, currToken);
                currToken = InitToken();
                prev = NONE;
            }
            continue;
        }
        if (prev == WORD)
        {
            currToken->Literal = concat;
            continue;
        }
        if (currChar == '#')
        {
            break;
        }
        currToken->Literal = concat;
        currToken->Type = WORD;
        prev = WORD;
    }

    if (prev != NONE)
    {
        InsertTokenBack(tl, currToken);
    }
}

int IsOperator(char *buf)
{
    for (int i = 0; i < OPSLEN; i++)
    {
        if (!strcmp(buf, OPERATORS[i]))
        {
            return TRUE;
        }
    }
    return FALSE;
}

TokenList *InitTokenList()
{
    TokenList *tl = (TokenList *)malloc(sizeof(TokenList));
    tl->Head = NULL;
    tl->Tail = NULL;
    tl->Count = 0;
    return tl;
}

void InsertTokenFront(TokenList *tl, Token *newTok)
{
    TokenNode *tn = (TokenNode *)malloc(sizeof(TokenNode));
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
        TokenNode *auxTn = tl->Head;
        tl->Head = tn;
        tn->Next = auxTn;
        auxTn->Prev = tn;
    }
    tl->Count++;
}

void InsertTokenBack(TokenList *tl, Token *newTok)
{
    TokenNode *tn = (TokenNode *)malloc(sizeof(TokenNode));
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
        tl->Tail = tn;
        tn->Prev = tl->Head;
        tl->Head->Next = tn;
    }
    else
    {
        TokenNode *auxTn = tl->Tail;
        tl->Tail = tn;
        tn->Prev = auxTn;
        auxTn->Next = tn;
    }

    tl->Count++;
}

Token *GetHeadToken(TokenList *tl)
{
    if (tl == NULL || tl->Count == 0)
    {
        return NULL;
    }

    TokenNode *node = tl->Head;
    if (tl->Count > 1)
    {
        tl->Head = node->Next;
        node->Next = NULL;
        tl->Head->Prev = NULL;
    }
    tl->Count--;

    return node->Tok;
}

Token *InitToken()
{
    Token *tok = (Token *)malloc(sizeof(Token));
    tok->Literal = "\0";
    tok->Type = NONE;
    return tok;
}

char *ConcatChar(char *str1, char _char)
{
    char aux[] = " \0";
    aux[0] = _char;
    char *strAux = (char *)calloc(strlen(str1) + 1, sizeof(char));
    strcat(strAux, str1);
    strcat(strAux, aux);
    return strAux;
}

//Operators.c
Cmd *InitCmd()
{
    Cmd *cmd = (Cmd *)malloc(sizeof(Cmd));

    cmd->CmdArgc = 0;
    cmd->CmdInPath = "";
    cmd->CmdLiteral = "";
    cmd->CmdOutPath = "";
    cmd->HasAppend = FALSE;
    cmd->HasIn = FALSE;
    cmd->HasOut = FALSE;

    return cmd;
}

Pipe_S *InitPipe_S()
{
    Pipe_S *_pipe = (Pipe_S *)malloc(sizeof(Pipe_S));

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

IFConditional *InitIFConditional()
{
    IFConditional *_ifCond = (IFConditional *)malloc(sizeof(IFConditional));

    _ifCond->_If = NULL;
    _ifCond->_Then = NULL;
    _ifCond->_Else = NULL;
    _ifCond->HasElse = FALSE;

    return _ifCond;
}

Expression *InitExpression()
{
    Expression *expr = (Expression *)malloc(sizeof(Expression));

    expr->ExprType = NONE;
    expr->_Chain = NONE;
    expr->PipeS = NULL;
    expr->_IfCond = NULL;
    expr->_Cmd = NULL;

    return expr;
}

ExpressionNode *InitExpressionNode()
{
    ExpressionNode *exprNode = (ExpressionNode *)malloc(sizeof(ExpressionNode));

    exprNode->Expr = NULL;
    exprNode->Next = NULL;
    exprNode->Prev = NULL;

    return exprNode;
}

ExpressionList *InitExpressionList()
{
    ExpressionList *exprL = (ExpressionList *)malloc(sizeof(ExpressionList));

    exprL->Head = NULL;
    exprL->Tail = NULL;
    exprL->Count = 0;

    return exprL;
}

void Pipe_SInsert(Pipe_S *pipeS, ExpressionNode *exprNode)
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

void Pipe_SInsertE(Pipe_S *pipeS, Expression *expr)
{
    ExpressionNode *exprNode = InitExpressionNode();
    exprNode->Expr = expr;
    Pipe_SInsert(pipeS, exprNode);
}

ExpressionNode *Pipe_SGet(Pipe_S *pipeS)
{
    if (pipeS->Count == 0)
    {
        return NULL;
    }

    ExpressionNode *auxExpr = InitExpressionNode();
    auxExpr = pipeS->Head;
    if (pipeS->Count > 1)
    {
        pipeS->Head = auxExpr->Next;
        pipeS->Head->Prev = NULL;
    }
    auxExpr->Next = NULL;
    pipeS->Count--;

    return auxExpr;
}

void ExpressionListInsert(ExpressionList *exprL, ExpressionNode *node)
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

void ExpressionListInsertE(ExpressionList *exprL, Expression *expr)
{
    ExpressionNode *auxExprNode = InitExpressionNode();
    auxExprNode->Expr = expr;
    ExpressionListInsert(exprL, auxExprNode);
}

ExpressionNode *ExpressionListGet(ExpressionList *exprL)
{
    if (exprL->Count == 0)
    {
        return NULL;
    }
    //printf("Inside ExpressionListGet\n");
    ExpressionNode *auxNode = InitExpressionNode();

    auxNode = exprL->Head;
    if (exprL->Count > 1)
    {
        exprL->Head = auxNode->Next;
        exprL->Head->Prev = NULL;
    }
    //printf("Inside ExpressionListGet\n");
    auxNode->Next = NULL;
    exprL->Count--;

    return auxNode;
}

void InsertArg(Cmd *cmd, char *arg)
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


// parser.c
int Parse(TokenList *tl, ExpressionList *superExpr)
{
    int expectWord = TRUE;
    int wasBG = FALSE;

    while (tl->Count > 0)
    {
        Expression *currExpr = InitExpression();
        Token *currToken = InitToken();
        currToken = tl->Head->Tok;

        if (expectWord && currToken->Type != WORD)
        {
            perror("Unexpected token, cmd or if expected");
            return FALSE;
        }
        if (!expectWord && currToken->Type == WORD)
        {
            perror("Unexpected token, cmd or if expected");
            return FALSE;
        }

        if (expectWord && currToken->Type == WORD)
        {
            currExpr->ExprType = OP_PIPE;
            int builPipe_ = BuildPipe(tl, currExpr, IFEXPECTED);
            if(!builPipe_)
            {
                return FALSE;
            }

            if (currExpr->PipeS->bg)
            {
                wasBG = TRUE;
            }

            ExpressionListInsertE(superExpr, currExpr);
            expectWord = FALSE;
            continue;
        }

        if (currToken->Type == OPERATOR)
        {
            currExpr->ExprType = OP_CHAIN;
            if (!strcmp(currToken->Literal, ";"))
            {
                currExpr->_Chain = END_CHAIN;
                wasBG = FALSE;
            }
            else if (wasBG)
            {
                perror("Invalid expression after &");
                return FALSE;
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
                perror("Invalid token in expression, expected &&, || or ;");
                return FALSE;
            }

            ExpressionListInsertE(superExpr, currExpr);
            currToken = GetHeadToken(tl);
            free(currToken);
            expectWord = TRUE;
            continue;
        }

        perror("Invalid input token, expected &&, || or ;");
        return FALSE;
    }

    return TRUE;
}

int BuildIf(TokenList *tl, Expression *ifExpr)
{
    //nullhere
    if (tl->Count == 0)
    {
        perror("Bad call to BuildIf");
        return FALSE;
    }

    Token *currToken = GetHeadToken(tl);
    IFConditional *_if = InitIFConditional();

    //nullhere
    if (strcmp(currToken->Literal, "if"))
    {
        perror("Bad call to buildIf");
        return FALSE;
    }

    free(currToken);
    Expression *_Pipe = InitExpression();
    _Pipe->ExprType = OP_PIPE;

    // lo que va entre el if y el then
    int buildPipe_ = BuildPipe(tl, _Pipe, THENEXPECTED);
    if(!buildPipe_)
    {
        return FALSE;
    }
    
    if (_Pipe->PipeS->bg)
    {
        perror("Invalid expression after &");
        return FALSE;
    }
    _if->_If = _Pipe;

    currToken = tl->Head->Tok;

    if (currToken == NULL || strcmp(currToken->Literal, "then"))
    {
        perror("Invalid syntax, 'then' expected");
        return FALSE;
    }

    currToken = GetHeadToken(tl);
    free(currToken);
    _Pipe = InitExpression();
    _Pipe->ExprType = OP_PIPE;

    //Lo que va despues del then
    buildPipe_ = BuildPipe(tl, _Pipe, ENDEXPECTED);
    if(!buildPipe_)
    {
        return FALSE;
    }

    if (_Pipe->PipeS->bg)
    {
        // Invalid expression after &
        return FALSE;
    }
    _if->_Then = _Pipe;

    currToken = tl->Head->Tok;

    if (currToken != NULL && !strcmp(currToken->Literal, "else"))
    {
        currToken = GetHeadToken(tl);
        free(currToken);
        _if->HasElse = TRUE;
        _Pipe = InitExpression();
        _Pipe->ExprType = OP_PIPE;
        buildPipe_ = BuildPipe(tl, _Pipe, ENDEXPECTED);
        if(!buildPipe_)
        {
            return FALSE;
        }

        if (_Pipe->PipeS->bg)
        {
            perror("Invalid expression after &");
            return FALSE;
        }
        _if->_Else = _Pipe;
        currToken = tl->Head->Tok;
    }

    if (currToken == NULL || strcmp(currToken->Literal, "end"))
    {
        perror("Invalid syntax, 'end' expected");
        return FALSE;
    }
    
    currToken = GetHeadToken(tl);
    ifExpr->_IfCond = _if;
    return TRUE;
}

int BuildPipe(TokenList *tl, Expression *pipeExpr, t_expected expect)
{
    if (tl == NULL || tl->Count == 0)
    {
        perror("Bad call to BuildPipe");
        return FALSE;
    }

    Token *currToken = tl->Head->Tok;
    if (currToken->Type != WORD)
    {
        perror("Invalid syntax, if or command expected");
        return FALSE;
    }

    Pipe_S *_pipe = InitPipe_S();

    while (tl->Count > 0)
    {
        Expression *currExpr = InitExpression();
        if (!strcmp(currToken->Literal, "if"))
        {
            currExpr->ExprType = OP_IF;
            int buildIf_ = BuildIf(tl, currExpr);
            if(!buildIf_)
            {
                return FALSE;
            }
        }
        else if(expect == THENEXPECTED && !strcmp(currToken->Literal, "then"))
        {
            break;
        }
        else if(expect == ENDEXPECTED && (!strcmp(currToken->Literal, "else") || !strcmp(currToken->Literal, "then")))
        {
            break;
        }
        else
        {
            currExpr->ExprType = OP_CMD;
            int buildCmd_ = BuildCmd(tl, currExpr);
            if(!buildCmd_)
            {
                return FALSE;
            }
        }

        Pipe_SInsertE(_pipe, currExpr);
        currToken = tl->Head->Tok;

        if (tl->Count == 0 || strcmp(currToken->Literal, "|"))
        {
            break;
        }
        currToken = GetHeadToken(tl);
        free(currToken);
        currToken = tl->Head->Tok;
    }

    if (tl->Count > 0 && !strcmp(currToken->Literal, "&"))
    {
        currToken = GetHeadToken(tl);
        free(currToken);
        _pipe->bg = TRUE;
    }

    pipeExpr->PipeS = _pipe;
    return TRUE;
}

int BuildCmd(TokenList *tl, Expression *cmdExpr)
{
    if (tl == NULL || tl->Count == 0)
    {
        perror("Invalid action trying to build command");
        return FALSE;
    }

    Token *currToken = GetHeadToken(tl);

    if (currToken->Type != WORD)
    {
        perror("Syntax error command expected");
        return FALSE;
    }

    Cmd *cmd = InitCmd();
    cmd->CmdLiteral = currToken->Literal;
    InsertArg(cmd, currToken->Literal);

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
            return TRUE;
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

        //printf("inside redirections, token: %i\n", currToken->Type);
        if (currToken->Type == OPERATOR)
        {
            if (!expectOperator)
            {
                perror("Invalid syntax, redirection arg expected");
                return FALSE;
            }
            if (!strcmp(currToken->Literal, ">"))
            {
                cmd->HasOut = TRUE;
                cmd->HasAppend = FALSE;
                outpath = TRUE;
                inpath = FALSE;
            }
            else if (!strcmp(currToken->Literal, ">>"))
            {
                cmd->HasOut = TRUE;
                cmd->HasAppend = TRUE;
                outpath = TRUE;
                inpath = FALSE;
            }
            else if (!strcmp(currToken->Literal, "<"))
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
                perror("Invalid syntax, redirection argument expected");
                return FALSE;
            }

            if (inpath)
            {
                cmd->CmdInPath = currToken->Literal;
                inpath = FALSE;
            }
            else if (outpath)
            {
                cmd->CmdOutPath = currToken->Literal;
                outpath = FALSE;
            }
            else
            {
                perror("oh oh! something went wrong");
                return FALSE;
            }

            free(currToken);
            continue;
        }
        else if (IsReserved(currToken->Literal))
        {
            break;
        }
        else
        {
            perror("Operator expected");
            return FALSE;
        }

    }

    cmdExpr->_Cmd = cmd;

    return TRUE;
}

// execute.c
int IsReserved(char *word)
{
    if (!strcmp(word, "if") || !strcmp(word, "then") || !strcmp(word, "else") || !strcmp(word, "end"))
    {
        return TRUE;
    }
    return FALSE;
}

void Execute(ExpressionList *exprL)
{
    if (exprL->Count == 0)
    {
        // no commands to execute
        return;
    }

    int lastStatus = 0;
    int currStatus = 0;
    int chain = NONE;
    int _pipe = TRUE;
    while (exprL->Count > 0)
    {
        ExpressionNode *currNode = ExpressionListGet(exprL);
        Expression *currExpr = currNode->Expr;
        if (_pipe && currExpr->ExprType != OP_PIPE)
        {
            perror("Invalid syntax");
            return;
        }

        if (_pipe)
        {
            
            currStatus = ExecutePipe(currExpr);

            _pipe = FALSE;

            if (chain == NONE)
            {
                lastStatus = currStatus;
                free(currExpr);
                continue;
            }
        }

        if (chain != NONE)
        {
            if (chain == AND_CHAIN)
            {
                lastStatus = (lastStatus && currStatus);
            }
            else if (chain == OR_CHAIN)
            {
                lastStatus = (lastStatus || currStatus);
            }
            else
            {
                perror("unexpected behaivor");
                return;
            }
            chain = NONE;
            free(currNode);
            continue;
        }

        if (!_pipe && currExpr->ExprType != OP_CHAIN)
        {
            perror("unexpected behaivor");
            return;
        }

        if (!_pipe)
        {
            chain = currExpr->_Chain;
            if (chain == AND_CHAIN && !lastStatus)
            {
                return;
            }
            if (chain == OR_CHAIN && lastStatus)
            {
                return;
            }
            if (chain == END_CHAIN)
            {
                lastStatus = TRUE;
                chain = NONE;
            }

            _pipe = TRUE;
        }

        free(currNode);
    }
}

void ExecuteIf(Expression *IfExpr, int inFd, int outFd)
{
    if (inFd != STDIN_FILENO)
    {
        dup2(inFd, STDIN_FILENO);
        close(inFd);
    }
    if (outFd != STDOUT_FILENO)
    {
        dup2(outFd, STDOUT_FILENO);
        close(outFd);
    }

    int ifStatus = TRUE;
    IFConditional* _if = IfExpr->_IfCond;
    
    ifStatus = ExecutePipe(_if->_If);

    // Si ifStatus es TRUE ejecutar el then.
    if(ifStatus)
    {
        
        int status = ExecutePipe(_if->_Then);
        
        // esperar a ver que devuelve el then.
        if(status)
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            exit(EXIT_FAILURE);
        }        
        
    }
    else if(_if->HasElse) // Si el if retorna FALSE entonces ejecutar el else si es que tiene.
    {
        int status = ExecutePipe(_if->_Else);
        
        // esperar a ver que devuelve el then.
        if(status)
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            exit(EXIT_FAILURE);
        }
    }
    else    // En caso de que el if sea falso y no haya Else retornar falso.
    {
        exit(EXIT_FAILURE);
    }
    
}

int ExecutePipe(Expression *pipeExpr)
{
    int fdPipe[2], inFd, outFd, lastStatus,
        backupStdin = dup(STDIN_FILENO), backupStdout = dup(STDOUT_FILENO);

    if (pipeExpr == NULL || pipeExpr->PipeS->Count == 0)
    {
        perror("Unexpected behaivor");
        return FALSE;
    }

    ExpressionNode *currNode = Pipe_SGet(pipeExpr->PipeS);
    Expression *currExpr = currNode->Expr;

    // get first command input
    inFd = GetInfd(currExpr);

    while (pipeExpr->PipeS->Count >= 0)
    {
        // Get current out file descriptor
        outFd = GetOutfd(currExpr);

        //Revisa si es el ultimo comando y prepara el pipe
        if (pipeExpr->PipeS->Count > 0)
        {
            int pp = pipe(fdPipe);
            if (pp < 0)
            {
                perror("Error pipying");
                return FALSE;
            }

            if (outFd == STDOUT_FILENO)
            {
                outFd = fdPipe[1];
            }
            else
            {
                close(fdPipe[1]);
            }
        }

        //execute cmd or if
        pid_t cpid = fork();
        if (cpid == 0)
        {
            signal(SIGINT, CtrlHandler);
            if(currExpr->ExprType == OP_CMD)
            {
                ExecuteCmd(currExpr, inFd, outFd);
            }
            else if (currExpr->ExprType == OP_IF)
            {
                ExecuteIf(currExpr, inFd, outFd);
            }
            else
            {
                perror("Unexpected type of expression in pipe");
                exit(EXIT_FAILURE);
            }
            
        }

        signal(SIGINT, SIG_IGN);
        int status;
        wait(&status);
        if(!status)
        {
            lastStatus = EXIT_SUCCESS;
        }
        else
        {
            lastStatus = EXIT_FAILURE;
        }
        

        // restore file descriptors
        if (inFd != GetInfd(currExpr))
        {
            close(inFd);
        }

        if (outFd != GetOutfd(currExpr))
        {
            close(outFd);
        }

        free(currNode);

        if (pipeExpr->PipeS->Count == 0)
        {
            break;
        }

        currNode = Pipe_SGet(pipeExpr->PipeS);
        currExpr = currNode->Expr;

        inFd = GetInfd(currExpr);

        // set input to pipe in case of no redirection
        if (inFd == STDIN_FILENO)
        {
            inFd = fdPipe[0];
        }
        else
        {
            close(fdPipe[0]);
        }
    }

    //int status;
    //wait(&status);
    if(lastStatus)
    {
        return FALSE;
    }

    return TRUE;
}

void ExecuteCmd(Expression *cmdExpr, int inFd, int outFd)
{
    if (inFd != STDIN_FILENO)
    {
        dup2(inFd, STDIN_FILENO);
        close(inFd);
    }
    if (outFd != STDOUT_FILENO)
    {
        dup2(outFd, STDOUT_FILENO);
        close(outFd);
    }
    if(IsBuiltIn(cmdExpr->_Cmd->CmdLiteral))
    {
        ExecuteBuiltIn(cmdExpr);
        exit(EXIT_FAILURE);
    }
    else
    {
        execvp(cmdExpr->_Cmd->CmdLiteral, cmdExpr->_Cmd->CmdArgv);
    }

    
    perror("Problam executing non built-in command");
    exit(EXIT_FAILURE);
}

void ExecuteBuiltIn(Expression* cmdExpr)
{
    char* cmd = cmdExpr->_Cmd->CmdLiteral;

    if(!strcmp(cmd, "true"))
    {
        True();
    }
    else if(!strcmp(cmd, "false"))
    {
        False();
    }
    else if (!strcmp(cmd , "cd"))
    {          
        if (cmdExpr->_Cmd->CmdArgc !=2) 
        {
            perror ("cd must has exactly one argument");
            exit(EXIT_FAILURE); 
        }
        else
        {
            Cd(cmdExpr->_Cmd);
        }
    }
    else if (!strcmp(cmd , "exit"))
    {
        _Exit_();
    }
    else if (!strcmp(cmd , "history" ))
    {
        execlp("./history.o", "./history.o", NULL );
    }
    else if (!strcmp(cmd, "help"))
    {
        
        /*
        Nota : aqui tener en cuenta el path global donde se ejecuta el fichero 
        */
        chdir("help");
        execlp("./help.o" , "./help.o" , cmdExpr->_Cmd->CmdArgv[1] , NULL);
    }
    else
    {
        perror("Not implemented Built-in command");
        exit(EXIT_FAILURE);
    }
    
}

int GetInfd(Expression *cmdExpr)
{
    int infd = STDIN_FILENO;

    if (cmdExpr->ExprType == OP_CMD && cmdExpr->_Cmd->HasIn)
    {
        infd = open(cmdExpr->_Cmd->CmdInPath, O_RDONLY);
        if (infd < 0)
        {
            perror("Not such a file or directory");
            exit(EXIT_FAILURE);
        }
    }

    return infd;
}

int GetOutfd(Expression *cmdExpr)
{
    int outfd = STDOUT_FILENO;

    if (cmdExpr->ExprType == OP_CMD && cmdExpr->_Cmd->HasOut)
    {
        if (cmdExpr->_Cmd->HasAppend)
        {
            outfd = open(cmdExpr->_Cmd->CmdOutPath, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
        }
        else
        {
            outfd = open(cmdExpr->_Cmd->CmdOutPath, O_CREAT | O_WRONLY, S_IRWXU);
        }

        if (outfd < 0)
        {
            perror("Error trying to open file for Writting");
            exit(1);
        }
    }

    return outfd;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Built-ins

int IsBuiltIn(char* buf)
{
    for (int i = 0; i < BUILTINLEN; i++)
    {
        if(!strcmp(buf, BUILTINS[i]))
        {
            return TRUE;
        }
    }

    return FALSE;    
}

void True()
{
    exit(EXIT_SUCCESS);
}

void False()
{
    exit(EXIT_FAILURE);
}

void _Exit_()
{
    int error = kill(shellId, SIGKILL);
    if(error)
    {
        perror("No se pudo ejecutar exit");
    }
    exit(EXIT_FAILURE);
}

void Cd(Cmd* cmdExpr)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////
//handlers


void CtrlHandler(int sig)
{
    printf("\nPresione Ctrl+c nuevamente para terminar proceso\n");
    signal(SIGINT, SIG_DFL);
}
