#ifndef RPNLIST_H_INCLUDED
#define RPNLIST_H_INCLUDED

struct DTVar;

#include "cprtypes.h"
#include <stdlib.h>
#include "stack.h"
#include <ctype.h>
#include <cstring>
#include <iostream>
#include "stringlist.h"
#include "cprapp.h"

enum RPNStackElementType {
            rsetNum=0,
            rsetStr=1,
            rsetAct=2
            };

struct RPNStackElement
{
    RPNStackElementType tp;
    void* d;
};

class rpnlist : public ag::list<RPNStackElement*>{};

bool isoper1(char o);
char isoper(char o,char n);
char isoperoneof(char o,char n,char* str);
char operprior(char o,char n);
rpnlist* MakePostfixFromInfix(char* infix);
ag::stack<DTVar*>* CalculateRPN(rpnlist* rpn);

#endif // RPNLIST_H_INCLUDED
