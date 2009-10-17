#ifndef CPR_PARSER_INCLUDED
#define CPR_PARSER_INCLUDED

#include "sets.h"

enum CPRParserExpType {petNone=0,petString=1,petInt=2,petFloat=3,petOther=4,
                       petSpace=5,petEOLN=6,petEOF=7,petOper=8};

struct CPRTokenInfo
{
    int iCurrLength,iStartPos,iFinishPos;
    char* sCurrText;
    CPRParserExpType petCurrType;
    ag::set stCurrSymbs;
    void operator=(CPRTokenInfo& _sec);
};

class CPRParser
{
public:
    CPRParser(char* sText="",int offset=0);
    ~CPRParser();

    //Initialization with text
    void SetFile(char*);             //+
    void SetText(char*);             //+

    //Work
    char* Next(bool bClearLast=true);//+
    CPRParserExpType NewPosition();  //+
    CPRTokenInfo* NextPreview();      //+
    CPRTokenInfo* GetInfo();          //+
    char* ReadIdent();               //+
    char* ReadToEOLN();              //+
    void  PassSymbs();               //+

    //Options
    CPRParserExpType NowType();
    void  SetReadNegativeNumbers(bool Value);
    void  SetReadEOLN(bool Value);
    void  SetReadSpaces(bool Value);
    bool  GetReadSpaces();
    bool  GetNegativeNumbers();
    bool  GetReadEOLN();
    int   CurrPosition();
    int   CurrLength();
    char* NowStr();

    char* sPText;
    int iPosition;
    int iCurrLength;
    int iSize;
    char* sCurrText;
    CPRParserExpType petCurrType;
    ag::set stCurrSymbs;
    bool bReadSpaces;
    bool bReadEOLN;
    bool bReadNegativeNumbers;
};


#endif // CPR_PARSER_INCLUDED
