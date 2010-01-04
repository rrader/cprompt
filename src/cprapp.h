#ifndef CPRAPP_INCLUDED
#define CPRAPP_INCLUDED

#include "trees.h"
#include "cpr_parser.h"
#include "main.h"
#include "cprtypes.h"
#include "stack.h"
#include <fstream>

enum CPRDefineType
    {
        cdtNone=0,
        cdtConst=1,
        cdtMacro=2
    };

struct DefStackElement
{
    int iPos;
    bool Processing;
};

struct StartEndStruct
{
    int start,end;
    void* data;
};

struct CPRDefine
{
    CPRDefineType dt;
    char* name;
    ag::stringlist* params;
    char* expr;
};

class CPRApplication
{
public:
    CPRApplication()
    {
        aTree=new ag::tree<CPRTreeNode*>(MakeCPRTreeNode(tntNone,"APPLICATION"));
        aTypenames=new ag::stringlist(arTypenames,sizeof(arTypenames)/sizeof(void*));
    };
    ~CPRApplication(){};

    void SetText(char* sText);
    void SetFile(char* sText);
    char* GetFileText(char* sFName);

    void ParseIt(ag::list<CPRTokenInfo>* pTok=NULL,char* sText=NULL, bool bReadEoln=false, bool bReadSpaces=false);
    void PreParseIt(ag::list<CPRTokenInfo>* pTok=NULL,char* sText=NULL);
    void Preprocessing(char**saveto=NULL,char* sText=NULL, char* workdir=NULL);
    void BuildTree(char* workpath, ag::list<CPRTokenInfo>* pTok,char* sftext,ag::tree<CPRTreeNode*>*parent);
    void ExecMainTree(ag::tree<CPRTreeNode*>* T);
    void ExecTree(ag::tree<CPRTreeNode*>* T,ag::list<DTVar*>* ExternalVars=NULL, char* retname=NULL);
    void ExecOutside(ag::tree<CPRTreeNode*>* T);

    char* ReadTypename(ag::list<CPRTokenInfo>::member& p);
    bool  IsTypename(ag::list<CPRTokenInfo>::member p);
    char* ReadIdent(ag::list<CPRTokenInfo>::member* p, char* FText);
    char* ReadToEOLN(ag::list<CPRTokenInfo>::member* p, char* FText);
    char* ReadToSymbol(ag::list<CPRTokenInfo>::member& p,char _symb,bool makespaces);

    DTVar* FindVariable(char* sName, ag::list<DTVar*>* local);

    char* GetWorkDir()
    {
        char* k=new char[strlen(sWorkDir)+1];
        strcpy(k,sWorkDir);
        k[strlen(sWorkDir)]=0;
        return k;
    };

    char* GetCurrentFileText()
    {
        char* k=new char[strlen(sPText)+1];
        strcpy(k,sPText);
        k[strlen(sPText)]=0;
        return k;
    };

    ag::list<CPRTokenInfo> aTokens;
    ag::list<DTVar*> aVars;
    ag::stringlist* aTypenames;
    ag::tree<CPRTreeNode*>* aTree;
    ag::stack<DTVar*> aStack;
    ag::list<CPRDefine> sDefines;
protected:

private:
    char* sPText;
    int   iSize;
    char* sFilePath;
    char* sWorkDir;

    void PreprocessIfDefs       (char**saveto=NULL, char* sText=NULL, char* workdir=NULL);
    void PreprocessDefineConsts (char**saveto=NULL, char* fText=NULL, char* workdir=NULL);
    void PreprocessIncludes     (char**saveto=NULL, char* sText=NULL, char* workdir=NULL);
    void* CreateBufferFromStackStdCall(ag::list<CPRTextDataType>* params,int& WordCount);
};

ag::tree<CPRTreeNode*>* FindText1InTree(ag::tree<CPRTreeNode*>* T,char* sText,CPRTreeNodeType tnt=tntNone);
ag::tree<CPRTreeNode*>* FindText2InTree(ag::tree<CPRTreeNode*>* T,char* sText,CPRTreeNodeType tnt=tntNone);
ag::tree<CPRTreeNode*>* FindFunctionInTree(ag::tree<CPRTreeNode*>* T,char* sText);

#endif // CPRAPP_INCLUDED
