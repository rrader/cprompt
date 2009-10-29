#include <stdio.h>
#include <stdlib.h>
#include <new>

#include "lists.h"
#include "stringlist.h"
#include "cprapp.h"
#include "cprtypes.h"

extern int argc;
extern char** argv;

void CPRApplication::SetText(char* sText)
{
    iSize=strlen(sText);
    sPText=new char[iSize];
    strcpy(sPText,sText);
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
    sFilePath=new char[tmp];
    strcpy(sFilePath,sName);
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

void CPRApplication::ParseIt(ag::list<CPRTokenInfo>* pTok,char* sText)
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
    CPRTokenInfo i;
    ag::list<CPRTokenInfo>*pTokens;
    if (pTok!=NULL)
        pTokens=pTok;
    else
        pTokens=&aTokens;

    while (cParser.NowType()!=petEOF)
    {
        cParser.Next();
        i=*(cParser.GetInfo());
        pTokens->add_tail(i);
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
            };
        };
    }
    for(ag::list<DTVar*>::member i=aVars.tail;i!=NULL;i=i->prev)
    {
        s=((DTMain*)(((DTVar*)i->data)->T))->sIdent;
        if (strcmp(s,sName)==0)
        {
            return i->data;
            break;
        };
    };
    return NULL;
};

char* CPRApplication::ReadTypename(ag::list<CPRTokenInfo>::member& p)
{
    std::cout<<"CPRApplication::ReadTypename\n";
    ag::stringlist sl;
    ag::stringlist::member sl_m;
    sl_m=aTypenames->findstr(p->data.sCurrText);
    sl.delall();
    while(sl_m!=NULL)
    {
        sl.addstr(p->data.sCurrText);
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
    std::cout<<"Typename: "<<str<<"\n";
    return str;
}

bool CPRApplication::IsTypename(ag::list<CPRTokenInfo>::member p)
{
    return (aTypenames->findstr(p->data.sCurrText))!=NULL;
}

char* CPRApplication::ReadToSymbol(ag::list<CPRTokenInfo>::member& p,char _symb)
{
    ag::stringlist sl;
    do
    {
        sl.addstr(p->data.sCurrText);
        p=p->next;
    }while((p->data.sCurrText[0]!=_symb)&&(p!=NULL));
    return sl.makestrfromelements();
}

char* CPRApplication::ReadIdent(ag::list<CPRTokenInfo>::member* p, char* FText)
{
    std::cout<<"CPRApplication::ReadIdent()\n";
    CPRParser prs(FText,(*p)->data.iStartPos);
    int* E=new int;
    char* m  = prs.ReadIdent();
/*    char* m2 = ((strcmp(m,"*")==0)||(strcmp(m,"&")==0))?prs.ReadIdent():NULL;
    if (m2!=NULL) strcat(m,m2);*/
    while((*p)->data.iStartPos<prs.iPosition-1)(*p)=(*p)->next;
    (*p)=(*p)->prev;
    std::cout<<"Ident: "<<m<<"\n";
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

void CPRApplication::BuildTree(char* spath,char* sfullpath,ag::list<CPRTokenInfo>* pTok)
{
    std::cout<<"CPRApplication::BuildTree()\n";
    int state=0;
    ag::tree<CPRTreeNode*>* mainparent=this->aTree;
    ag::tree<CPRTreeNode*>* funcparent=FindText1InTree(aTree,"FUNCTIONS");
    if (funcparent==NULL)
        funcparent=this->aTree->addchild(MakeCPRTreeNode(tntNone,"FUNCTIONS"));
    ag::tree<CPRTreeNode*>* currparent=funcparent;
    ag::tree<CPRTreeNode*>* tp;
    ag::list<CPRTextDataType>::member lm;
    CPRTreeNode *n;
    CPRTextDataType *dt=new CPRTextDataType;
    int k=0;
    char *str1,*str2,*str3,*str4;
    int* q;
    int iSz;
    int iSz2;
    ag::list<CPRTokenInfo>*pTokens;
    if (pTok!=NULL)
        pTokens=pTok;
    else
        pTokens=&aTokens;

    char* path;
    if (spath==NULL)
    {
        path=new char[strlen(sFilePath)+1];
        strcpy(path,sFilePath);
        path[strlen(sFilePath)]=0;
        while((path[strlen(path)-1]!='/')&&(strlen(path)>0))
        {
            path[strlen(path)-1]=0;
        }
    }
    else
        { path=spath; }

    char* fpath;
    if (spath==NULL)
        { fpath=new char[strlen(sFilePath)+1]; strcpy(fpath,sFilePath); fpath[strlen(sFilePath)]=0; }
    else
        { fpath=sfullpath; }


    for(ag::list<CPRTokenInfo>::member p=pTokens->head;(p!=NULL)&&(p->data.petCurrType!=petEOF);p=p->next)
    {
        //l=new int;
        switch (state)
        {
            case 0://new expression
                std::cout<<"(s0) start: "<<p->data.sCurrText<<", "<<p->data.petCurrType<<"\n";
                if (p->data.sCurrText[0]=='#')
                {
					p=p->next;
					if (strcmp(p->data.sCurrText,"include")==0)
					{
					    p=p->next;
					    str1=ReadToEOLN(&p, GetFileText(fpath));
					    str2=new char[strlen(str1)+strlen(path)+2];
                        std::string q2;
					    if ((str1[0]=='"')&&(str1[strlen(str1)-1]=='"'))
					    {
					        str1++;
					        str1[strlen(str1)-1]=0;
					        q2=path;
					        if (path[strlen(path)-1]!='/')
                                q2+='/';
                            q2+=str1;
					    }else
					    if ((str1[0]=='<')&&(str1[strlen(str1)-1]=='>'))
					    {
					        char* incpath=getenv("CPROMPTINCLUDE");
					        if(incpath==NULL)
					        {
					            std::cout<<"(FATAL ERROR) Enviromnent variable CPROMPTINCLUDE is not initialized\n";
					        }
					        q2=incpath;
					        if (incpath[strlen(incpath)-1]!='/')
                                q2+='/';
                            str3=new char[q2.size()+1];
                            strcpy(str3,q2.c_str());
                            str3[q2.size()]=0;
					        str1++;
					        str1[strlen(str1)-1]=0;
                            q2+=str1;
                            str4=new char[q2.size()+1];
                            strcpy(str4,q2.c_str());
                            str4[q2.size()]=0;
                            std::cout<<"path: "<<q2<<"\n";
                            ag::list<CPRTokenInfo>* aTo=new ag::list<CPRTokenInfo>;
                            ParseIt(aTo,GetFileText(str4));
                            for(ag::list<CPRTokenInfo>::member p=aTo->head;p!=NULL;p=p->next)
                                std::cout << p->data.sCurrText << ": " << p->data.petCurrType << "; ";
                            std::cout<<"\n";
                            BuildTree(str3,str4,aTo);
					    }else
					    {
					        std::cout<<"(ERROR) Erroneous include\n";
					    }
					}else
					{
                        str1=ReadToEOLN(&p, GetFileText(fpath));
                        std::cout<<"(s0): directive: "<<str1<<"\n";
                        tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntDirective,str1));
					}
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
                    std::cout<<"(s0): new level created && level down\n";
                }else
                if (p->data.sCurrText[0]=='}')
                {
                    currparent=currparent->parent;
                    std::cout<<"(s0): level up\n";
                }else
                if (IsTypename(p)) // it is typename
                {
                    str1=ReadTypename(p);
                    state=1;
                    std::cout<<"(s0): '"<<str1<<"' typename detected\n";
                }else
                if (strcmp(p->data.sCurrText,"return")==0)
                {
                    p=p->next;
                    str1=ReadToSymbol(p,';');
                    std::cout<<"(s0): '"<<str1<<"' return expression\n";
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntReturn,str1));
                    tp->data->r1=MakePostfixFromInfix(str1);
                }else
                {
                    str1=ReadToSymbol(p,';');
                    std::cout<<"(s0): '"<<str1<<"' expression\n";
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntExpression,str1));
                    tp->data->r1=MakePostfixFromInfix(str1);
                };
            break;

            case 1://<typename>_
                std::cout<<"(s1) start\n";
                str2=ReadIdent(&p, GetFileText(fpath));
                state=2;
                std::cout<<"(s1): '"<<str2<<"' ident for typename '"<<str1<<"'\n";
            break;

            case 2://<typename>_<ident>_
                std::cout<<"(s2) start\n";
                if (strcmp(p->data.sCurrText,"(")==0)
                {
                    // function
                    std::cout<<"(s2): '"<<str1<<" "<<str2<<"()' is function\n";
                    state=4;
                }else
                {
                    // variable
                    std::cout<<"(s2): '"<<str1<<" "<<str2<<"' is variable\n";
                    p=p->prev;
                    state=3;
                }
            break;

            case 3://<typename>_<ident>=, <typename>_<ident>;
                std::cout<<"(s3) start\n";
                if (strcmp(p->data.sCurrText,"=")==0)
                {
                    p=p->next;
                    str3=ReadToSymbol(p,';');
                    std::cout<<"(s3): init '"<<str2<<"' variable with '"<<str3<<"'\n";
//                    CPRTreeNode* C=MakeCPRTreeNode(tntDeclareVar,NULL,NULL,NULL);
//                    tp=new ag::tree<CPRTreeNode*>(currparent,C);
//                    FillCPRTreeNode(C,tntDeclareVar,str1,str2,str3);
                    CPRTreeNode* C=MakeCPRTreeNode(tntDeclareVar,str1,str2,str3);
                    tp=new ag::tree<CPRTreeNode*>(currparent,C);
                }else
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntDeclareVar,str1,str2,NULL));
                q=new int;
                state=0;
            break;

            case 4://<typename>_<ident>(
                std::cout<<"(s4) start\n";
                n=MakeCPRTreeNode(tntFunction,str1,str2);
                n->r1=new ag::list<CPRTextDataType>;
                while ((strcmp(p->data.sCurrText,")")!=0)&&(p!=NULL))
                {
                    if (!IsTypename(p))
                    {
                        std::cout<<"(s4) ERROR "<<p->data.sCurrText<<" is not typename\n";
                        state=0;
                        break;
                    }
                    lm=((ag::list<CPRTextDataType>*)(n->r1))->add_tail(*dt); // Внимание! Я не помню зачем это и как оно работает! Возможны ошибки!
                    str1=ReadTypename(p);
                    lm->data.str1=str1;
                    p=p->next;
                    str2=ReadIdent(&p, GetFileText(fpath));
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
                    //std::cout<<"(s4): function parameter '"<<str1<<" "<<str2<<"'\n";
                }
                for(ag::list<CPRTextDataType>::member I=(*((ag::list<CPRTextDataType>*)(n->r1))).head;I!=NULL;I=I->next)
                {
                    std::cout<<"(s4): function parameter '"<<I->data.str1<<" "<<I->data.str2;
                    if(I->data.str3!=NULL)std::cout<<" "<<I->data.str3;
                    std::cout<<"'\n";
                }
                p=p->next;
                tp=new ag::tree<CPRTreeNode*>(currparent,n);
                if (strcmp(p->data.sCurrText,"{")==0)
                {
                    std::cout<<"(s4): function start: new level. level down..\n";
                    currparent=tp;
                }
                if (strcmp(p->data.sCurrText,";")==0)
                {
                    n->tntType=tntDeclareFunc;
                    std::cout<<"(s4): it was forward declaration of function\n";
                }
                state=0;
            break;
        case 5:
            break;
        }
        //std::cout << p->data.sCurrText << ": " << p->data.petCurrType << "; ";
    }
}

ag::tree<CPRTreeNode*>* FindText2InTree(ag::tree<CPRTreeNode*>* T,char* sText)
{
    ag::listmember< ag::tree<CPRTreeNode*>* >* p=(*T).childs.head;
    while (p!=NULL)
    {
		if (strcmp(p->data->data->text2,sText)==0)
		{
			return p->data;
		}
        p=p->next;
    }
	return NULL;
}

ag::tree<CPRTreeNode*>* FindText1InTree(ag::tree<CPRTreeNode*>* T,char* sText)
{
    ag::listmember< ag::tree<CPRTreeNode*>* >* p=(*T).childs.head;
    while (p!=NULL)
    {
		if (strcmp(p->data->data->text,sText)==0)
		{
			return p->data;
		}
        p=p->next;
    }
	return NULL;
}

void CPRApplication::ExecTree(ag::tree<CPRTreeNode*>* T)
{
    ag::list<DTVar*> Local;
    bool was_ret=false;
    char* sq_s;
    if (T->data->tntType==tntFunction)
    {//if it's function, load arguments from stack to Local. search from the end
        int c=((ag::list<CPRTextDataType>*)(T->data->r1))->count();
        ag::list<CPRTextDataType>::member pm=((ag::list<CPRTextDataType>*)(T->data->r1))->head;
        for (int i=0;i<c;i++)
        {
            ag::list<DTVar*>::member m=Local.add_tail(aStack.pop());// названия
            ((DTMain*)(m->data->T))->sIdent=pm->data.str2;
            pm=pm->next;
        }
        std::string sq=T->data->text2;
        sq+="_result";
        sq_s=new char[sq.size()+1];
        strcpy(sq_s,sq.c_str());
        sq_s[sq.size()]=0;
        DTVar* r=ParseDataTypeString(T->data->text,sq_s,NULL,&Local);
        Local.add_tail(r);
    }

    ag::listmember< ag::tree<CPRTreeNode*>* >* p=(*T).childs.head;
    while (p!=NULL)
    {
        switch(p->data->data->tntType)
        {
            case tntDeclareVar:
                {
                    //if (p->data->data->text3)
                    std::cout<<"  :  Declarating variable: "<<p->data->data->text<<", "<<p->data->data->text2;
                    if (p->data->data->text3!=NULL)
                    {
                        std::cout<<" = "<<p->data->data->text3;
                    }
                    std::cout<<"; regular variable\n";
                    try
                    {
                        std::string rpnstr;
                        /*rpnstr=p->data->data->text2;
                        rpnstr+="=";*/
                        rpnstr+=(p->data->data->text3)?p->data->data->text3:"";
                        DTVar* dtv=ParseDataTypeString(p->data->data->text,p->data->data->text2,//p->data->data->text3);
                               MakePostfixFromInfix((char*)rpnstr.c_str()), &Local);
                        Local.add_tail(dtv);
                        std::cout<<"variable was added to Local\n";
                        std::cout<<((DTMain*)(dtv->T))->DTFullName()<<"\n";
                    }catch(char* k)
                    {
                        std::cout<<"(Error): "<<k<<"\n";
                    };
                    //((DTMain*)(dtv->T))->;
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
            case tntVarOutput:
                {
                    break;
                }
            case tntExpression:
                {
                    CalculateRPN((rpnlist*)p->data->data->r1, &Local);
                    break;
                }
            case tntReturn:
                {
                    ag::stack<DTVar*>* g= CalculateRPN((rpnlist*)p->data->data->r1, &Local);
                    DTVar* ret_var=FindVariable(sq_s,&Local);
                    CalculateAssignation((DTMain*)(ret_var->T),(DTMain*)(g->pop()->T),&Local);
                    aStack.push(ret_var);
                    //if ((DTMain*)(ret_var->T)->typeoftype()==)
                    return;
                    break;
                }
            case tntDirective:
                {
                    DTVar* g;
                    std::cout<<"#"<<p->data->data->text<<"\n";
                    CPRParser* pd=new CPRParser(p->data->data->text);
                    pd->Next();
                    if (strcmp(pd->sCurrText,"TEST")==0)
                    {
                        pd->Next();
                        if (strcmp(pd->sCurrText,"out")==0)
                        {
                            pd->Next();
                            char* svar=pd->ReadIdent();
                            std::cout<<"out("<<svar<<")\n";
                            DTVar* x=FindVariable(svar,&Local);
                            if (x!=NULL)
                            {
                                std::cout<<svar<<" = "<< (((DTMain*)(x->T))->tostring()) <<"\n";
                            }else
                            {
                                std::cout<<svar<<" not found!";
                            }
//                            for(ag::list<DTVar*>::member i=aVars.head;i!=NULL;i=i->next)
//                            {
//                                g=(DTVar*)(i->data);
//                                if (((DTMain*)(g->T))->sIdent==NULL) continue;
//                                if (strcmp((((DTMain*)(g->T))->sIdent),svar)==0)
//                                {
//                                    std::cout<<svar<<" = "<< (((DTMain*)(g->T))->tostring()) <<"\n";
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
    aStack.push(FindVariable(sq_s,&Local));
}

void CPRApplication::ExecMainTree(ag::tree<CPRTreeNode*>* T)
{
	ag::tree<CPRTreeNode*>* tFuncs=T->childs[0]->data;
	ag::tree<CPRTreeNode*>* mainf=FindText2InTree(tFuncs,"main");
	if (mainf==NULL)
	{
		std::cout<<"(FATAL ERROR) Function 'main' not found. Terminated.";
		return;
	}
	mainf->drawtree_con(&std::cout);
	std::cout<<"function parameter argc="<<argc-1<<"\n";
    DTInt* L=new DTInt("argc",argc-1);

    DTArray* cpargv=new DTArray("argv",sizeof(char*),argc-1,"char*");
    DTVar* m=DTVar::CreateNativeDTVarFromDTMain(cpargv);

    aStack.push(m);
    aStack.push(DTVar::CreateNativeDTVarFromDTMain(L));

    void** ss=new void*;
    char* tmp;
	for(int i=1;i<argc;i++)
	{
        *ss=new char[strlen(argv[i])];
        strcpy((char*)*ss,argv[i]);
        cpargv->FillElement(i-1,ss);
	}
	for(int i=0;i<cpargv->count;i++)
        std::cout<<"cpargv["<<i<<"]:: "<<(char*)(cpargv->GetElement(i))<<"\n";
    std::cout<<"DTArray.tostring(): \""<<cpargv->tostring()<<"\"\n";
	//test
/*	int k=*((unsigned int*)(((DTUInt*)(((DTVar*)(aStack.pop()))->T))->pData));
	DTArray& a;
	a=(*aStack.pop())->T;
	for(int i=0;i<k;i++)
	{
	    std::cout<<i<<"\n";
	    try{
        tmp=(((DTArray*)((*).T))->GetElement(i));
        std::cout<<i<<"\n";
	    std::cout<<"pop: cpargv["<<i<<"]:: "<<tmp<<"\n";
	    }
	    catch (...){
	        std::cout<<"(INTERNAL ERROR) 1\n";
	    }
	}
*/
    std::cout<<"\n\n";
    ExecTree(mainf);
    //recoursively run all childs
    ;
}
