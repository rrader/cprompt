#include "cprtypes.h"
#include "cprexec.h"


// thanks for ivanrt (habrahabr.ru)
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
    int count=params->count();
    ag::stack<DTVar*> lStack;
    for(int i=0;i<count;i++)
    {
        lStack.push(aStack.pop());
    }
    ag::list<CPRTextDataType>::member pm;
    DTVar*v,*dv;
    int size=0;
    pm=params->head;
    for(int i=0;i<count;i++)
    {
        dv=ParseDataTypeString(pm->data.str1,pm->data.str2,NULL,NULL);
        if (((DTMain*)(dv->T))->sizeoftype()<=4)
            size+=4;
        if (((DTMain*)(dv->T))->sizeoftype()>4)
        {
            if ((((DTMain*)(dv->T))->sizeoftype())%4==0)
            {
                size+=((DTMain*)(dv->T))->sizeoftype();
            }else
            {
                size+=(((DTMain*)(dv->T))->sizeoftype()/4)*4+4;
            }
        }
        delete (DTMain*)(dv->T);
        pm=pm->next;
    }

    void* buf=new char[size];
    WordCount=size/4;
    int offset=0;
    pm=params->tail;
    for(int i=0;i<count;i++)
    {
        v=lStack.pop();
        dv=ParseDataTypeString(pm->data.str1,pm->data.str2,NULL,NULL);
        switch(((DTMain*)(dv->T))->typeoftype())
        {
            /*case 4: {
                CalculateAssignation((DTMain*)(dv->T),(DTMain*)(v->T),NULL);
                if (((DTMain*)(dv->T))->typeoftype()!=3)
                    throw "Uncompatible types!";
                memcpy(buf+offset,((DTMain*)(dv->T))->pData,sizeof(void*));
                break;
            }*/
            case 3: { //pointer, 4 bytes
                if ((((DTMain*)(dv->T))->typeoftype()!=3)&&(((DTMain*)(dv->T))->typeoftype()!=4))
                    throw "Uncompatible types!";
                CalculateAssignation((DTMain*)(dv->T),(DTMain*)(v->T),NULL);
                //memcpy(buf+offset,,sizeof(void*));
                *(void**)(buf+offset)=((DTMain*)(dv->T))->pData;
                break;
            }
            case 1: { //Integer types
                    CalculateAssignation((DTMain*)(dv->T),(DTMain*)(v->T),NULL);
                    if ((strcmp(((DTMain*)(dv->T))->DTName(),"signed int")==0)||(strcmp(((DTMain*)(dv->T))->DTName(),"unsigned int")==0))
                    {
                        *(int*)(buf+offset)=*((int*)(((DTMain*)(dv->T))->pData));
                    }
                    break;
                }
            case 2: { //Float types
                    CalculateAssignation((DTMain*)(dv->T),(DTMain*)(v->T),NULL);
                    if (strcmp(((DTMain*)(dv->T))->DTName(),"double")==0)
                    {
                        double x=*((double*)(((DTMain*)(dv->T))->pData));
                        memcpy(buf+offset+4,&x,4);
                        memcpy(buf+offset,(char*)&x+4,4);
                    }
                    if (strcmp(((DTMain*)(dv->T))->DTName(),"float")==0)
                    {
                        float x=*((float*)(((DTMain*)(dv->T))->pData));
                        memcpy(buf+offset,&x,sizeof(float));
                    }
                    break;
                }
        }
        offset+=((DTMain*)(dv->T))->sizeoftype();
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
