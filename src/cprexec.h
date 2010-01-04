#ifndef CPR_EXEC_INCLUDED
#define CPR_EXEC_INCLUDED

enum CPROutsideCallConvention //соглашения вызова
        {
            occNone=0,
            occCDecl=1
        };

enum BVarType {vtNone=0, vtPtr, vtInt};

struct CPROutsideHeader
{
    CPROutsideCallConvention cc;
};

DTVar* CallOutsideCDecl(void* Addr, int WordCount, void* VarBuffer, char* rettype);

#endif // CPR_EXEC_INCLUDED
