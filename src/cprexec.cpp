#include "cprtypes.h"
#include "cprexec.h"


// спасибо ivanrt (habrahabr.ru) за идею
extern "C" {
    int CallCDecl(void*f, int sz, void*args);
}

typedef int   (*CallCDeclIntFunc)  (void*,int,void*);
typedef float (*CallCDeclFloatFunc)(void*,int,void*);
typedef double (*CallCDeclDoubleFunc)(void*,int,void*);
typedef void* (*CallCDeclPointerFunc)(void*,int,void*);

CallCDeclIntFunc   CallCDecl_Int   = (CallCDeclIntFunc)   CallCDecl;
CallCDeclFloatFunc CallCDecl_Float = (CallCDeclFloatFunc) CallCDecl;
CallCDeclDoubleFunc CallCDecl_Double = (CallCDeclDoubleFunc) CallCDecl;
CallCDeclPointerFunc CallCDecl_Pointer = (CallCDeclPointerFunc) CallCDecl;

void* CPRApplication::CreateBufferFromStackStdCall(ag::list<CPRTextDataType>* params,int& WordCount)
{
    int count=((DTMain*)(aStack.pop()->T))->toint();//params->count();
    ag::stack<DTVar*> tmpStack;
    ag::stack<DTVar*> lStack;
    ag::stack<DTVar*> lStack2;
    DTVar* x;
    DTMain* y;
    for(int i=0;i<count;i++)
    {
        x=aStack.pop();
        lStack.push(x);
        lStack2.push(x);
        if (i>=params->count())
        {
            CPRTextDataType td;
            td.str1="...";
            td.str2="...";
            td.str3="...";
            params->add_tail(td);
        }
    }
    ag::list<CPRTextDataType>::member pm;
    DTVar*v,*dv;
    int size=0;
    int point3c=0;
    pm=params->tail;
    bool checktype=true;
    for(int i=0;i<count;i++)
    {
        if (checktype) if (strcmp(pm->data.str1,"...")==0) checktype=false;
        if (checktype)
        {
            lStack2.pop();
            dv=ParseDataTypeString(pm->data.str1,pm->data.str2,NULL,NULL);
            y=(DTMain*)(dv->T);
        }else
        {
            x=lStack2.pop();
            y=(DTMain*)(x->T);
            if (strcmp(y->DTName(),"float")==0)
              x=ParseDataTypeString("double",pm->data.str2,NULL,NULL);
            y=(DTMain*)(x->T);
            point3c++;
        }
        if (y->sizeoftype()<=4)
            size+=4;
        if (y->sizeoftype()>4)
        {
            if ((y->sizeoftype())%4==0)
            {
                size+=y->sizeoftype();
            }else
            {
                size+=(y->sizeoftype()/4)*4+4;
            }
        }
        if (checktype)
        {
            delete (DTMain*)(dv->T);
        }
        pm=pm->prev;
    }

    void* buf=new char[size];
    WordCount=size/4;
    int offset=0;
    pm=params->tail;
    checktype=true;
    for(int i=0;i<count;i++)
    {
        checktype=true;
        if (strcmp(pm->data.str1,"...")==0) checktype=false;
        v=lStack.pop();
        if (checktype)
        {
            dv=ParseDataTypeString(pm->data.str1,pm->data.str2,NULL,NULL);
        }else
        {
            dv=v;
        }
        switch(((DTMain*)(dv->T))->typeoftype())
        {
            /*case 4: {
                CalculateAssignation((DTMain*)(dv->T),(DTMain*)(v->T),NULL);
                if (((DTMain*)(dv->T))->typeoftype()!=3)
                    throw "Uncompatible types!";
                memcpy(buf+offset,((DTMain*)(dv->T))->pData,sizeof(void*));
                break;
            }*/
            case 4: { //array->pointer 4 bytes
                if ((((DTMain*)(v->T))->typeoftype()!=3)&&(((DTMain*)(v->T))->typeoftype()!=4))
                    throw "Uncompatible types!";
                DTVar* lnns = ParseDataTypeString(((DTArray*)(dv->T))->type_one,NULL,NULL,NULL);
                if(lnns->dtet==dtetNative)
                {
                    (*(void**)((char*)buf+offset))=((DTMain*)(dv->T))->pData;
                }else
                if(lnns->dtet==dtetPointer)
                {
                    (*(void**)((char*)buf+offset))=*(void**)(((DTMain*)(dv->T))->pData);
                }
                break;
            }
            case 3: { //pointer, 4 bytes
                if ((((DTMain*)(v->T))->typeoftype()!=3)&&(((DTMain*)(v->T))->typeoftype()!=4))
                    throw "Uncompatible types!";
                /*if ()
                {
                    *(void**)(buf+offset)=new char[((DTArray*)(dv->T))->sizeoftype()+1];
                    memset(*(void**)(buf+offset),0,((DTArray*)(dv->T))->sizeoftype()+1);
                    memcpy(*(void**)(buf+offset),((DTArray*)(dv->T))->pData,((DTArray*)(dv->T))->sizeoftype());
                }else*/
                if (checktype) CalculateAssignation((DTMain*)(dv->T),(DTMain*)(v->T),NULL);
                //memcpy(buf+offset,,sizeof(void*));
                *(void**)((char*)buf+offset)=((DTMain*)(dv->T))->pData;
                break;
            }
            case 1: { //Integer types
                    if (checktype) CalculateAssignation((DTMain*)(dv->T),(DTMain*)(v->T),NULL);
                    if ((strcmp(((DTMain*)(dv->T))->DTName(),"signed int")==0)||(strcmp(((DTMain*)(dv->T))->DTName(),"unsigned int")==0))
                    {
                        *(int*)((char*)buf+offset)=*((int*)(((DTMain*)(dv->T))->pData));
                    }
                    break;
                }
            case 2: { //Float types
                    if (checktype) CalculateAssignation((DTMain*)(dv->T),(DTMain*)(v->T),NULL);
                    if (((strcmp(((DTMain*)(dv->T))->DTName(),"double")==0)||(!checktype)))
                    {
                        if (!checktype)
                        {
                            dv=ParseDataTypeString("double",pm->data.str2,NULL,NULL);
                            CalculateAssignation((DTMain*)(dv->T),(DTMain*)(v->T),NULL);
                        }
                        double x=*((double*)(((DTMain*)(dv->T))->pData));
                        memcpy((char*)buf+offset+4,&x,4);
                        memcpy((char*)buf+offset,(char*)&x+4,4);
                    }else
                    if ((strcmp(((DTMain*)(dv->T))->DTName(),"float")==0))//||(checktype))
                    {
                        //float* x=new float;
                        *(float*)((char*)buf+offset)=*((float*)(((DTMain*)(dv->T))->pData));
                        //memcpy(buf+offset,x,sizeof(float));
                    }
                    break;
                }
        }
        offset+=((DTMain*)(dv->T))->sizeoftype();
        if (pm->prev!=NULL)
            pm=pm->prev;
    }
    return buf;
}

DTVar* CallOutsideCDecl(void* Addr, int WordCount, void* VarBuffer, char* rettype)
{
    DTVar* ret=ParseDataTypeString(rettype,NULL,NULL,NULL);
    DTMain* b;
    switch(((DTMain*)(ret->T))->typeoftype())
    {
        case 3: { //pointer
                void* k=CallCDecl_Pointer(Addr,WordCount,VarBuffer);
                b=new DTPtr(NULL,NULL,k);
                break;
            }
        case 1: {//integer
                int k=CallCDecl_Int(Addr,WordCount,VarBuffer);
                if (strcmp(rettype,"unsigned int")==0)
                    b=new DTUInt(NULL,k);
                else
                    b=new DTInt(NULL,k);
                break;
            }
        case 2: {//double
                if (strcmp(rettype,"float")==0)
                {
                    float k=CallCDecl_Float(Addr,WordCount,VarBuffer);
                    b=new DTFloat(NULL,k);
                }
                if (strcmp(rettype,"double")==0)
                {
                    double k=CallCDecl_Double(Addr,WordCount,VarBuffer);
                    b=new DTDouble(NULL,k);
                }
                break;
            }
    }
    CalculateAssignation((DTMain*)ret->T,b,NULL);

    return ret;
}
