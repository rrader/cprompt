#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>

char* concat(char* s1,char* s2);

enum CPRTreeNodeType
    {
        tntNone=0,
        tntDeclareVar=1,
        tntFunction=2,
        tntDeclareFunc=3,
        tntExpression=4,
        tntDirective=98,
        tntVarOutput=99
    };

static char* arTypenames[]={"char","int","float","double","void","long","short","super","signed","unsigned","*","&"};

struct CPRTextDataType
{
    char *str1,*str2,*str3;
};

struct CPRTextVariable
{
    CPRTextDataType dt;
};

struct CPRTreeNode
{
    CPRTreeNodeType tntType;
    void* r1;
    void* r2;
    char *text,*text2,*text3,*text4;
    CPRTreeNode& operator=(CPRTreeNode& _s)
    {
        tntType=_s.tntType;
        if (_s.text!=NULL)
        {
            text=new char[strlen(_s.text)];
            strcpy(text,_s.text);
        }

        if (_s.text2!=NULL)
        {
            text2=new char[strlen(_s.text2)];
            strcpy(text2,_s.text2);
        }

        if (_s.text3!=NULL)
        {
            text3=new char[strlen(_s.text3)];
            strcpy(text3,_s.text3);
        }

        if (_s.text4!=NULL)
        {
            text4=new char[strlen(_s.text4)];
            strcpy(text4,_s.text4);
        }
        return *this;
    }
    operator char*();
};

CPRTreeNode* MakeCPRTreeNode(CPRTreeNodeType tp=tntNone, char* text=NULL,char*text2=NULL,char*text3=NULL,char*text4=NULL,void* rx1=NULL,void* rx2=NULL);
void FillCPRTreeNode(CPRTreeNode* k,CPRTreeNodeType tp=tntNone, char* text=NULL,char*text2=NULL,char*text3=NULL,char*text4=NULL);
#endif // MAIN_H_INCLUDED
