#ifndef CPRTYPES_INCLUDED
#define CPRTYPES_INCLUDED

#include "lists.h"
#include "rpnlist.h"

enum DTVarElementType {
    dtetNative=0,
    dtetPointer=1,
    dtetMemory=2
};

class DTMain;

struct DTVar
{
    public:
        void* T;
        DTVarElementType dtet;
        static DTVar* CreateNativeDTVarFromDTMain(DTMain* D)
        {
            DTVar* k=new DTVar;
            k->T=D;
            k->dtet=dtetNative;
            return k;
        };
        static DTVar* CreateMemory(int size)
        {
            DTVar* k=new DTVar;
            k->T=new char[size];
            k->dtet=dtetMemory;
            return k;
        };
};

class DTMain
{
    public:
        void* 	pData;
        char* 	sIdent;
        int   	size;

        DTMain();
        DTMain(char* name);
        void SetIdent(char* name);
        char* DTFullName();
        void assign(DTMain* u);

        virtual void dtmemalloc()=0;
        virtual void dtmemfree()=0;
        virtual char* DTName()=0;
        virtual int typeoftype()=0;
        virtual int sizeoftype()=0;

        virtual char* tostring()=0;
        virtual int64_t toint(){ return 0; };
        virtual double tofloat(){ return 0; };
private:
};

class DTIntegerTypes : public DTMain { public:
                                        int typeoftype() { return 1; };
                                        virtual void setint(int64_t i)=0;
                                        double tofloat(){ return toint(); };
                                     };
class DTFloatTypes   : public DTMain { public:
                                        int typeoftype() { return 2; };
                                        virtual double tofloat()=0;
                                        virtual void setfloat(double i)=0;
                                     };

class DTUInt : public DTIntegerTypes
{
    public:
        DTUInt(char* name);
        DTUInt(char* name,unsigned int d);

        void dtmemalloc();
        void dtmemfree();
        char* DTName();
        char* tostring();
        int sizeoftype(){return sizeof(int);};
        int64_t toint(){ return *((unsigned int*)pData); };
        void setint(int64_t i){ dtmemalloc(); (*((unsigned int*)pData))=i; };
    private:
};

class DTInt : public DTIntegerTypes
{
    public:
        DTInt(char* name);
        DTInt(char* name,int d);

        void dtmemalloc();
        void dtmemfree();
        char* DTName();
        char* tostring();
        int sizeoftype(){return sizeof(int);};
        int64_t toint(){ return *((int*)pData); };
        void setint(int64_t i){ dtmemalloc(); *((int*)pData)=i; };
    private:
};

class DTDouble : public DTFloatTypes
{
    public:
        DTDouble(char* name);
        DTDouble(char* name,double d);

        void dtmemalloc();
        void dtmemfree();
        char* DTName();
        char* tostring();
        int sizeoftype(){return sizeof(double);};
        double tofloat(){ return *((double*)pData); };
        void setfloat(double i){ dtmemalloc(); (*((double*)pData))=i; };
    private:
};

class DTFloat : public DTFloatTypes
{
    public:
        DTFloat(char* name);
        DTFloat(char* name,float d);

        void dtmemalloc();
        void dtmemfree();
        char* DTName();
        char* tostring();
        int sizeoftype(){return sizeof(float);};
        double tofloat(){ return *((float*)pData); };
        void setfloat(double i){ dtmemalloc(); (*((float*)pData))=i; };
    private:
};

class DTChar : public DTIntegerTypes
{
    public:
        DTChar(char* name);
        DTChar(char* name,char d);

        void dtmemalloc();
        void dtmemfree();
        char* DTName();
        char* tostring();
        int sizeoftype(){return sizeof(char);};
        int64_t toint(){ return *((char*)pData); };
        void setint(int64_t i){ dtmemalloc(); (*((int*)pData))=i; };
    private:
};

class DTPtr : public DTMain
{
   public:

        DTPtr(char* name);
        DTPtr(char* name,char* tp);
        DTPtr(char* name,char* tp,void* p);

        void dtmemalloc();
        void dtmemfree();
        char* DTName();
        char* tostring();
        int typeoftype() { return 3; };
        int sizeoftype(){return sizeof(void*);};
    private:
        char* _tp;
};

class DTArray : public DTMain
{
    public:
        DTArray(char* name,int sz,int c,char* tp);

        void dtmemalloc();
        void dtmemfree();
        char* DTName();

        bool FillElement(int n,void* buf);
        void* GetElement(int n);

        int count;
        int size_one;
        char* type_one;
        char* tostring();
        int typeoftype() { return 4; };
        int sizeoftype(){return size_one*count;};
    private:
};


typedef DTInt DTBigIntegerType;
typedef DTDouble DTBigFloatType;

DTVar* ParseDataTypeString(char* sDT,char* sName, rpnlist* data, ag::list<DTVar*>* local);//rpnlist&
DTVar* CalculateAct2op(DTMain* a,DTMain* b,char c1,char c2, ag::list<DTVar*>* local);
DTVar* CalculateAct1op(DTMain* a,char c1,char c2);
DTVar* CalculateAssignation(DTMain* a,DTMain* b, ag::list<DTVar*>* local);
#endif // CPRTYPES_INCLUDED
