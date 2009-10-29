#include <stdlib.h>

#include "cprtypes.h"
#include "rpnlist.h"

extern CPRApplication* AppV;



const char* sOperators=",=?:|&^!<>+-*/%~";
const char* sOperatorsLeft  = ", ||&&| ^ & ==!=< <==>> >><<+ - * / % ";

const int PRIOR_COUNT=15;

const char* sOperPrior[PRIOR_COUNT]={
                ", ", //15
                "( [ ", //1
                "++--& * ", //2
                "* / % ", //3
                "+ - ", //4
                "<<>>", //5
                "< <=> >=", //6
                "==!=", //7
                "& ", //8
                "^ ", //9
                "| ", //10
                "&&", //11
                "||", //12
                "= +=-=*=/=%=|=^=&=", //14
                "?:" //13
            };

char operprior(char o,char n);

bool isoper1(char o)
{
    char k;
    int i=0;
    while(k=sOperators[i++])
        if (k==o) return true;
    return false;
}

char isoper(char o,char n)
{
    return (operprior(o,n)==0)?((isoper1(o))?1:0):2;
}

char isoperoneof(char o,char n,char* str)
{
    char a,b;
    int i=0;
    //std::cout<<"isoperleft("<<o<<", "<<n<<", \""<<str<<"\")\n";
    while (a=str[i++],b=str[i++],a)
    {
        if ((a==o)&&(b==n)) return 1;
    };
    return 0;
}

char operprior(char o,char n)
{
    for(int i=PRIOR_COUNT-1;i>=0;i--)
    {
        if (isoperoneof(o,n,(char*)(sOperPrior[i]))==1)
        {
            return i+1;
        }
    };
    return 0;
};

//
//rpnlist* MakePostfixFromInfix2(char* infix)
//{
//    int i=0,j;
//    char k,k2;
//    int s_len=0;
//    char s[256];
//    ag::stack<RPNStackElement*> stack;
//    rpnlist* res=new rpnlist;
//    RPNStackElement* se;
//    while (k=infix[i])
//    {
//        j=i;
//        s_len=1;
//        s[0]='i';
//        bool isfloat=false;
//        while ( (((k=infix[i])=='.')&&(!isfloat))||(isdigit(k=infix[i])) )
//        {
//            i++;
//            s[s_len++]=k;
//            if (k=='.')
//            {
//                isfloat=true;
//                s[0]='f';
//            }
//        }
//        s_len--;
//        i=j+s_len-((s_len==0) ? 0 : 1);
//        s[s_len+1]=0;
//        if (s_len!=0)
//        {
//            //NUMBER
//            std::cout<<"   num: "<<s<<"\n";
//            se=new RPNStackElement;
//            se->tp=rsetNum;
//            if (isfloat)
//                se->d=new DTBigFloatType(NULL,atof(s+1));
//            else
//                se->d=new DTBigIntegerType(NULL,atoi(s+1));
//            res->add_tail(se);
//        }
//
//        j=i;
//        s_len=0;
//        bool b=false;
//        k=infix[i++];
//        while ( isalpha(k)||((isdigit(k))&&(b))||(k=='_') )
//        {
//            if (!b) b=isalpha(k);
//            s[s_len++]=k;
//            k=infix[i++];
//        }
//        i=j+s_len-((s_len==0) ? 0 : 1);
//        s[s_len]=0;
//    }
//}

rpnlist* MakePostfixFromInfix(char* infix)
{
    int i=0,j;
    char k,k2;
    char* s=new char[256];
//     res;
    char* ss;
    int s_len=0;
    ag::stack<RPNStackElement*> stack;
    rpnlist* res=new rpnlist;
    rpnlist* _res=new rpnlist;
    RPNStackElement* se;
    bool last_num=false;
    while(k=infix[i])
    {
        j=i;
        s_len=1;
        s[0]='i';
        bool isfloat=false;
        while ( (((k=infix[i])=='.')&&(!isfloat))||(isdigit(k=infix[i])) )
        {
            i++;
            s[s_len++]=k;
            if (k=='.')
            {
                isfloat=true;
                s[0]='f';
            }
        }
        s_len--;
        i=j+s_len-((s_len==0) ? 0 : 1);
        s[s_len+1]=0;
        if (s_len!=0)
        {
            last_num=true;
            se=new RPNStackElement;
            se->tp=rsetNum;
            se->d=new char[strlen(s)];
            strcpy((char*)se->d,s);
            res->add_tail(se);
            std::cout<<se->tp<<": "<<s<<"\n";

            //
            se=new RPNStackElement;
            se->tp=rsetNum;
            if (isfloat)
                se->d=new DTBigFloatType(NULL,atof(s+1));
            else
                se->d=new DTBigIntegerType(NULL,atoi(s+1));
            _res->add_tail(se);
        }

        j=i;
        s_len=0;
        bool b=false;
        k=infix[i++];
        while ( isalpha(k)||((isdigit(k))&&(b))||(k=='_') )
        {
            if (!b) b=isalpha(k);
            s[s_len++]=k;
            k=infix[i++];
        }
        i=j+s_len-((s_len==0) ? 0 : 1);
        s[s_len]=0;

        if ((infix[i+1]=='(')&&(s_len!=0))
        {
         // function
            last_num=false;
            se=new RPNStackElement;
            se->tp=rsetStr;
            ss=new char[s_len+2];
            j=0;
            while(ss[j++]=s[j]);
            ss[s_len]=':';
            ss[s_len+1]=0;
            se->d = ss;
            std::cout<<"temp "<<ss<<"\n";
            stack.push(se);
            se=new RPNStackElement;
            se->tp=rsetAct;
            ss=new char[3];
            ss="( ";
            se->d=ss;
            stack.push(se);
            i++;
            k=infix[i+1];
        }else
        if (s_len!=0)
        {
            //variable
            last_num=true;
            se=new RPNStackElement;
            se->tp=rsetStr;
            ss=new char[255];
            j=0;
            while(ss[j++]=s[j]);
            se->d = ss;
            res->add_tail(se);
            std::cout<<se->tp<<": "<<s<<"\n";

            //
            _res->add_tail(se);
            //
            //i++;         // 1111111111111111
            k=infix[i];
        };

        if ((k==',')||(k==')'))
        {
            last_num=false;
            while(!stack.empty())
            {
                se=(RPNStackElement*)stack.tail->data;
                if ((se->tp==rsetAct)&&(strcmp(((char*)(se->d)),"( ")==0))
                    break;
                se=stack.pop();
                res->add_tail(se);
                std::cout<<se->tp<<": "<<(char*)(se->d)<<"\n";
            }
            if (k==')')
            {
                while(!stack.empty())
                {
                    se=stack.pop();
                    if ((se->tp==rsetAct)&&(strcmp(((char*)(se->d)),"( ")==0))
                        break;
                    res->add_tail(se);

                    std::cout<<se->tp<<": "<<(char*)(se->d)<<"\n";
                    //
                    if (se->tp==rsetNum)
                    {
                        switch (((char*)(se->d))[0])
                        {
                            case 'i':se->d=new DTInt(NULL,atoi(((char*)(se->d))+1));
                            case 'f':se->d=new DTDouble(NULL,atof(((char*)(se->d))+1));
                        }
                    }
                    _res->add_tail(se);
                    //
                }
                if (!stack.empty())
                {
                    se=stack.tail->data;
                    if (se->tp==rsetStr)
                    {
                        se=stack.pop();
                        res->add_tail(se);
                        std::cout<<se->tp<<": "<<(char*)(se->d)<<"\n";

                        //
                        if (se->tp==rsetNum)
                        {
                            switch (((char*)(se->d))[0])
                            {
                                case 'i':se->d=new DTInt(NULL,atoi(((char*)(se->d))+1));
                                case 'f':se->d=new DTDouble(NULL,atof(((char*)(se->d))+1));
                            }
                        }
                        _res->add_tail(se);
                        //

                    }
                }
            }
        }

        if (k=='(')
        {
            last_num=false;
            se=new RPNStackElement;
            se->tp=rsetAct;
            ss=new char[3];
            ss="( ";
            se->d=ss;
            stack.push(se);
        }

        if (isoper(k,infix[i+1]))
        {
            k2=(isoper(k,infix[i+1])==1)?' ':infix[i+1];
            if (!stack.empty())
            {
                if (stack.tail->data->tp!=rsetAct) throw "(SYNTAX ERROR)";
                if(isoperoneof(k,k2,(char*)(sOperatorsLeft))==1)//left
                {
                    ss=(char*)stack.tail->data->d;
                    while(operprior(k,k2)>=operprior(ss[0],ss[1]))
                    {
                        if ((ss[0]=='(')&&(ss[1]==' ')) break;
                        se=stack.pop();
                        res->add_tail(se);
                        std::cout<<se->tp<<": "<<(char*)(se->d)<<"\n";

                        //
                        if (se->tp==rsetNum)
                        {
                            switch (((char*)(se->d))[0])
                            {
                                case 'i':se->d=new DTInt(NULL,atoi(((char*)(se->d))+1));
                                case 'f':se->d=new DTDouble(NULL,atof(((char*)(se->d))+1));
                            }
                        }
                        _res->add_tail(se);
                        //

                        if (stack.empty()) break;
                        ss=(char*)stack.tail->data->d;
                    }; // !!!!
                }else
                {
                    ss=(char*)stack.tail->data->d;
                    while(operprior(k,k2)>operprior(ss[0],ss[1]))
                    {
                        if ((ss[0]=='(')&&(ss[1]==' ')) break;
                        se=stack.pop();
                        res->add_tail(se);
                        std::cout<<se->tp<<": "<<(char*)(se->d)<<"\n";
                        //
                        if (se->tp==rsetNum)
                        {
                            switch (((char*)(se->d))[0])
                            {
                                case 'i':se->d=new DTInt(NULL,atoi(((char*)(se->d))+1));
                                case 'f':se->d=new DTDouble(NULL,atof(((char*)(se->d))+1));
                            }
                        }
                        _res->add_tail(se);
                        //
                        if (stack.empty()) break;
                        ss=(char*)stack.tail->data->d;
                    };
                }
            }
            se=new RPNStackElement;
            se->tp=rsetAct;
            ss=new char[3];
            ss[0]=k;
            ss[1]=last_num?k2:'u';
            ss[2]=0;
            se->d=ss;
            stack.push(se);
            last_num=false;
        }

        i++;
    };
    while(!stack.empty())
    {
        se=stack.pop();
        res->add_tail(se);
        std::cout<<se->tp<<": "<<(char*)(se->d)<<"\n";
        //
        if (se->tp==rsetNum)
        {
            switch (((char*)(se->d))[0])
            {
                case 'i':se->d=new DTInt(NULL,atoi(((char*)(se->d))+1));
                case 'f':se->d=new DTDouble(NULL,atof(((char*)(se->d))+1));
            }
        }
        _res->add_tail(se);
        //
    }
    std::cout<<"\n";
    return _res;
}
//6+*i
ag::stack<DTVar*>* CalculateRPN(rpnlist* rpn, ag::list<DTVar*>* local)
{
    try{
        std::cout<<"CalculateRPN(\n";
        DTMain* v;
        DTVar* j;
        int i;
        int* yu=new int;
        ag::stack<DTVar*>* st=new ag::stack<DTVar*>;
        for(rpnlist::member m=(*rpn).head;m!=NULL;m=m->next)
        {
            if(m->data->tp==rsetNum)
            {
                std::cout<<"    "<<((*((DTMain*)m->data->d)).tostring())<<"\n";;
                st->push(DTVar::CreateNativeDTVarFromDTMain((DTMain*)m->data->d));
            };
            if(m->data->tp==rsetAct)
            {
                std::cout<<"    "<<((char*)m->data->d)[0]<<((char*)m->data->d)[1]<<"\n";
                DTVar* v;
                int r=st->count();
                std::cout<<r<<";\n";
                if (((char*)m->data->d)[1]!='u')
                  v=CalculateAct2op((DTMain*)(st->pop()->T),(DTMain*)(st->pop()->T),((char*)m->data->d)[0],((char*)m->data->d)[1],
                                        local);
                else
                  v=CalculateAct1op((DTMain*)(st->pop()->T),((char*)m->data->d)[0],((char*)m->data->d)[1]);
                st->push(v);
                //std::cout<<"    "<<((*((DTMain*)j->T)).tostring())<<"\n";
            }
            if(m->data->tp==rsetStr)
            {
                if ((((char*)m->data->d)[strlen(((char*)m->data->d))-1])==':')
                {
                    //App.aTree
                    char* s=new char[strlen((char*)m->data->d)];
                    strcpy(s,(char*)m->data->d);
                    s[strlen((char*)m->data->d)-1]=0;
                    ag::tree<CPRTreeNode*>* func_t=
                        FindText2InTree((ag::tree<CPRTreeNode*>*)((CPRApplication*)AppV->aTree->childs[0]->data),s);
                    if (func_t==NULL)
                    {
                        throw "(FATAL ERROR) Function not found. Terminated.";
                    }
                    func_t->drawtree_con(&std::cout);
                    ag::list<CPRTextDataType>* prms=(ag::list<CPRTextDataType>*)(func_t->data->r1);
                    std::cout<<"function "<<(char*)m->data->d<<" have "<<prms->count()<<" parametets:\n";
                    for(ag::list<CPRTextDataType>::member m=prms->head;m!=NULL;m=m->next)
                    {
                        std::cout<<((m->data.str1!=NULL)?m->data.str1:"")<<" ";
                        std::cout<<((m->data.str2!=NULL)?m->data.str2:"")<<" ";
                        std::cout<<((m->data.str3!=NULL)?m->data.str3:"")<<". \n";
                    };
                    DTVar* m;
                    for(int i=0;i<prms->count();i++)
                    {
                        m=st->pop();
                        (CPRApplication*)(AppV->aStack).push(m);
                    }
                    ((CPRApplication*)AppV)->ExecTree(func_t);
                    st->push((((CPRApplication*)AppV)->aStack).pop());
                }else
                {
                    j=((CPRApplication*)AppV)->FindVariable((char*)m->data->d, local);
                    if (j!=NULL)
                    {
                        st->push(j);
                        std::cout<<"    "<<((*((DTMain*)j->T)).tostring())<<"\n";
                    }else
                    {
                        char* s;
                        std::string ss="unknown variable ";
                        ss+=(char*)m->data->d;
                        s=(char*)ss.c_str();
                        throw s;
                    }
                }

            }
            /*switch (((char*)(m->data->d))[0])
            {
                case 'i':
                    {
                        char* k=(char*)(m->data->d)+1;
                        std::cout<<"      int "<<atoi(k)<<"\n";
                        v=new DTInt(NULL,atoi(k));
                        break;
                    }
            }*/
        };
        std::cout<<"  );\n";
        return st;
    }catch(char* o)
    {
        std::cout<<"(ERROR) "<<o<<"\n";
    }
    catch(...)
    {
        std::cout<<"(UERROR)\n";
    }
};
