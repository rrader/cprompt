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
    if (isoper1(o))
    {
        if (isoper1(n))
            return 2;
        else
            return 1;
    }
    return 0;
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
                se->d=new DTDouble(NULL,atof(s+1));
            else
                se->d=new DTInt(NULL,atoi(s+1));
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
            se=new RPNStackElement;
            se->tp=rsetStr;
            ss=new char[255];
            j=0;
            while(ss[j++]=s[j]);
            ss[s_len]=':';
            ss[s_len+1]=0;
            se->d = ss;
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
            i++;
            k=infix[i];
        };

        if ((k==',')||(k==')'))
        {
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
            ss[1]=k2;
            ss[2]=0;
            se->d=ss;
            stack.push(se);
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
ag::stack<DTVar*>* CalculateRPN(rpnlist* rpn)
{
    try{
        std::cout<<"CalculateRPN(\n";
        DTMain* v;
        DTVar* j;
        int i;
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
                if (operprior((char*)m->data->d)[0],(char*)m->data->d)[1]))
                if (r>1)
                  v=CalculateAct2op((DTMain*)(st->pop()->T),(DTMain*)(st->pop()->T),((char*)m->data->d)[0],((char*)m->data->d)[1]);
                else
                  v=CalculateAct2op(&DTInt(NULL,0),(DTMain*)(st->pop()->T),((char*)m->data->d)[0],((char*)m->data->d)[1]);
                st->push(v);
                std::cout<<"    "<<((*((DTMain*)j->T)).tostring())<<"\n";
            }
            if(m->data->tp==rsetStr)
            {
                    j=((CPRApplication*)AppV)->FindVariable((char*)m->data->d);
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
