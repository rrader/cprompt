#ifndef CPRAPP_INCLUDED
#define CPRAPP_INCLUDED

#include "trees.h"
#include "cpr_parser.h"
#include "main.h"
#include "cprtypes.h"
#include "stack.h"
#include <fstream>

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

    void ParseIt(ag::list<CPRTokenInfo>* pTok=NULL,char* sText=NULL);
    void BuildTree(char* spath=NULL,char* sfullpath=NULL,ag::list<CPRTokenInfo>* pTok=NULL);
    void ExecMainTree(ag::tree<CPRTreeNode*>* T);
    void ExecTree(ag::tree<CPRTreeNode*>* T);

    char* ReadTypename(ag::list<CPRTokenInfo>::member& p);
    bool  IsTypename(ag::list<CPRTokenInfo>::member p);
    char* ReadIdent(ag::list<CPRTokenInfo>::member* p, char* FText);
    char* ReadToEOLN(ag::list<CPRTokenInfo>::member* p, char* FText);
    char* ReadToSymbol(ag::list<CPRTokenInfo>::member& p,char _symb);

    DTVar* FindVariable(char* sName, ag::list<DTVar*>* local);

    ag::list<CPRTokenInfo> aTokens;
    ag::list<DTVar*> aVars;
    ag::stringlist* aTypenames;
    ag::tree<CPRTreeNode*>* aTree;
    ag::stack<DTVar*> aStack;
protected:

private:
    char* sPText;
    int   iSize;
    char* sFilePath;
};

ag::tree<CPRTreeNode*>* FindText1InTree(ag::tree<CPRTreeNode*>* T,char* sText);
ag::tree<CPRTreeNode*>* FindText2InTree(ag::tree<CPRTreeNode*>* T,char* sText);

#endif // CPRAPP_INCLUDED
