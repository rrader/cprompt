#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <dlfcn.h>

#include "lists.h"
#include "stringlist.h"
#include "cprapp.h"
#include "cprtypes.h"
#include "cprexec.h"

extern int argc;
extern char** argv;
extern int argnum;

void CPRApplication::SetText(char* sText)
{
    iSize=strlen(sText);
    sPText=new char[iSize+1];
    strcpy(sPText,sText);
    sPText[iSize]=0;
}

void CPRApplication::SetFile(char* sName)
{
    std::ifstream file;
    file.open (sName, std::ios::binary );

    file.seekg (0, std::ios::end);
    int length = file.tellg();
    file.seekg (0, std::ios::beg);
    sPText=new char[length+1];
    file.read(sPText,length);
    sPText[length]=0;
    file.close();

    int tmp=strlen(sName);
    sFilePath=new char[tmp+1];
    strcpy(sFilePath,sName);
    sFilePath[tmp]=0;

    sWorkDir=new char[strlen(sName)+1];
    strcpy(sWorkDir,sName);
    sWorkDir[strlen(sName)]=0;
    while((sWorkDir[strlen(sWorkDir)-1]!='/')&&(strlen(sWorkDir)>0)) sWorkDir[strlen(sWorkDir)-1]=0;
}

char* CPRApplication::GetFileText(char* sFName)
{
    char* res;
    std::ifstream file;
    file.open (sFName, std::ios::binary );

    file.seekg (0, std::ios::end);
    int length = file.tellg();
    file.seekg (0, std::ios::beg);
    res=new char[length+1];
    file.read(res,length);
    res[length]=0;
    file.close();
    return res;
}

void CPRApplication::ParseIt(ag::list<CPRTokenInfo>* pTok,char* sText,bool bReadEoln, bool bReadSpaces, bool bReadQStr)
{
    char* psText;
    if (sText==NULL)
    {
        psText=sPText;
    }else
    {
        psText=sText;
    }
    CPRParser cParser(psText);
    cParser.SetReadEOLN(bReadEoln);
    cParser.SetReadSpaces(bReadSpaces);
    cParser.SetReadQuotedStrings(bReadQStr);
    CPRTokenInfo i;
    ag::list<CPRTokenInfo>*pTokens;
    if (pTok!=NULL)
        pTokens=pTok;
    else
        pTokens=&aTokens;

    cParser.Next();
    while (cParser.NowType()!=petEOF)
    {
        i=*(cParser.GetInfo());
        pTokens->add_tail(i);
        cParser.Next();
    }
}

DTVar* CPRApplication::FindVariable(char* sName, ag::list<DTVar*>* local)
{
    char* s;
    if (local!=NULL)
    {
        for(ag::list<DTVar*>::member i=local->tail;i!=NULL;i=i->prev)
        {
            s=((DTMain*)(((DTVar*)i->data)->T))->sIdent;
            if (strcmp(s,sName)==0)
            {
                return i->data;
                break;
            }
        }
    }
    for(ag::list<DTVar*>::member i=aVars.tail;i!=NULL;i=i->prev)
    {
        s=((DTMain*)(((DTVar*)i->data)->T))->sIdent;
        if (strcmp(s,sName)==0)
        {
            return i->data;
            break;
        }
    }
    return NULL;
}

char* CPRApplication::ReadTypename(ag::list<CPRTokenInfo>::member& p)
{
    if (debugmode) std::cout<<"CPRApplication::ReadTypename\n";
    ag::stringlist sl;
    ag::stringlist::member sl_m;
    sl_m=aTypenames->findstr(p->data.sCurrText);
    sl.delall();
    while(sl_m!=NULL)
    {
        sl.addstr(p->data.sCurrText);
        if (debugmode) std::cout<<"part of type (): '"<<p->data.sCurrText<<"'\n";
        p=p->next;
        sl_m=aTypenames->findstr(p->data.sCurrText);
    };
    p=p->prev;

    int iSz=0;

    for(sl_m=sl.head;sl_m!=NULL;sl_m=sl_m->next)
        iSz+=strlen(sl_m->data)+1;
    iSz--;
    char* str=new char[iSz+1];
    int iSz2=0;
    for(sl_m=sl.head;sl_m!=NULL;sl_m=sl_m->next)
    {
        for(int i=0;i<strlen(sl_m->data);i++)
            str[i+iSz2]=sl_m->data[i];
        iSz2+=strlen(sl_m->data)+1;
        str[iSz2-1]=' ';
    }
    str[iSz]=0;
    if (debugmode) std::cout<<"Typename: "<<str<<"\n";
    return str;
}

bool CPRApplication::IsTypename(ag::list<CPRTokenInfo>::member p)
{
    return (((aTypenames->findstr(p->data.sCurrText))!=NULL)&&((aTypenamesExclude->findstr(p->data.sCurrText))==NULL));
}

char* CPRApplication::ReadToSymbol(ag::list<CPRTokenInfo>::member& p,char _symb,bool makespaces)
{
    ag::stringlist sl;
    do
    {
        sl.addstr(p->data.sCurrText);
        p=p->next;
        if (makespaces) sl.addstr(" ");
    }while((p->data.sCurrText[0]!=_symb)&&(p!=NULL));
    return sl.makestrfromelements();
}

char* CPRApplication::ReadIdent(ag::list<CPRTokenInfo>::member* p, char* FText)
{
    if (debugmode) std::cout<<"CPRApplication::ReadIdent()\n";
    CPRParser prs(FText,(*p)->data.iStartPos);
    int* E=new int;
    char* m  = prs.ReadIdent();
    //trim
    while(m[0]==' ')m++;
    while(m[strlen(m)-1]==' ')m[strlen(m)-1]=0;
/*    char* m2 = ((strcmp(m,"*")==0)||(strcmp(m,"&")==0))?prs.ReadIdent():NULL;
    if (m2!=NULL) strcat(m,m2);*/
    while((*p)->data.iStartPos<prs.iPosition-1)(*p)=(*p)->next;
    (*p)=(*p)->prev;
    if (debugmode) std::cout<<"Ident: "<<m<<"\n";
    return m;
}

char* CPRApplication::ReadToEOLN(ag::list<CPRTokenInfo>::member* p, char* FText)//!!!!!!!!!!!!!!!!!
{
    CPRParser prs(FText,(*p)->data.iStartPos);
    char* m  = prs.ReadToEOLN();
/*    char* m2 = ((strcmp(m,"*")==0)||(strcmp(m,"&")==0))?prs.ReadIdent():NULL;
    if (m2!=NULL) strcat(m,m2);*/
    while((*p)->data.iStartPos<prs.iPosition-1)(*p)=(*p)->next;
    (*p)=(*p)->prev;
    return m;
}

void CPRApplication::PreprocessIfDefs(char** saveto, char* sText, char* workdir)
{
    // обработка: define, ifdef, ifndef, endif, elif
    //ag::tree<> pPpcsTree;
    ag::list<CPRTokenInfo>* pTok=new ag::list<CPRTokenInfo>;
    ag::list<StartEndStruct> StartEndList;
    CPRDefine dfn;
    ag::stack<DefStackElement> DefStack;
    ParseIt(pTok,sText,true,true);
    DefStackElement dse;
    dse.iPos=0;
    dse.Processing=true;
    DefStack.add_tail(dse);

    for(ag::list<CPRTokenInfo>::member p=pTok->head;(p!=NULL)&&(p->data.petCurrType!=petEOF);p=p->next)
    {
        if (p->data.sCurrText[0]=='#')
        {
            do p=p->next; while (p->data.sCurrText[0]==' ');
            if (DefStack.empty()||(DefStack.tail->data.Processing))
            {
                if (strcmp(p->data.sCurrText,"define")==0)
                {
                    do p=p->next; while (p->data.sCurrText[0]==' ');
                    char* ident;
                    ident=ReadIdent(&p,sText);
                    if (p->data.sCurrText[0]=='(') dfn.dt=cdtMacro; else dfn.dt=cdtConst;
                    if (dfn.dt==cdtConst)
                    {
                        dfn.params=NULL;
                        dfn.name=ident;
                        while (p->data.sCurrText[0]==' ') p=p->next;
                        if (p->data.sCurrText[0]!='\n')
                        {
                            dfn.expr=ReadToEOLN(&p,sText);
                        }else
                            dfn.expr=NULL;
                    }
                    if (dfn.dt==cdtMacro)
                    {
                        //
                    }
                    bool replace=false;
                    for(ag::list<CPRDefine>::member m=sDefines.head;m!=NULL;m=m->next)
                    {
                        if(strcmp(m->data.name,ident)==0)
                        {
                            m->data.dt=dfn.dt;
                            m->data.params=dfn.params;
                            m->data.expr=dfn.expr;
                            replace=true;
                            break;
                        }
                    }
                    if (!replace)
                        sDefines.add_tail(dfn);
                }
                if (strcmp(p->data.sCurrText,"undef")==0)
                {
                    do p=p->next; while (p->data.sCurrText[0]==' ');
                    std::string ident;
                    while(sIdentSymbs%p->data.sCurrText[0])
                    {
                        ident+=p->data.sCurrText;
                        p=p->next;
                    }
                    for(ag::list<CPRDefine>::member m=sDefines.head;m!=NULL;m=m->next)
                    {
                        if(strcmp(m->data.name,ident.c_str())==0)
                        {
                            sDefines.del(m);
                            break;
                        }
                    }
                }
                if ((strcmp(p->data.sCurrText,"ifdef")==0)||(strcmp(p->data.sCurrText,"ifndef")==0))
                {
                    //if (StartEndList.count()==1) {StartEndList.tail->data.end=p->data.iStartPos;};
                    StartEndStruct ses;
                    ses.start=DefStack.tail->data.iPos;
                    ses.end=p->prev->data.iStartPos;
                    StartEndList.add_tail(ses);
                    do p=p->next; while (p->data.sCurrText[0]==' ');
                    char* ident;
                    ident=ReadIdent(&p,sText);
                    bool def=false;
                    for(ag::list<CPRDefine>::member m=sDefines.head;m!=NULL;m=m->next)
                    {
                        if(strcmp(m->data.name,ident)==0)
                        {
                            def=true;
                            break;
                        }
                    }
                    DefStackElement dse;
                    dse.iPos = p->data.iStartPos;
                    if (strcmp(p->data.sCurrText,"ifdef")==0)
                        dse.Processing=def;
                    else
                        dse.Processing=!def;
                    DefStack.push(dse);
                }
            }
            if (strcmp(p->data.sCurrText,"else")==0)
            {
                DefStack.tail->data.Processing=!DefStack.tail->data.Processing;
                if (DefStack.tail->data.Processing)
                {
                    DefStack.tail->data.iPos=p->data.iFinishPos;
                    StartEndStruct ses;
                }else
                {
                    StartEndStruct ses;
                    ses.start=DefStack.tail->data.iPos;
                    ses.end=p->prev->data.iStartPos;
                    StartEndList.add_tail(ses);
                }
                do p=p->next; while (p->data.sCurrText[0]==' ');
            }
            if (strcmp(p->data.sCurrText,"endif")==0)
            {
                if (DefStack.tail->data.Processing)
                {
                    StartEndStruct ses;
                    ses.start=DefStack.tail->data.iPos;
                    ses.end=p->prev->data.iStartPos;
                    StartEndList.add_tail(ses);
                }
                do p=p->next; while (p->data.sCurrText[0]==' ');
                DefStack.pop();
                DefStack.tail->data.iPos=p->prev->data.iFinishPos;
            }
        }
    }
    StartEndStruct ses;
    ses.start=DefStack.tail->data.iPos;
    ses.end=strlen(sText)-1;
    StartEndList.add_tail(ses);
    DefStack.pop();
    if (!DefStack.empty()) throw "The count of #ifdef's and #endif's is not equall";

    std::string res;
    char* k;
    res="";
    for(ag::list<StartEndStruct>::member p=StartEndList.head;(p!=NULL);p=p->next)
    {
        k=new char[p->data.end-p->data.start+1];
        strncpy(k,sText+p->data.start,p->data.end-p->data.start);
        k[p->data.end-p->data.start]=0;
        res+=k;
        delete[] k;
    }
    k=new char[res.size()+1];
    strcpy(k,res.c_str());
    k[res.size()]=0;
    ((saveto!=NULL)?*saveto:sPText) = k;
}

void CPRApplication::PreprocessIncludes(char** saveto, char* sText, char* workdir)
{
    // обработка инклудов
    ag::list<CPRTokenInfo>* pTok=new ag::list<CPRTokenInfo>;
    ParseIt(pTok,sText,true,true);
    ag::list<StartEndStruct> StartEndList;
    StartEndStruct ses;

    for(ag::list<CPRTokenInfo>::member p=pTok->head;(p!=NULL)&&(p->data.petCurrType!=petEOF);p=p->next)
    {
        if (p->data.sCurrText[0]=='#')
        {
            ses.start=p->data.iStartPos;
            do p=p->next; while (p->data.sCurrText[0]==' ');
            if (strcmp(p->data.sCurrText,"include")==0)
            {
                do p=p->next; while (p->data.sCurrText[0]==' ');
                int IncludeType = (p->data.sCurrText[0]=='"')?1:( (p->data.sCurrText[0]=='<')?2:-1 );
                if (IncludeType==-1) throw "Invalid include directive";

                if (IncludeType==1)
                {
                    do p=p->next; while (p->data.sCurrText[0]==' ');
                    std::string wd=workdir;
                    if (workdir[strlen(workdir)-1]!='/')
                        wd+='/';
                    std::string sPath;
                    do{ sPath+=p->data.sCurrText; p=p->next;}while ((p->data.sCurrText[0]!='"')&&(p!=NULL));
                    do p=p->next; while (p->data.sCurrText[0]==' ');
                    if (p==NULL) throw "Second brace is not found";
                    sPath=wd+sPath;
                    if (debugmode) std::cout<<"Path: "<<sPath<<"\n";
                    if (debugmode) std::cout<<"Work: "<<wd   <<"\n";
                    char** inc=new char*;
                    ses.end=p->data.iStartPos;
                    Preprocessing(inc,GetFileText((char*)sPath.c_str()),(char*)wd.c_str());
                    ses.data=inc;
                    StartEndList.add_tail(ses);
                }
                if (IncludeType==2)
                {
                    do p=p->next; while (p->data.sCurrText[0]==' ');
                    char* incpath=getenv("CPROMPTINCLUDES");
                    if(incpath==NULL)
                    {
                        throw "(FATAL ERROR) Enviromnent variable CPROMPTINCLUDES is not initialized\n";
                    }
                    std::string wd=incpath;
                    if (incpath[strlen(incpath)-1]!='/')
                        wd+='/';
                    std::string sPath;
                    do{ sPath+=p->data.sCurrText; p=p->next;}while ((p->data.sCurrText[0]!='>')&&(p!=NULL));
                    do p=p->next; while (p->data.sCurrText[0]==' ');
                    if (p==NULL) throw "Second brace is not found";
                    sPath=wd+sPath;
                    if (debugmode) std::cout<<"Path: "<<sPath<<"\n";
                    if (debugmode) std::cout<<"Work: "<<wd   <<"\n";
                    char** inc=new char*;
                    ses.end=p->data.iStartPos;
                    Preprocessing(inc,GetFileText((char*)sPath.c_str()),(char*)wd.c_str());
                    ses.data=inc;
                    StartEndList.add_tail(ses);
                }
            }
        }
    }
    char* k;
    std::string res="";
    int last=0;
    for(ag::list<StartEndStruct>::member p=StartEndList.head;(p!=NULL);p=p->next)
    {
        k=new char[p->data.start+1];
        strncpy(k,sText+last,p->data.start-last);
//        if (debugmode) std::cout<<": "<<k<<"\n";
        k[(p->data.start)-last]=0;
        res+=k;
        delete[] k;
        res+=*(char**)(p->data.data);
        last=p->data.end;
    }
    k=new char[strlen(sText)-last+1];
    strncpy(k,sText+last,strlen(sText)-last);
//    if (debugmode) std::cout<<p->data.start<<' '<<p->data.end<<": "<<k;
    k[strlen(sText)-last]=0;
    res+=k;
    delete[] k;

    k=new char[res.size()+1];
    strcpy(k,res.c_str());
    k[res.size()]=0;
    ((saveto!=NULL)?*saveto:sPText) = k;
}

void CPRApplication::PreprocessDefineConsts(char** saveto, char* fText, char* workdir)
{
    // обработка дефайн-констант
    char* sText=new char[strlen(fText)+1];
    strcpy(sText,fText);
    sText[strlen(fText)]=0;
    ag::list<CPRTokenInfo>* pTok=new ag::list<CPRTokenInfo>;
    ParseIt(pTok,sText,true,true);
    ag::list<StartEndStruct> StartEndList;
    ag::list<StartEndStruct> ToErase;
    StartEndStruct ses;
    CPRDefine dfn;
    ag::list<CPRTokenInfo>* Tk=new ag::list<CPRTokenInfo>;

    //ag::list<CPRDefine> sDefinesL;
    int iSt;

    for(ag::list<CPRTokenInfo>::member p=pTok->head;(p!=NULL);p=p->next)
    {
        if (p->data.petCurrType==petEOF) break;
        if (p->data.sCurrText[0]=='#')
        {
            iSt=p->data.iStartPos;
            do p=p->next; while (p->data.sCurrText[0]==' ');
            if (strcmp(p->data.sCurrText,"pragma")==0)
            {
                sText[iSt]=' ';
                ReadToEOLN(&p,sText);
            }
            if (strcmp(p->data.sCurrText,"define")==0)
            {
                do p=p->next; while (p->data.sCurrText[0]==' ');
                char* ident;
                ident=ReadIdent(&p,sText);
                if (p->data.sCurrText[0]=='(') dfn.dt=cdtMacro; else dfn.dt=cdtConst;
                if (dfn.dt==cdtConst)
                {
                    dfn.params=NULL;
                    dfn.name=ident;
                    while (p->data.sCurrText[0]==' ') p=p->next;
                    if (p->data.sCurrText[0]!='\n')
                    {
                        dfn.expr=ReadToEOLN(&p,sText);
                    }else
                        dfn.expr=NULL;
                }
                if (dfn.dt==cdtMacro)
                {
                    //
                }
                bool replace=false;
                for(ag::list<CPRDefine>::member m=sDefines.head;m!=NULL;m=m->next)
                {
                    if(strcmp(m->data.name,ident)==0)
                    {
                        m->data.dt=dfn.dt;
                        m->data.params=dfn.params;
                        m->data.expr=dfn.expr;
                        replace=true;
                        break;
                    }
                }
                if (!replace)
                    sDefines.add_tail(dfn);
            }
            if (strcmp(p->data.sCurrText,"undef")==0)
            {
                do p=p->next; while (p->data.sCurrText[0]==' ');
                std::string ident;
                while(sIdentSymbs%p->data.sCurrText[0])
                {
                    ident+=p->data.sCurrText;
                    p=p->next;
                }
                for(ag::list<CPRDefine>::member m=sDefines.head;m!=NULL;m=m->next)
                {
                    if(strcmp(m->data.name,ident.c_str())==0)
                    {
                        sDefines.del(m);
                        break;
                    }
                }
            }
        }
        ag::list<CPRTokenInfo>::member Tkm;
        ag::list<CPRDefine>::member b;
        //if (debugmode) std::cout<<">>";
        /*for(ag::list<CPRDefine>::member kms=sDefines.head;kms!=NULL;kms=kms->next)
                if (debugmode) std::cout<<kms->data.name<<"\n"; */
        for(ag::list<CPRDefine>::member m=sDefines.head;m!=NULL;m=m->next)
        {
            Tk->delall();
            ParseIt(Tk,m->data.name,true,true);
            Tkm=Tk->head;
            ses.start=p->data.iStartPos;
            while((strcmp(Tkm->data.sCurrText,p->data.sCurrText)==0))
            {
                Tkm=Tkm->next;
                p=p->next;
                if (Tkm==NULL)
                {
                    //Нашли!
                    ses.end=p->prev->data.iFinishPos;
                    char* j=new char[strlen(m->data.expr)+1];
                    strcpy(j,m->data.expr);
                    j[strlen(m->data.expr)]=0;
                    ses.data=j;
                    if (ses.data!=NULL)
                        StartEndList.add_tail(ses);
                    break;
                }
            }
        }
    }

    char* k;
    std::string res;
    int last=0;
    for(ag::list<StartEndStruct>::member p=StartEndList.head;(p!=NULL);p=p->next)
    {
        k=new char[p->data.start+1];
        strncpy(k,sText+last,p->data.start-last);
//        if (debugmode) std::cout<<": "<<k<<"\n";
        k[(p->data.start)-last]=0;
        res+=k;
        delete[] k;
        if (p->data.data!=NULL)
            res+=(char*)(p->data.data);
        last=p->data.end;
    }
    k=new char[strlen(sText)-last+1];
    strncpy(k,sText+last,strlen(sText)-last);
//    if (debugmode) std::cout<<p->data.start<<' '<<p->data.end<<": "<<k;
    k[strlen(sText)-last]=0;
    res+=k;
//    delete[] k;
    k=new char[res.size()+1];
    strcpy(k,res.c_str());
    k[res.size()]=0;


    pTok->delall();
    ToErase.delall();
    ParseIt(pTok,k,true,true);
    for(ag::list<CPRTokenInfo>::member p=pTok->head;(p!=NULL)&&(p->data.petCurrType!=petEOF);p=p->next)
    {
        if (p->data.sCurrText[0]=='#')
        {
            iSt=p->data.iStartPos;
            do p=p->next; while (p->data.sCurrText[0]==' ');
            if (strcmp(p->data.sCurrText,"define")==0)
            {
                ReadToEOLN(&p,k);
                ses.start=iSt;
                ses.end=p->data.iFinishPos;
                ses.data=NULL;
                ToErase.add_tail(ses);
            }
            if (strcmp(p->data.sCurrText,"undef")==0)
            {
                ReadToEOLN(&p,k);
                ses.start=iSt;
                ses.end=p->data.iFinishPos;
                ses.data=NULL;
                ToErase.add_tail(ses);
            }
        }
    }

    res="";
    char* y;
    last=0;
    for(ag::list<StartEndStruct>::member p=ToErase.head;(p!=NULL);p=p->next)
    {
        y=new char[p->data.start+1];
        strncpy(y,k+last,p->data.start-last);
//        if (debugmode) std::cout<<": "<<k<<"\n";
        y[(p->data.start)-last]=0;
        res+=y;
        delete[] y;
        if (p->data.data!=NULL)
            res+=(char*)(p->data.data);
        last=p->data.end;
    }
    y=new char[strlen(k)-last+1];
    strncpy(y,k+last,strlen(k)-last);
//    if (debugmode) std::cout<<p->data.start<<' '<<p->data.end<<": "<<k;
    y[strlen(k)-last]=0;
    res+=y;
    delete[] y;
    //if (debugmode) std::cout<<res;
    y=new char[res.size()+1];
    strcpy(y,res.c_str());
    y[res.size()]=0;


    ((saveto!=NULL)?*saveto:sPText) = y;
}

void CPRApplication::PreprocessComments(char** saveto, char* fText, char* workdir)
{
    // обработка комментариев
    char* sText=new char[strlen(fText)+1];
    strcpy(sText,fText);
    sText[strlen(fText)]=0;
    ag::list<CPRTokenInfo>* pTok=new ag::list<CPRTokenInfo>;
    ParseIt(pTok,sText,true,true);
    ag::list<StartEndStruct> StartEndList;
    ag::list<StartEndStruct> ToErase;
    StartEndStruct ses;
    CPRDefine dfn;
    ag::list<CPRTokenInfo>* Tk=new ag::list<CPRTokenInfo>;

    //ag::list<CPRDefine> sDefinesL;
    int iSt,iEnd;

    for(ag::list<CPRTokenInfo>::member p=pTok->head;(p!=NULL);p=p->next)
    {
        if (p->data.petCurrType==petEOF) break;
        if (p->data.sCurrText[0]=='/')
        {
            p=p->next;
            if (p->data.sCurrText[0]=='/')
            {
                iSt=p->data.iStartPos;
                ReadToEOLN(&p,sText);

                iEnd=p->data.iFinishPos;
                for (int j=iSt;j<=iEnd;j++)
                {
                    sText[j]=' ';
                }
            }
        }
    }

    ((saveto!=NULL)?*saveto:sPText) = sText;
}

void CPRApplication::Preprocessing(char** saveto, char* sText, char* workdir)
{
    // 1) обработка инклудов
    PreprocessIncludes     (saveto,sText,workdir);
    // 2) обработка: define, ifdef, ifndef, endif, elif
    PreprocessIfDefs       (saveto,((saveto!=NULL)?*saveto:sPText),workdir);
    PreprocessDefineConsts (saveto,((saveto!=NULL)?*saveto:sPText),workdir);
    // 3) обработка pragma
    ;
    // 4) обработка комментариев
    //PreprocessComments      (saveto,((saveto!=NULL)?*saveto:sPText),workdir);
}

bool CPRApplication::HashBang()
{
    if ((sPText[0]=='#')&&(sPText[1]=='!'))
    {
        while((sPText[0]!='\xA')&&(sPText[0]!='\x0')) sPText++;
        sPText++;
    }
}

void CPRApplication::BuildTree(char* workpath, ag::list<CPRTokenInfo>* pTok,char* sftext,ag::tree<CPRTreeNode*>*parent)
{
    if (debugmode) std::cout<<"CPRApplication::BuildTree()\n";
    int state=0;
    ag::tree<CPRTreeNode*>* mainparent=this->aTree;
    ag::tree<CPRTreeNode*>* funcparent=FindText1InTree(aTree,"FUNCTIONS");
    if (funcparent==NULL)
        funcparent=this->aTree->addchild(MakeCPRTreeNode(tntNone,"FUNCTIONS"));
    ag::tree<CPRTreeNode*>* currparent=(parent)?parent:funcparent;
    ag::tree<CPRTreeNode*>* tp;
    ag::list<CPRTextDataType>::member lm;
    CPRTreeNode *n;
    CPRTextDataType *dt=new CPRTextDataType;
    int k=0;
    char *str1,*str2,*str3,*str4;
    int* q;
    int iSz;
    int iSz2;
    bool outside=false;
    ag::list<CPRTokenInfo>*pTokens;
    CPROutsideHeader* coh;
    if (pTok!=NULL)
        pTokens=pTok;
    else
        pTokens=&aTokens;
    bool newnode;
    ag::tree<CPRTreeNode*>* pfc;

    int iCommandsIndexOut=-1;

    for(ag::list<CPRTokenInfo>::member p=pTokens->head;(p!=NULL)&&(p->data.petCurrType!=petEOF);p=p->next)
    {
        switch (state)
        {
            case 0://new expression
                if (debugmode) aTree->drawtree_con(&std::cout);// DELETE
                if (debugmode) std::cout<<"(s0) start: "<<p->data.sCurrText<<", "<<p->data.petCurrType<<"\n";
                if (p->data.sCurrText[0]==';'){} else
                if ((p->data.sCurrText[0]=='}')||(iCommandsIndexOut==0))
                {
                    bool bCmdIndexOut=(iCommandsIndexOut==0);
                    iCommandsIndexOut=-1;
                    if (currparent->data->tntType==tntIFTrue)
                    {
                        if (!bCmdIndexOut)
                            p=p->next;
                        if (strcmp(p->data.sCurrText,"else")==0)
                        {
                            currparent=currparent->parent;
                            for(ag::list<ag::tree<CPRTreeNode*>*>::member i=currparent->childs.head;i!=NULL;i=i->next)
                            {
                                if (i->data->data->tntType==tntIFFalse)
                                {
                                    currparent=i->data;
                                    break;
                                }
                            }
                            p=p->next;
                            if (p->data.sCurrText[0]!='{')
                            {
                                iCommandsIndexOut=2;
                                p=p->prev;
                            }
                        }else
                        {
                            currparent=currparent->parent->parent;
                            p=p->prev;
                        }
                    }else
                    {
                        if (currparent->data->tntType==tntForLoop)
                        {
                            ag::list<CPRTokenInfo>* aTo=new ag::list<CPRTokenInfo>;
                            std::string rs1=currparent->data->text3;
                            rs1+=';';
                            ParseIt(aTo,(char*)rs1.c_str());
                            for(ag::list<CPRTokenInfo>::member pa=aTo->head;pa!=NULL;pa=pa->next)
                                if (debugmode) std::cout << pa->data.sCurrText << ": " << pa->data.petCurrType << "; ";
                            if (debugmode) std::cout<<"\n";
                            BuildTree(workpath,aTo,(char*)rs1.c_str(),currparent);
                            if (bCmdIndexOut)
                                p=p->prev;
                            currparent=currparent->parent;
                        }else
                        if (currparent->data->tntType==tntDoWhileLoop)
                        {
                            if (p->data.sCurrText[0]=='}') p=p->next;
                            if (strcmp(p->data.sCurrText,"while")!=0)
                                throw "There is no \"while\" in Do-While loop";
                            p=p->next;
                            str1=ReadToSymbol(p,';',false);
                            currparent->data->r1=MakePostfixFromInfix(str1);
                            currparent=currparent->parent;
                        }else
                        if (currparent->data->tntType==tntIFFalse)
                        {
                            currparent=currparent->parent->parent;
                            if (bCmdIndexOut)
                                p=p->prev;
                        }else
                        {
                            if (bCmdIndexOut)
                                p=p->prev;
                            currparent=currparent->parent;
                            if (debugmode) std::cout<<"(s0): level up\n";
                        }
                    }
                }else
                if (strcmp(p->data.sCurrText,"if")==0)
                {
                    p=p->next;
                    int brackets=0;
                    std::string ex;
                    do
                    {
                        ex+=p->data.sCurrText;
                        brackets=(p->data.sCurrText[0]=='(')?brackets+1:brackets;
                        brackets=(p->data.sCurrText[0]==')')?brackets-1:brackets;
                        p=p->next;
                    }while ((brackets>0)&&(p->data.petCurrType!=petEOF));
                    str4=new char[ex.size()+1];
                    strcpy(str4,ex.c_str());
                    str4[ex.size()]=0;
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntIF,str4));
                    tp->data->r1=MakePostfixFromInfix(str4);
                    currparent=tp;
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntIFFalse,"False branch"));
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntIFTrue,"True branch"));
                    currparent=tp;
                    if (debugmode) std::cout<<"(s0) IF "<<ex<<", TRUE level created && FALSE level created && TRUE level down\n";
                    if (p->data.sCurrText[0]!='{')
                    {
                        iCommandsIndexOut=2;
                        p=p->prev;
                    }
                }else
                if (strcmp(p->data.sCurrText,"while")==0)
                {
                    p=p->next;
                    int brackets=0;
                    std::string ex;
                    do
                    {
                        ex+=p->data.sCurrText;
                        brackets=(p->data.sCurrText[0]=='(')?brackets+1:brackets;
                        brackets=(p->data.sCurrText[0]==')')?brackets-1:brackets;
                        p=p->next;
                    }while ((brackets>0)&&(p->data.petCurrType!=petEOF));
                    str4=new char[ex.size()+1];
                    strcpy(str4,ex.c_str());
                    str4[ex.size()]=0;
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntWhileLoop,str4));
                    tp->data->r1=MakePostfixFromInfix(str4);
                    currparent=tp;
                    if (p->data.sCurrText[0]!='{')
                    {
                        iCommandsIndexOut=2;
                        p=p->prev;
                    }
                }else
                if (strcmp(p->data.sCurrText,"for")==0)
                {
                    p=p->next;
                    if (p->data.sCurrText[0]!='(')
                        throw "'(' expected";
                    p=p->next;
                    str1=ReadToSymbol(p,';',true);
                    p=p->next;
                    str2=ReadToSymbol(p,';',false);
                    p=p->next;
                    str3=ReadToSymbol(p,')',false);

                    ag::list<CPRTokenInfo>* aTo=new ag::list<CPRTokenInfo>;
                    std::string rs1=str1;
                    rs1+=';';
                    ParseIt(aTo,(char*)rs1.c_str());
                    for(ag::list<CPRTokenInfo>::member pa=aTo->head;pa!=NULL;pa=pa->next)
                        if (debugmode) std::cout << pa->data.sCurrText << ": " << pa->data.petCurrType << "; ";
                    if (debugmode) std::cout<<"\n";
                    BuildTree(workpath,aTo,(char*)rs1.c_str(),currparent);
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntForLoop,str1,str2,str3));

                    tp->data->r2=MakePostfixFromInfix(str2);
                    tp->data->r3=MakePostfixFromInfix(str3);
                    currparent=tp;
                    p=p->next;
                    if (p->data.sCurrText[0]!='{')
                    {
                        iCommandsIndexOut=2;
                        p=p->prev;
                    }
                }else
                if (strcmp(p->data.sCurrText,"do")==0)
                {
                    p=p->next;
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntDoWhileLoop));
                    currparent=tp;
                    if (p->data.sCurrText[0]!='{')
                    {
                        iCommandsIndexOut=2;
                        p=p->prev;
                    }
                }else
                if (strcmp(p->data.sCurrText,"pragma")==0)
                {
                    str1=ReadToEOLN(&p, sftext);
                    if (debugmode) std::cout<<"(s0): directive: "<<str1<<"\n";
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntDirective,str1));
                }else
                if (strcmp(p->data.sCurrText,"outside")==0)
                {
                    outside=true;
                    coh=new CPROutsideHeader;
                    p=p->next;
                    char* x=ReadIdent(&p,sftext);
                    while ((p!=NULL)&&(x[0]!=':'))
                    {
                        if(strcmp(x,"cdecl")==0)
                        {
                            coh->cc=occCDecl;
                        }

                        p=p->next;
                        x=ReadIdent(&p,sftext);
                    }
                    p=p->next;
                }else
				/*if (p->data.sCurrText[0]=='@')
				{
					p=p->next;
					str1=ReadIdent(&p);
					tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntVarOutput,str1));
				}*/
                if (p->data.sCurrText[0]=='{')
                {
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntNone));
                    currparent=tp;
                    if (debugmode) std::cout<<"(s0): new level created && level down\n";
                }else
                if (IsTypename(p)) // it is typename
                {
                    str1=ReadTypename(p);
                    state=1;
                    if (debugmode) std::cout<<"(s0): '"<<str1<<"' typename detected\n";
                }else
                if (strcmp(p->data.sCurrText,"return")==0)
                {
                    p=p->next;
                    str1=ReadToSymbol(p,';',false);
                    if (debugmode) std::cout<<"(s0): '"<<str1<<"' return expression\n";
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntReturn,str1));
                    tp->data->r1=MakePostfixFromInfix(str1);
                }else
                {
                    str1=ReadToSymbol(p,';',false);
                    if (debugmode) std::cout<<"(s0): '"<<str1<<"' expression\n";
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntExpression,str1));
                    tp->data->r1=MakePostfixFromInfix(str1);
                };
                if (iCommandsIndexOut>-1)
                  iCommandsIndexOut--;
            break;
            case 1://<typename>_
                if (debugmode) std::cout<<"(s1) start\n";
                str2=ReadIdent(&p, sftext);
                state=2;
                if (debugmode) std::cout<<"(s1): '"<<str2<<"' ident for typename '"<<str1<<"'\n";
            break;

            case 2://<typename>_<ident>_
                if (debugmode) std::cout<<"(s2) start\n";
                if (strcmp(p->data.sCurrText,"(")==0)
                {
                    // function
                    if (debugmode) std::cout<<"(s2): '"<<str1<<" "<<str2<<"()' is function\n";
                    state=4;
                }else
                {
                    // variable
                    if (debugmode) std::cout<<"(s2): '"<<str1<<" "<<str2<<"' is variable\n";
                    p=p->prev;
                    state=3;
                }
            break;

            case 3://<typename>_<ident>=, <typename>_<ident>;
                if (debugmode) std::cout<<"(s3) start\n";
                if (strcmp(p->data.sCurrText,"=")==0)
                {
                    p=p->next;
                    str3=ReadToSymbol(p,';',false);
                    if (debugmode) std::cout<<"(s3): init '"<<str2<<"' variable with '"<<str3<<"'\n";
//                    CPRTreeNode* C=MakeCPRTreeNode(tntDeclareVar,NULL,NULL,NULL);
//                    tp=new ag::tree<CPRTreeNode*>(currparent,C);
//                    FillCPRTreeNode(C,tntDeclareVar,str1,str2,str3);
                    CPRTreeNode* C=MakeCPRTreeNode(tntDeclareVar,str1,str2,str3);
                    tp=new ag::tree<CPRTreeNode*>(currparent,C);
                }else
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntDeclareVar,str1,str2,NULL));
                state=0;
            break;

            case 4://<typename>_<ident>(
                if (debugmode) std::cout<<"(s4) start\n";
if (debugmode) aTree->drawtree_con(&std::cout);
                pfc=FindText2InTree(currparent,str2);
                newnode=(pfc==NULL);
                if (!newnode)
                {
//                    n=pfc->data;
//                    n->tntType=tntFunction;
                    currparent->delchild(pfc);
                }
if (debugmode) aTree->drawtree_con(&std::cout);
                    n=MakeCPRTreeNode(tntFunction,str1,str2);

                     n->r1=new ag::list<CPRTextDataType>;
                    while ((strcmp(p->data.sCurrText,")")!=0)&&(p!=NULL))
                    {
                        if (!IsTypename(p))
                        {
                            if ((strcmp(p->data.sCurrText,".")==0)&&(strcmp(p->next->data.sCurrText,".")==0)&&(strcmp(p->next->next->data.sCurrText,".")==0))
                            {
                                lm=((ag::list<CPRTextDataType>*)(n->r1))->add_tail(*dt);
                                lm->data.str1="...";
                                lm->data.str2="...";
                                lm->data.str3=NULL;
                                p=p->next->next->next;
                                continue;
                            } else
                            {
                                if (debugmode) std::cout<<"(s4) ERROR "<<p->data.sCurrText<<" is not typename\n";
                                state=0;
                                break;
                            }
                        }
                        lm=((ag::list<CPRTextDataType>*)(n->r1))->add_tail(*dt);
                        str1=ReadTypename(p);
                        lm->data.str1=str1;
                        p=p->next;
                        str2=ReadIdent(&p, sftext);
                        lm->data.str2=str2;
                        p=p->next;
                        k=0;

                        k+=(strcmp(p->data.sCurrText,"[")==0)?1:0;
                        k+=(strcmp(p->data.sCurrText,"]")==0)?-1:0;
                        //p=p->next;

                        if (k>0)
                        {
                            str3=new char[1];
                            str3[0]=0;
                        }else
                        str3=NULL;

                        if(k==1)strcat(str3,"[");
                        if(k==-1)strcat(str3,"]");
                        if (k>0) p=p->next;
                        while ((k>0)&&(p!=NULL))
                        {
                            k+=(strcmp(p->data.sCurrText,"[")==0)?1:0;
                            if(strcmp(p->data.sCurrText,"[")==0)strcat(str3,"[");
                            k+=(strcmp(p->data.sCurrText,"]")==0)?-1:0;
                            if(strcmp(p->data.sCurrText,"]")==0)strcat(str3,"]");
                            if(k>0)
                                strcat(str3,p->data.sCurrText);
                            p=p->next;
                        };
                        lm->data.str3=str3;
                        if (strcmp(p->data.sCurrText,",")==0)
                            p=p->next;
                        //if (debugmode) std::cout<<"(s4): function parameter '"<<str1<<" "<<str2<<"'\n";
                    }
                    for(ag::list<CPRTextDataType>::member I=(*((ag::list<CPRTextDataType>*)(n->r1))).head;I!=NULL;I=I->next)
                    {
                        if (debugmode) std::cout<<"(s4): function parameter '"<<I->data.str1<<" "<<I->data.str2;
                        if(I->data.str3!=NULL)if (debugmode) std::cout<<" "<<I->data.str3;
                        if (debugmode) std::cout<<"'\n";
                    }
                    p=p->next;
                    if (strcmp(p->data.sCurrText,"{")==0)
                    {
                        tp=new ag::tree<CPRTreeNode*>(currparent,n);
                        if (debugmode) aTree->drawtree_con(&std::cout);
                        if (debugmode) std::cout<<"(s4): function start: new level. level down..\n";
                        currparent=tp;
                    }
                    if (strcmp(p->data.sCurrText,";")==0)
                    {
                        if (outside)
                        {
                            outside=false;
                            n->tntType=tntOutside;
                            n->r2=coh;
                            tp=new ag::tree<CPRTreeNode*>(currparent,n);
                            if (debugmode) aTree->drawtree_con(&std::cout);
                        }
                        else
                        {
                            ((ag::list<CPRTextDataType>*)(n->r1))->delall();
                            ag::list<CPRTextDataType>::member kx;
                            for(ag::list<CPRTextDataType>::member I=(*((ag::list<CPRTextDataType>*)(n->r1))).tail;I!=NULL;)
                            {
                                kx=I->prev;
                                delete I;
                                I=kx;
                            }
                            delete n->r1;
                            delete n;
                            delete tp;
                        }
                        if (debugmode) std::cout<<"(s4): it was forward declaration of function\n";
                    }
                state=0;
            break;
        case 5:
            break;
        }
        //if (debugmode) std::cout << p->data.sCurrText << ": " << p->data.petCurrType << "; ";
    }
}

ag::tree<CPRTreeNode*>* FindText2InTree(ag::tree<CPRTreeNode*>* T,char* sText,CPRTreeNodeType tnt)
{
    ag::listmember< ag::tree<CPRTreeNode*>* >* p=(*T).childs.head;
    while (p!=NULL)
    {
		if (strcmp(p->data->data->text2,sText)==0)
		{
		    if ((tnt==tntNone)||((tnt!=tntNone)&&(p->data->data->tntType==tnt)))
                return p->data;
		}
        p=p->next;
    }
	return NULL;
}

ag::tree<CPRTreeNode*>* FindFunctionInTree(ag::tree<CPRTreeNode*>* T,char* sText)
{
    ag::listmember< ag::tree<CPRTreeNode*>* >* p=(*T).childs.head;
    while (p!=NULL)
    {
		if (strcmp(p->data->data->text2,sText)==0)
		{
		    if (p->data->data->tntType!=tntDeclareFunc)
                return p->data;
		}
        p=p->next;
    }
	return NULL;
}

ag::tree<CPRTreeNode*>* FindText1InTree(ag::tree<CPRTreeNode*>* T,char* sText,CPRTreeNodeType tnt)
{
    ag::listmember< ag::tree<CPRTreeNode*>* >* p=(*T).childs.head;
    while (p!=NULL)
    {
		if (strcmp(p->data->data->text,sText)==0)
		{
		    if ((tnt==tntNone)||((tnt!=tntNone)&&(p->data->data->tntType==tnt)))
                return p->data;
		}
        p=p->next;
    }
	return NULL;
}


void CPRApplication::ExecOutside(ag::tree<CPRTreeNode*>* T)
{
    CPROutsideHeader* coh=(CPROutsideHeader*)(T->data->r2);
    if (coh->cc==occCDecl)
    {
        int wc;
        void* buf=CreateBufferFromStackStdCall((ag::list<CPRTextDataType>*)(T->data->r1),wc);  //пихаем в буфер параметры
        void *addr = dlsym(NULL,T->data->text2);
        if (debugmode) std::cout<<"calling the "<<T->data->text2<<" ...\n";
        aStack.push(CallOutsideCDecl(addr,wc,buf,T->data->text));
        if (debugmode) std::cout<<"\ncall finished\n";
        //CallStdCall(addr,wc,buf,8); // скармливаем буфер функции
    }
}

void CPRApplication::OutVarList(ag::list<DTVar*>* Vars)
{
    if (Vars==NULL) return;
    int b=0;
    if (debugmode) std::cout<<"Vars.count="<<Vars->count()<<"\n";
    for (ag::list<DTVar*>::member m=Vars->head;m!=NULL;m=m->next)
    {
        b++;
        if (debugmode) std::cout<<"Local["<<b<<"]="<<(((DTMain*)(((DTVar*)m->data)->T))->sIdent)<<"\n";
    }
}

void CPRApplication::ExecTree(ag::tree<CPRTreeNode*>* T,ag::list<DTVar*>* ExternalVars, char* retname)
{
    try
    {
        ag::list<DTVar*> Local;
        //ag::list<CPRRestoreVarName*> ToRestore;
        if (ExternalVars!=NULL)
        {
            DTVar* q;
            rpnlist* rl;
            RPNStackElement* rse;
            DTVar* dv;
            for (ag::list<DTVar*>::member m=ExternalVars->head;m!=NULL;m=m->next)
            {
                Local.add_tail(m->data);
            }
        }
        OutVarList(&Local);
        bool was_ret=false;
        char* sq_s;
        if (T->data->tntType==tntFunction)
        {//if it's function, load arguments from stack to Local. search from the end
            int c=((ag::list<CPRTextDataType>*)(T->data->r1))->count();
            int c_k=((DTMain*)(aStack.pop()->T))->toint();
            if (c!=c_k) if (debugmode) std::cout<<"(WARNING) Params count is not equally to declaration\n";
            ag::list<CPRTextDataType>::member pm=((ag::list<CPRTextDataType>*)(T->data->r1))->head;
            OutVarList(&Local);
            ag::list<DTVar*>::member m;
            for (int i=0;i<c;i++)
            {
                OutVarList(&Local);
                m=Local.add_tail(aStack.pop());// названия
                OutVarList(&Local);
                //CPRRestoreVarName* rvn=new CPRRestoreVarName;
                //rvn->mem=m;
                //rvn->name=new char[strlen(((DTMain*)(m->data->T))->sIdent)+1];
                //strcpy(rvn->name,((DTMain*)(m->data->T))->sIdent);
                //rvn->name[strlen(rvn->name)]=0;
                //ToRestore.add_tail(rvn);
                ((DTMain*)(m->data->T))->sIdent=pm->data.str2;
                OutVarList(&Local);
                pm=pm->next;
            }
            OutVarList(&Local);
            std::string sq=T->data->text2;
            sq+="_result_";
//            std::ostringstream si;
//            si << rand();
//            sq+=si.str();
            sq_s=new char[sq.size()+1];
            strcpy(sq_s,sq.c_str());
            sq_s[sq.size()]=0;
            DTVar* r=ParseDataTypeString(T->data->text,sq_s,NULL,&Local);
            Local.add_tail(r);
        }else
        {
            sq_s=retname;
        }
        OutVarList(&Local);
        ag::listmember< ag::tree<CPRTreeNode*>* >* p=(*T).childs.head;
        while (p!=NULL)
        {
            switch(p->data->data->tntType)
            {
                case tntDeclareVar:
                    {
                        //if (p->data->data->text3)
                        if (debugmode) std::cout<<"  :  Declarating variable: "<<p->data->data->text<<", "<<p->data->data->text2;
                        if (p->data->data->text3!=NULL)
                        {
                            if (debugmode) std::cout<<" = "<<p->data->data->text3;
                        }
                        if (debugmode) std::cout<<"; regular variable\n";
                        try
                        {
                            std::string rpnstr;
                            /*rpnstr=p->data->data->text2;
                            rpnstr+="=";*/
                            rpnstr+=(p->data->data->text3)?p->data->data->text3:"";
                            DTVar* dtv=ParseDataTypeString(p->data->data->text,p->data->data->text2,//p->data->data->text3);
                                   NULL, NULL);
                            Local.add_tail(dtv);
                            rpnlist* rls;
                            rls=MakePostfixFromInfix((char*)rpnstr.c_str());
                            RPNStackElement* rlsel=new RPNStackElement;
                            rlsel->d=p->data->data->text2;
                            rlsel->tp=rsetStr;
                            rls->add_head(rlsel);
                            rlsel=new RPNStackElement;
                            rlsel->d=new char[3];
                            ((char*)rlsel->d)[0]='=';
                            ((char*)rlsel->d)[1]=' ';
                            ((char*)rlsel->d)[2]=0;
                            rlsel->tp=rsetAct;
                            rls->add_tail(rlsel);
                            OutVarList(&Local);
                            CalculateRPN(rls,&Local);
                            OutVarList(&Local);
                            if (debugmode) std::cout<<"variable was added to Local\n";
                            if (debugmode) std::cout<<((DTMain*)(dtv->T))->DTFullName()<<"\n";
                        }catch(char* k)
                        {
                            if (debugmode) std::cout<<"(Error): "<<k<<"\n";
                        }
                        catch(const char* k)
                        {
                            if (debugmode) std::cout<<"(Error): "<<k<<"\n";
                        };
                        //((DTMain*)(dtv->T))->;
                        OutVarList(&Local);
                        break;
                    }
                case tntDeclareFunc:
                    {
                        break;
                    }
                case tntFunction:
                    {
                        break;
                    }
                case tntIF:
                    {
                        ag::stack<DTVar*>* g= CalculateRPN((rpnlist*)p->data->data->r1, &Local);
                        DTMain* if_ex=((DTMain*)(g->pop()->T));
                        if (if_ex->typeoftype()!=1)
                        {
                            char* qerr=new char[strlen("Result of expression at \"if\" must be integer type!")+1];
                            strcpy(qerr,"Result of expression at \"if\" must be integer type!");
                            qerr[strlen("Result of expression at \"if\" must be integer type!")]=0;

                            throw qerr;
                        }
                        CPRTreeNodeType r;
                        if (((DTIntegerTypes*)(if_ex))->toint()) r=tntIFTrue; else r=tntIFFalse;
                        ag::tree<CPRTreeNode*>* call;
                        for(ag::list<ag::tree<CPRTreeNode*>*>::member i=p->data->childs.head;i!=NULL;i=i->next)
                        {
                            if (i->data->data->tntType==r)
                            {
                                call=i->data;
                                break;
                            }
                        }
                        ExecTree(call,&Local,sq_s);
                        break;
                    }
                case tntWhileLoop:
                    {
                        bool bOk;
                        do
                        {
                            ag::stack<DTVar*>* g= CalculateRPN((rpnlist*)p->data->data->r1, &Local);
                            DTMain* if_ex=((DTMain*)(g->pop()->T));
                            if (if_ex->typeoftype()!=1)
                            {
                                char* qerr=new char[strlen("Result of expression at \"while\" must be integer type!")+1];
                                strcpy(qerr,"Result of expression at \"while\" must be integer type!");
                                qerr[strlen("Result of expression at \"while\" must be integer type!")]=0;

                                throw qerr;
                            }
                            bOk=(((DTIntegerTypes*)(if_ex))->toint());
                            if (bOk)
                                ExecTree(p->data,&Local,sq_s);
                        }while (bOk);
                        break;
                    }
                case tntDoWhileLoop:
                    {
                        bool bOk;
                        do
                        {
                            ExecTree(p->data,&Local,sq_s);
                            ag::stack<DTVar*>* g= CalculateRPN((rpnlist*)p->data->data->r1, &Local);
                            DTMain* if_ex=((DTMain*)(g->pop()->T));
                            if (if_ex->typeoftype()!=1)
                            {
                                char* qerr=new char[strlen("Result of expression at \"while\" must be integer type!")+1];
                                strcpy(qerr,"Result of expression at \"while\" must be integer type!");
                                qerr[strlen("Result of expression at \"while\" must be integer type!")]=0;

                                throw qerr;
                            }
                            bOk=(((DTIntegerTypes*)(if_ex))->toint());
                        }while (bOk);
                        break;
                    }
                case tntForLoop:
                    {
                        bool bOk;
                        do
                        {
                            ag::stack<DTVar*>* g= CalculateRPN((rpnlist*)p->data->data->r2, &Local);
                            DTMain* if_ex=((DTMain*)(g->pop()->T));
                            if (if_ex->typeoftype()!=1)
                            {
                                char* qerr=new char[strlen("Result of expression at \"for\" must be integer type!")+1];
                                strcpy(qerr,"Result of expression at \"for\" must be integer type!");
                                qerr[strlen("Result of expression at \"for\" must be integer type!")]=0;

                                throw qerr;
                            }
                            bOk=(((DTIntegerTypes*)(if_ex))->toint());
                            if (bOk)
                                ExecTree(p->data,&Local,sq_s);
                        }while (bOk);
                        break;
                    }
                case tntExpression:
                    {
                        CalculateRPN((rpnlist*)p->data->data->r1, &Local);
                        break;
                    }
                case tntReturn:
                    {
                        if (sq_s==NULL) throw "return is not allowed in this context";
                        ag::stack<DTVar*>* g= CalculateRPN((rpnlist*)p->data->data->r1, &Local);
                        DTVar* ret_var=FindVariable(sq_s,&Local);
                        DTMain* qw=(DTMain*)(g->pop()->T);
                        DTMain* er=(DTMain*)(ret_var->T);
                        CalculateAssignation(er,qw,&Local);
                        aStack.push(ret_var);
                        //if ((DTMain*)(ret_var->T)->typeoftype()==)
                        return;
                        break;
                    }
                case tntDirective:
                    {
                        DTVar* g;
                        if (debugmode) std::cout<<"#"<<p->data->data->text<<"\n";
                        CPRParser* pd=new CPRParser(p->data->data->text);
                        pd->Next();
                        if (strcmp(pd->sCurrText,"pragma")==0)
                        {
                            pd->Next();
                            if (strcmp(pd->sCurrText,"out")==0)
                            {
                                pd->Next();
                                char* svar=pd->ReadIdent();
                                if (debugmode) std::cout<<"out("<<svar<<")\n";
                                DTVar* x=FindVariable(svar,&Local);
                                if (x!=NULL)
                                {
                                    if (debugmode) std::cout<<svar<<" = "<< (((DTMain*)(x->T))->tostring()) <<"\n";
                                }else
                                {
                                    if (debugmode) std::cout<<svar<<" not found!";
                                }
    //                            for(ag::list<DTVar*>::member i=aVars.head;i!=NULL;i=i->next)
    //                            {
    //                                g=(DTVar*)(i->data);
    //                                if (((DTMain*)(g->T))->sIdent==NULL) continue;
    //                                if (strcmp((((DTMain*)(g->T))->sIdent),svar)==0)
    //                                {
    //                                    if (debugmode) std::cout<<svar<<" = "<< (((DTMain*)(g->T))->tostring()) <<"\n";
    //                                    break;
    //                                };
    //                            };
                            };
                        };
                        break;
                    }
            };
            p=p->next;
        };
        if (T->data->tntType==tntFunction)
            aStack.push(FindVariable(sq_s,&Local));
//        for (ag::list<CPRRestoreVarName*>::member m=ToRestore.head;m!=NULL;m=m->next)
//        {
//            ((DTMain*)(m->data->mem->data->T))->sIdent=m->data->name;
//        }
    }catch(char* k)
    {
        if (debugmode) std::cout<<"(Error): "<<k<<"\n";
    }
    catch(const char* k)
    {
        if (debugmode) std::cout<<"(Error): "<<k<<"\n";
    }
}

void CPRApplication::ExecMainTree(ag::tree<CPRTreeNode*>* T)
{
	ag::tree<CPRTreeNode*>* tFuncs=T->childs[0]->data;
	ag::tree<CPRTreeNode*>* mainf=FindText2InTree(tFuncs,"main");
	if (mainf==NULL)
	{
		if (debugmode) std::cout<<"(FATAL ERROR) Function 'main' not found. Terminated.";
		return;
	}
	if (debugmode) mainf->drawtree_con(&std::cout);
	if (debugmode) std::cout<<"function parameter argc="<<argc-1<<"\n";
    DTInt* L=new DTInt("argc",argc-argnum);

    DTArray* cpargv=new DTArray("argv",sizeof(char*),argc-argnum,"char*");
    DTVar* m=DTVar::CreateNativeDTVarFromDTMain(cpargv);

    aStack.push(m);
    aStack.push(DTVar::CreateNativeDTVarFromDTMain(L));

    char* ss;
    void** tmp;
	for(int i=argnum;i<argc;i++)
	{
	    //tmp=new void*;
        ss=new char[strlen(argv[i])+1];
        strcpy(ss,argv[i]);
        ss[strlen(argv[i])]=0;
        cpargv->FillElement(i-argnum,ss);
	}
	for(int i=0;i<cpargv->count;i++)
        if (debugmode) std::cout<<"cpargv["<<i<<"]:: "<<((DTMain*)(((DTVar*)(cpargv->GetElement(i)))->T))->tostring()<<"\n";
    if (debugmode) std::cout<<"DTArray.tostring(): \""<<cpargv->tostring()<<"\"\n";

    L=new DTInt(NULL,2);
    aStack.push(DTVar::CreateNativeDTVarFromDTMain(L));
	//test
/*	int k=*((unsigned int*)(((DTUInt*)(((DTVar*)(aStack.pop()))->T))->pData));
	DTArray& a;
	a=(*aStack.pop())->T;
	for(int i=0;i<k;i++)
	{
	    if (debugmode) std::cout<<i<<"\n";
	    try{
        tmp=(((DTArray*)((*).T))->GetElement(i));
        if (debugmode) std::cout<<i<<"\n";
	    if (debugmode) std::cout<<"pop: cpargv["<<i<<"]:: "<<tmp<<"\n";
	    }
	    catch (...){
	        if (debugmode) std::cout<<"(INTERNAL ERROR) 1\n";
	    }
	}
*/
    if (debugmode) std::cout<<"\n\n";
    ExecTree(mainf);
    //recoursively run all childs
    ;
}
