#include <iostream>
#include <string.h>
#include <fstream>
#include "cpr_parser.h"
#include <fstream>
extern bool debugmode;

char sTxtSymb[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
//АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯабвгдеёжзийклмнопрстуфхцчшщьыъэюя";
char sNumSymb[]="0123456789.e";
char sPassSymb[]=" \xD\xA\x9";
char sBreakSymb[]="\xD\xA";
char sEofSymb[]="\x0";
char sSpaceSymb[]=" ";
//char sQuoteStart[]="\"";
//char sQuoteEnd[]="\"\'";

ag::set stTxtSymb (sTxtSymb);
ag::set stNumSymb (sNumSymb);
ag::set stPassSymb(sPassSymb);
ag::set stBreakSymb(sBreakSymb);
ag::set stEofSymb(sEofSymb);
ag::set stSpaceSymb(sSpaceSymb);
ag::set sIdentSymbs(sTxtSymb);
ag::set sIdentSymbs2(sTxtSymb);
//ag::set sQuoteStartSymbs(sQuoteStart);
//ag::set sQuoteEndSymbs(sQuoteEnd);

void CPRTokenInfo::operator=(CPRTokenInfo& _sec)
{
    iCurrLength=_sec.iCurrLength;
    iStartPos=_sec.iStartPos;
    iFinishPos=_sec.iFinishPos;
    sCurrText=new char[strlen(_sec.sCurrText)+1];
    strcpy(sCurrText,_sec.sCurrText);
    sCurrText[strlen(_sec.sCurrText)]=0;
    stCurrSymbs=_sec.stCurrSymbs;
    petCurrType=_sec.petCurrType;
}

    CPRParserExpType CPRParser::NowType()
    {
        return petCurrType;
    }

    void CPRParser::SetReadNegativeNumbers(bool Value)
    {
        bReadNegativeNumbers=Value;
    }

    void CPRParser::SetReadEOLN(bool Value)
    {
        bReadEOLN=Value;
    }

    void CPRParser::SetReadQuotedStrings(bool Value)
    {
        bReadQuotedStrings=Value;
    }

    void CPRParser::SetReadSpaces(bool Value)
    {
        bReadSpaces=Value;
    }

    bool CPRParser::GetReadSpaces()
    {
        return bReadSpaces;
    };

    bool CPRParser::GetNegativeNumbers()
    {
        return bReadNegativeNumbers;
    };

    bool CPRParser::GetReadQuotedStrings()
    {
        return bReadQuotedStrings;
    };

    bool CPRParser::GetReadEOLN()
    {
        return bReadEOLN;
    };

    int  CPRParser::CurrPosition()
    {
        return iPosition;
    };

    int  CPRParser::CurrLength()
    {
        return iCurrLength;
    };

    char* CPRParser::NowStr()
    {
        return sCurrText;
    };

CPRParser::CPRParser(char* sText,int offset)
{
    SetText(sText);
    iPosition=offset;
    iCurrLength=0;
    sCurrText="";
    bReadSpaces=false;
    bReadEOLN=false;
    bReadNegativeNumbers=false;
    bReadQuotedStrings=false;
    bInTheQString=false;
    sIdentSymbs=sTxtSymb;
}

CPRParser::~CPRParser()
{
    delete[] sPText;
    sPText=NULL;
}

void CPRParser::SetText(char* sText)
{
    iSize=strlen(sText);
    sPText=new char[iSize+1];
    strcpy(sPText,sText);
    sPText[iSize]=0;
    iPosition=0;
}

void CPRParser::SetFile(char* sName)
{
    std::ifstream file;
    file.open (sName, std::ios::binary );

    file.seekg (0, std::ios::end);
    int length = file.tellg();
    file.seekg (0, std::ios::beg);
    sPText=new char[length];
    file.read(sPText,length);
    file.close();
    iPosition=0;
}

CPRTokenInfo* CPRParser::GetInfo()
{
    CPRTokenInfo* tmp=new CPRTokenInfo;
    tmp->iCurrLength=this->iCurrLength;
    tmp->iFinishPos=this->iPosition;
    tmp->iStartPos=this->iPosition-iCurrLength;
    tmp->petCurrType=this->petCurrType;
    tmp->sCurrText=this->sCurrText;
    tmp->stCurrSymbs=this->stCurrSymbs;
    return tmp;
}

CPRParserExpType CPRParser::NewPosition()
{
    if (iPosition>=iSize)
    {
        petCurrType=petEOF;
    }
    else
    {
        if (stSpaceSymb%sPText[iPosition])
        {
            petCurrType=petSpace;
            stCurrSymbs=stSpaceSymb;
        }
        else
            if (stTxtSymb%sPText[iPosition])
            {
                petCurrType=petString;
                stCurrSymbs=stTxtSymb;
            }
            else
                if ((stNumSymb-".e")%sPText[iPosition])
                {
                    petCurrType=petInt;
                    stCurrSymbs=stNumSymb;
                }
                else
                    if (stBreakSymb%sPText[iPosition])
                    {
                        petCurrType=petEOLN;
                        stCurrSymbs=stBreakSymb;
                    }
                    else
                        if ((sPText[iPosition]=='.')&&((iPosition+1)<iSize)&&
                                (ag::set("0123456789")%sPText[iPosition+1]))
                        {
                            petCurrType=petFloat;
                            stCurrSymbs=stNumSymb;
                        }
                        else
                            if ((sPText[iPosition]=='-')&&(bReadNegativeNumbers)&&((iPosition+1)<iSize)&&
                                    (ag::set("0123456789")%sPText[iPosition+1]))
                            {
                                petCurrType=petInt;
                                stCurrSymbs=stNumSymb;
                            }
                            else
                            {
                                petCurrType=petOther;
                                stCurrSymbs="";
                            }
    }
}

char* CPRParser::Next(bool bClearLast)
{
//    if (iPosition>=iSize)
//    {
//        petCurrType=petEOF;
//        return NULL;
//    }
    PassSymbs();
    NewPosition();
    char* sres=new char[256];
    memset(sres,0,256);
    int iLen=0;
    if (!bClearLast)
    {
        strcpy(sres,sCurrText);
        iLen=iCurrLength;
    }

    if ((bReadQuotedStrings)&&(sPText[iPosition]=='"'))
    {
        //int* K=new int;
        sres[iLen]=sPText[iPosition];
        if (debugmode) std::cout<<sres[iLen];
        if (debugmode) std::cout.flush();
        stCurrSymbs+=sres[iLen];
        iPosition++; iLen++;
        while(sPText[iPosition]!='"')
        {
            sres[iLen]=sPText[iPosition];
            if (debugmode) std::cout<<sres[iLen];
            if (debugmode) std::cout.flush();
            stCurrSymbs+=sres[iLen];
            iLen++;
            iPosition++;
            if (iPosition>=iSize) break;
        }
        sres[iLen]=sPText[iPosition];
        if (debugmode) std::cout<<sres[iLen]<<"\n";
        std::cout.flush();
        stCurrSymbs+=sres[iLen];
        iLen++;
        iPosition++;
        petCurrType=petQuotedStr;
    }else
    {
        do
        {
            if ((sPText[iPosition]=='-')&&(stCurrSymbs%"-"))
                stCurrSymbs-='-';
            if ((ag::set(sres)%'.')&&(sPText[iPosition]=='.'))
                break;

            if ((sPText[iPosition]=='e')&&((petCurrType==petFloat)||(petCurrType==petInt)))
            {
                if ((iPosition+1<iSize)&&(ag::set("0123456789-")%sPText[iPosition+1]))
                    stCurrSymbs+='-';
                else break;
            }

            if ((sPText[iPosition]=='.')&&(petCurrType=petInt))
                petCurrType=petFloat;

            sres[iLen]=sPText[iPosition];
            iLen++;
            iPosition++;
        }
        while (((iPosition<iSize)&&(stCurrSymbs.present(sPText[iPosition]))));
    }
    //sres[iLen]=0;
    sCurrText=new char[strlen(sres)+1];
    strcpy(sCurrText,sres);
    sCurrText[strlen(sres)]=0;
    iCurrLength=iLen;
    return sres;
}

CPRTokenInfo* CPRParser::NextPreview()
{
    CPRTokenInfo* tmp=this->GetInfo();
    Next();
    CPRTokenInfo* tmp2=this->GetInfo();
    this->iCurrLength=tmp->iCurrLength;
    this->iPosition=tmp->iStartPos;
    this->petCurrType=tmp->petCurrType;
    this->sCurrText=tmp->sCurrText;
    this->stCurrSymbs=tmp->stCurrSymbs;
    return tmp2;
}

char* CPRParser::ReadToEOLN()
{
    int iPos1=iPosition;
    int sz=0;
    while ((!(stBreakSymb%sPText[iPosition]))&&(iPosition<iSize))
    {
        iPosition++;
        sz++;
    }
    char* res=new char[sz+1];
    for (int i=0;i<sz;i++) res[i]=sPText[iPos1+i];
    res[sz]=0;
    PassSymbs();
    return res;
}

void CPRParser::PassSymbs()
{
    ag::set sPass(stPassSymb);
    if (this->bReadSpaces) sPass-=stSpaceSymb;
    if (this->bReadEOLN) sPass-=stBreakSymb;
    while (sPass%sPText[iPosition])iPosition++;
}

char* CPRParser::ReadIdent()
{
    if (debugmode) std::cout<<"CPRParser::ReadIdent()\n";
    //int* J=new int; //не трогать! q
    ag::set sAllowed;
    sAllowed=sIdentSymbs;
    int iPos1=iPosition-iCurrLength;
    int sz=0;
    while (sAllowed%sPText[iPosition-iCurrLength])
    {
        iPosition++;
        sz++;
        if (sz==1) sAllowed+=stNumSymb+"_";
    }
    if (sz==0)
    {
        Next();
        return sCurrText;
    }
    else
    {
        char* res=new char[sz+1];
        for (int i=0;i<sz;i++) res[i]=sPText[iPos1+i];
        res[sz]=0;
        Next();
        return res;
    }
}
