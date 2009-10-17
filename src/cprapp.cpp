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
    sPText=new char[length];
    file.read(sPText,length);
    file.close();
}

void CPRApplication::ParseIt()
{
    CPRParser cParser(sPText);
    CPRTokenInfo i;
    while (cParser.NowType()!=petEOF)
    {
        cParser.Next();
        i=*(cParser.GetInfo());
        aTokens.add_tail(i);
    }
}

DTVar* CPRApplication::FindVariable(char* sName)
{
    char* s;
    for(ag::list<DTVar*>::member i=aVars.head;i!=NULL;i=i->next)
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
    ag::stringlist sl;
    ag::stringlist::member sl_m;
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

char* CPRApplication::ReadIdent(ag::list<CPRTokenInfo>::member* p)
{
    CPRParser prs(this->sPText,(*p)->data.iStartPos);
    char* m  = prs.ReadIdent();
/*    char* m2 = ((strcmp(m,"*")==0)||(strcmp(m,"&")==0))?prs.ReadIdent():NULL;
    if (m2!=NULL) strcat(m,m2);*/
    while((*p)->data.iStartPos<prs.iPosition-1)(*p)=(*p)->next;
    (*p)=(*p)->prev;
    return m;
}

char* CPRApplication::ReadToEOLN(ag::list<CPRTokenInfo>::member* p)//!!!!!!!!!!!!!!!!!
{
    CPRParser prs(this->sPText,(*p)->data.iStartPos);
    char* m  = prs.ReadToEOLN();
/*    char* m2 = ((strcmp(m,"*")==0)||(strcmp(m,"&")==0))?prs.ReadIdent():NULL;
    if (m2!=NULL) strcat(m,m2);*/
    while((*p)->data.iStartPos<prs.iPosition-1)(*p)=(*p)->next;
    (*p)=(*p)->prev;
    return m;
}
void CPRApplication::BuildTree()
{
    int state=0;
    ag::tree<CPRTreeNode*>* mainparent=this->aTree;
    ag::tree<CPRTreeNode*>* funcparent=this->aTree->addchild(MakeCPRTreeNode(tntNone,"FUNCTIONS"));
    ag::tree<CPRTreeNode*>* currparent=funcparent;
    ag::tree<CPRTreeNode*>* tp;
    ag::list<CPRTextDataType>::member lm;
    CPRTreeNode *n;
    CPRTextDataType *dt;
    int k=0;
    char *str1,*str2,*str3;
    int iSz;
    int iSz2;
    int*l;
    for(ag::list<CPRTokenInfo>::member p=aTokens.head;p!=NULL;p=p->next)
    {
        l=new int;
        switch (state)
        {
            case 0://new expression
                if (p->data.sCurrText[0]=='#')
                {
					p=p->next;
					str1=ReadToEOLN(&p);
                    std::cout<<"(s0): directive: "<<str1<<"\n";
					tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntDirective,str1));
                }
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
                }
                if (p->data.sCurrText[0]=='}')
                {
                    currparent=currparent->parent;
                    std::cout<<"(s0): level up\n";
                }

                if (IsTypename(p)) // it is typename
                {
                    str1=ReadTypename(p);
                    state=1;
                    std::cout<<"(s0): '"<<str1<<"' typename detected\n";
                }
            break;

            case 1://<typename>_
                str2=ReadIdent(&p);
                state=2;
                std::cout<<"(s1): '"<<str2<<"' ident for typename '"<<str1<<"'\n";
            break;

            case 2://<typename>_<ident>_
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
                if (strcmp(p->data.sCurrText,"=")==0)
                {
                    p=p->next;
                    str3=ReadToSymbol(p,';');
                    std::cout<<"(s3): init '"<<str2<<"' variable with '"<<str3<<"'\n";
                    CPRTreeNode* C=MakeCPRTreeNode(tntDeclareVar,NULL,NULL,NULL);
                    tp=new ag::tree<CPRTreeNode*>(currparent,C);
                    FillCPRTreeNode(C,tntDeclareVar,str1,str2,str3);
                }else
                    tp=new ag::tree<CPRTreeNode*>(currparent,MakeCPRTreeNode(tntDeclareVar,str1,str2,NULL));
                state=0;
            break;

            case 4://<typename>_<ident>(
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
                    lm=(*((ag::list<CPRTextDataType>*)(n->r1))).add_tail(*dt);
                    str1=ReadTypename(p);
                    lm->data.str1=str1;
                    p=p->next;
                    str2=ReadIdent(&p);
                    lm->data.str2=str2;
                    p=p->next;
                    k=0;

                    k+=(strcmp(p->data.sCurrText,"[")==0)?1:0;
                    k+=(strcmp(p->data.sCurrText,"]")==0)?-1:0;
                    p=p->next;

                    if (k>0)
                    {
                        str3=new char[1];
                        str3[0]=0;
                    }else
                    str3=NULL;

                    if(k==1)strcat(str3,"[");
                    if(k==-1)strcat(str3,"]");

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
                    if (strcmp(p->data.sCurrText,",")==0) p=p->next;
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
                    std::cout<<"(s4): it was declaration of function\n";
                }
                state=0;
            break;
        }
        //std::cout << p->data.sCurrText << ": " << p->data.petCurrType << "; ";
    }
}

ag::tree<CPRTreeNode*>* FindFuncInTree(ag::tree<CPRTreeNode*>* T,char* sText)
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

void CPRApplication::ExecTree(ag::tree<CPRTreeNode*>* T)
{
    ag::list<DTVar*> Local;
    if (T->data->tntType==tntFunction)
    {//if it's function, load arguments from stack to Local. search from the end
        int c=((ag::list<CPRTextDataType>*)(T->data->r1))->count();
        for (int i=0;i<c;i++)
        {
            Local.add_tail(aStack.pop());
        }
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
                               MakePostfixFromInfix((char*)rpnstr.c_str()));
                        aVars.add_tail(dtv);
                        std::cout<<"variable was added to aVar\n";
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
                            for(ag::list<DTVar*>::member i=aVars.head;i!=NULL;i=i->next)
                            {
                                g=(DTVar*)(i->data);
                                if (((DTMain*)(g->T))->sIdent==NULL) continue;
                                if (strcmp((((DTMain*)(g->T))->sIdent),svar)==0)
                                {
                                    std::cout<<svar<<" = "<< (((DTMain*)(g->T))->tostring()) <<"\n";
                                    break;
                                };
                            };
                        };
                    };
                    break;
                }
        };
        p=p->next;
    };
}

void CPRApplication::ExecMainTree(ag::tree<CPRTreeNode*>* T)
{
	ag::tree<CPRTreeNode*>* tFuncs=T->childs[0]->data;
	ag::tree<CPRTreeNode*>* mainf=FindFuncInTree(tFuncs,"main");
	if (mainf==NULL)
	{
		std::cout<<"(FATAL ERROR) Function 'main' not found. Terminated.";
		return;
	}
	mainf->drawtree_con(&std::cout);
	std::cout<<"function parameter argc="<<argc-1<<"\n";
    DTInt* L=new DTInt("argc",argc-1);
	aStack.push(DTVar::CreateNativeDTVarFromDTMain(L));

    DTArray* cpargv=new DTArray("argv",sizeof(char*),argc-1,"char*");
    DTVar* m=DTVar::CreateNativeDTVarFromDTMain(cpargv);
    aStack.push(m);

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
