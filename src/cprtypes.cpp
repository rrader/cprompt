#include "cprtypes.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "rpnlist.h"

//class DTMain

DTMain::DTMain()
{
	if (debugmode) std::cout<<"DTMain()\n";
	sIdent=NULL;
	pData=NULL;
}

DTMain::DTMain(char* name)
{
	if (debugmode) std::cout<<"DTMain("<<name<<")\n";
	sIdent=new char[strlen(name)+1];
	strcpy(sIdent,name);
	sIdent[strlen(name)]=0;
	pData=NULL;
}

void DTMain::SetIdent(char* name)
{
    if (debugmode) std::cout<<"DTMain: SetIdent\n";
	if (sIdent!=NULL) delete[] sIdent;
	sIdent=new char[strlen(name)+1];
	sIdent[strlen(name)]=0;
	strcpy(sIdent,name);
}

char* DTMain::DTFullName()
{

	std::string a;
	char* n=DTName();
	a+=n;
	if (debugmode) std::cout<<"DTFullName():\n";
	if (debugmode) std::cout<<"DTName()="<<n<<"\n";

	if (sIdent==NULL)
	{
        if (debugmode) std::cout<<"sIdent=NULL\n";
	}
    else
	{
        if (debugmode) std::cout<<"sIdent="<<sIdent<<"\n";
	}
	//char* a=new char[strlen(n)+1+sIdLen];
	if (sIdent!=NULL)
	{
	    //strcpy(a,n);
        a+=" ";
        //strcat(a," ");
        a+=sIdent;
        //strcat(a,sIdent);
	};
	char* ar=new char[a.size()+1];
	strcpy(ar,a.c_str());
	ar[a.size()]=0;
	return ar;
}

void DTMain::assign(DTMain* u, bool Copy)
{
    if (Copy)
        memcpy(pData,u->pData,sizeoftype());
    else
    {
        size=u->size;
        pData=u->pData;
    }
}

//class DTInt

DTInt::DTInt(char* name) //without memalloc!
{
	if (debugmode) std::cout<<"DTInt("<<((name!=NULL)?name:"NULL")<<")\n";
	pData=NULL;
	if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
	}
	size=0;
	dtmemalloc();
}

DTInt::DTInt(char*name,int d)
{
	if (debugmode) std::cout<<"DTInt("<<((name!=NULL)?name:"NULL")<<", "<<d<<")\n";
	dtmemalloc();
	*((int*)pData)=d;
	if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
	}
}

void DTInt::dtmemalloc()
{
    if (debugmode) std::cout<<"DTInt: dtmemalloc\n";
    dtmemfree();
	pData=new (int);
}

void DTInt::dtmemfree()
{
    if (debugmode) std::cout<<"DTInt: dtmemfree\n";
    if (pData!=NULL)
        delete (int*)pData;
    pData=NULL;
}

char* DTInt::DTName()
{
    if (debugmode) std::cout<<"DTInt: DTName\n";
	char* rs=new char[strlen("signed int")+1];
	strcpy(rs,"signed int");
	rs[strlen("signed int")]=0;
	return rs;
}

char* DTInt::tostring()
{
    if (debugmode) std::cout<<"DTInt: tostring\n";
    char* tmp=new char[100];
    if (pData!=NULL)
        sprintf(tmp,"%d",*(int*)pData);
    else
        tmp="NULL";
	char* rs=new char[strlen(tmp)+1];
	strcpy(rs,tmp);
	rs[strlen(tmp)]=0;
//	delete[] tmp;
	return rs;
}

//class DTMarker

DTMarker::DTMarker(char* name) //without memalloc!
{
	if (debugmode) std::cout<<"DTMarker("<<((name!=NULL)?name:"NULL")<<")\n";
	pData=NULL;
	if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
	}
	size=0;
}

void DTMarker::dtmemalloc()
{
    if (debugmode) std::cout<<"DTMarker: dtmemalloc\n";
}

void DTMarker::dtmemfree()
{
    if (debugmode) std::cout<<"DTMarker: dtmemfree\n";
}

char* DTMarker::DTName()
{
    if (debugmode) std::cout<<"DTMarker: DTName\n";
	char* rs=new char[strlen("marker")+1];
	strcpy(rs,"marker");
	rs[strlen("marker")]=0;
	return rs;
}

char* DTMarker::tostring()
{
    if (debugmode) std::cout<<"DTInt: tostring\n";
	char* rs=new char[strlen("marker")+1];
	strcpy(rs,"marker");
	rs[strlen("marker")]=0;
	return rs;
}


//class DTUInt

DTUInt::DTUInt(char* name) //without memalloc!
{
	if (debugmode) std::cout<<"DTUInt("<<((name!=NULL)?name:"NULL")<<")\n";
	pData=NULL;
	size=0;
	if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
	}
    dtmemalloc();
}

DTUInt::DTUInt(char*name,unsigned int d)
{
	if (debugmode) std::cout<<"DTUInt("<<((name!=NULL)?name:"NULL")<<", "<<d<<")\n";
	dtmemalloc();
	*((unsigned int*)pData)=d;
	sIdent=new char[strlen(name)+1];
	strcpy(sIdent,name);
	sIdent[strlen(name)]=0;
}

void DTUInt::dtmemalloc()
{
    if (debugmode) std::cout<<"DTUInt::dtmemalloc\n";
    dtmemfree();
	pData=new (unsigned int);
}

void DTUInt::dtmemfree()
{
    if (debugmode) std::cout<<"DTUInt::dtmemfree()\n";
    if (pData!=NULL)
        delete (unsigned int*)pData;
	pData=NULL;
}

char* DTUInt::DTName()
{
//    if (debugmode) std::cout<<"DTInt: DTName\n";
//	char* rs=new char[strlen("signed int")];
//	strcpy(rs,"signed int");
//	return rs;
    if (debugmode) std::cout<<"DTUInt::DTName()\n";
	char* rs=new char[13];
	strcpy(rs,"unsigned int");
	rs[12]=0;
	return rs;
}

char* DTUInt::tostring()
{
    if (debugmode) std::cout<<"DTUInt::tostring()\n";
    char* tmp=new char[100];
    if (pData!=NULL)
        sprintf(tmp,"%u",(*(int*)pData));
    else
        tmp="NULL";
	char* rs=new char[strlen(tmp)+1];
	strcpy(rs,tmp);
	rs[strlen(tmp)]=0;
	return rs;
}

//class DTDouble

DTDouble::DTDouble(char* name) //without memalloc!
{
	if (debugmode) std::cout<<"DTDouble("<<((name!=NULL)?name:"NULL")<<")\n";
	pData=NULL;
	if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
	}
	size=0;
    dtmemalloc();
}

DTDouble::DTDouble(char*name,double d)
{
	if (debugmode) std::cout<<"DTDouble("<<((name!=NULL)?name:"NULL")<<", "<<d<<")\n";
	dtmemalloc();
	*((double*)pData)=d;
	if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
	}
}

void DTDouble::dtmemalloc()
{
    if (debugmode) std::cout<<"DTDouble::dtmemalloc()\n";
    dtmemfree();
	pData=new (double);
}

void DTDouble::dtmemfree()
{
    if (debugmode) std::cout<<"DTDouble::dtmemfree()\n";
    if (pData!=NULL)
        delete (double*)pData;
    pData=NULL;
}

char* DTDouble::DTName()
{
    if (debugmode) std::cout<<"DTDouble::DTName()\n";
	char* rs=new char[strlen("double")+1];
	strcpy(rs,"double");
	rs[strlen("double")]=0;
	return rs;
}

char* DTDouble::tostring()
{
    if (debugmode) std::cout<<"DTDouble::tostring()\n";
    char* tmp=new char[100];
    if (pData!=NULL)
        sprintf(tmp,"%f",*(double*)pData);
    else
        tmp="NULL";
	char* rs=new char[strlen(tmp)+1];
	strcpy(rs,tmp);
	rs[strlen(tmp)]=0;
	return rs;
}

//class DTFloat

DTFloat::DTFloat(char* name) //without memalloc!
{
	if (debugmode) std::cout<<"DTFloat("<<((name!=NULL)?name:"NULL")<<")\n";
	pData=NULL;
	if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
	}
	size=0;
    dtmemalloc();
}

DTFloat::DTFloat(char*name,float d)
{
	if (debugmode) std::cout<<"DTFloat("<<((name!=NULL)?name:"NULL")<<", "<<d<<")\n";
	dtmemalloc();
	*((float*)pData)=d;
	if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
	}
}

void DTFloat::dtmemalloc()
{
    if (debugmode) std::cout<<"DTFloat::dtmemalloc()\n";
    dtmemfree();
	pData=new (float);
}

void DTFloat::dtmemfree()
{
    if (debugmode) std::cout<<"DTFloat::dtmemfree()\n";
    if (pData!=NULL)
        delete (float*)pData;
    pData=NULL;
}

char* DTFloat::DTName()
{
    if (debugmode) std::cout<<"DTFloat::DTName()\n";
	char* rs=new char[strlen("float")+1];
	strcpy(rs,"float");
	rs[strlen("float")]=0;
	return rs;
}

char* DTFloat::tostring()
{
    if (debugmode) std::cout<<"DTFloat::tostring()\n";
    char* tmp=new char[100];
    if (pData!=NULL)
        sprintf(tmp,"%f",*(float*)pData);
    else
        tmp="NULL";
	char* rs=new char[strlen(tmp)+1];
	strcpy(rs,tmp);
	rs[strlen(tmp)]=0;
	return rs;
}

//class DTChar

DTChar::DTChar(char* name) //without memalloc!
{
	if (debugmode) std::cout<<"DTChar("<<((name!=NULL)?name:"NULL")<<")\n";
	pData=NULL;
	size=0;
	if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
	}
    dtmemalloc();
}

DTChar::DTChar(char*name,char d)
{
	if (debugmode) std::cout<<"DTChar("<<((name!=NULL)?name:"NULL")<<", "<<d<<")\n";
	dtmemalloc();
	*((char*)pData)=d;
	sIdent=new char[strlen(name)+1];
	strcpy(sIdent,name);
	sIdent[strlen(name)]=0;
}

void DTChar::dtmemalloc()
{
    if (debugmode) std::cout<<"DTChar::dtmemalloc()\n";
    dtmemfree();
	pData=new char;
}

void DTChar::dtmemfree()
{
    if (debugmode) std::cout<<"DTChar::dtmemfree()\n";
    if (pData!=NULL)
        delete (char*)pData;
	pData=NULL;
}

char* DTChar::DTName()
{
    if (debugmode) std::cout<<"DTChar::DTName()\n";
	char* rs=new char[strlen("char")+1];
	strcpy(rs,"char");
	rs[strlen("char")]=0;
	return rs;
}

char* DTChar::tostring()
{
    if (debugmode) std::cout<<"DTChar::tostring()\n";
	char* rs=new char[2];
	if (pData!=NULL)
	{
        rs[0]=*((char*)pData);
        rs[1]=0;
	}
    else
        rs="NULL";
	return rs;
}

//class DTPtr

DTPtr::DTPtr(char* name) //without memalloc!
{
	if (debugmode) std::cout<<"DTPtr("<<((name!=NULL)?name:"NULL")<<")\n";
	pData=NULL;
	size=0;
	if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
	}
    dtmemalloc();
}

DTPtr::DTPtr(char*name,char* tp,void* p)
{
	if (debugmode) std::cout<<"DTPtr("<<((name!=NULL)?name:"NULL")<<", "<<((tp!=NULL)?tp:"NULL")<<", <pointer>"<<")\n";
	dtmemalloc();
	if (tp!=NULL)
	{
        _tp=new char[strlen(tp)+1];
        strcpy(_tp,tp);
        _tp[strlen(tp)]=0;
	}
	pData=p;
	if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
    }
}

DTPtr::DTPtr(char*name,char* tp)
{
	if (debugmode) std::cout<<"DTPtr("<<((name!=NULL)?name:"NULL")<<", "<<((tp!=NULL)?tp:"NULL")<<")\n";
	dtmemalloc();
    _tp=new char[strlen(tp)+1];
    strcpy(_tp,tp);
    _tp[strlen(tp)]=0;
	pData=NULL;
	if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
    }
}

void DTPtr::dtmemalloc()
{
    if (debugmode) std::cout<<"DTPtr::dtmemalloc()\n";
    dtmemfree();
	pData=new (void**);
}

void DTPtr::dtmemfree()
{
    if (debugmode) std::cout<<"DTPtr::dtmemfree()\n";
    if (pData!=NULL)
        delete (void**)pData;
	pData=NULL;
}

char* DTPtr::DTName()
{
    if (debugmode) std::cout<<"DTPtr::DTName()\n";
    char* rs;
    if (_tp==NULL)
    {
        rs=new char[strlen("void*")+1];
        strcpy(rs,"void*");
        rs[strlen("void*")]=0;
    }else
    {
        std::string rss=_tp;
        rss+='*';
        rs=new char[rss.size()+1];
        strcpy(rs,rss.c_str());
        rs[rss.size()]=0;
    }
	return rs;
}

char* DTPtr::tostring()
{
    if (debugmode) std::cout<<"DTPtr::tostring()\n";
    char* rs;
    if (pData==NULL)
    {
        rs=new char[5];
        rs="NULL";
        return rs;
    }
    if ((strcmp(_tp,"char")==0)||(strcmp(_tp,"const char")==0))
    {
        rs=new char[strlen((char*)pData)+1];
        strcpy(rs,(char*)pData);
        rs[strlen((char*)pData)]=0;
    } else
    {
        char* tmp=new char[100];
        sprintf(tmp,"%d",*(int*)pData);
        rs=new char[strlen(tmp)+1];
        strcpy(rs,tmp);
        rs[strlen(tmp)]=0;
        delete[] tmp;
    }
	return rs;
}

//class DTArray

DTArray::DTArray(char* name,int sz,int c,char* tp) //with memalloc
{
	if (debugmode) std::cout<<"DTArray("<<((name!=NULL)?name:"NULL")<<", "<<sz<<", "<<c<<", "<<((tp!=NULL)?tp:"NULL")<<")\n";
	pData=new char[sz*c];
	size=size*c;
    size_one=sz;
    count=c;
    type_one=new char[strlen(tp)+1];
    strcpy(type_one,tp);
    type_one[strlen(tp)]=0;
    if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
	}
}

DTArray::DTArray(char* name,int sz,int c,char* tp,void* init) //with memalloc
{
	if (debugmode) std::cout<<"DTArray("<<((name!=NULL)?name:"NULL")<<", "<<sz<<", "<<c<<", "<<((tp!=NULL)?tp:"NULL")<<") with init\n";
	pData=init;
	size=size*c;
    size_one=sz;
    count=c;
    type_one=new char[strlen(tp)+1];
    strcpy(type_one,tp);
    type_one[strlen(tp)]=0;
    if (name!=NULL)
	{
        sIdent=new char[strlen(name)+1];
        strcpy(sIdent,name);
        sIdent[strlen(name)]=0;
	}
}

void DTArray::dtmemalloc()
{
    if (debugmode) std::cout<<"DTArray::dtmemalloc()\n";
    dtmemfree();
	pData=new char[size_one*count];
}

void DTArray::dtmemfree()
{
    if (debugmode) std::cout<<"DTArray::dtmemfree()\n";
    if (pData!=NULL)
        delete pData;
	pData=NULL;
}

char* DTArray::DTName()
{
    if (debugmode) std::cout<<"DTArray::DTName()\n";
	char* rs=new char[100];
	sprintf(rs,"array[sz:%i;cnt:%i]",size_one,count);
	return rs;
}

bool DTArray::FillElement(int n,void* buf)
{
    if (debugmode) std::cout<<"DTArray::FillElement("<<n<<",buf)\n";
    if (n>=count) return false;
    void* p=(char*)pData+(n*size_one);
    //p=buf;

    DTVar* o=ParseDataTypeString(type_one,NULL,NULL, NULL);
    if (o->dtet==dtetNative)
        memcpy(p,buf,size_one);
    else
        *(int*)p=(int)(buf);




    //char* x=new char[100];
    //memcpy(x,*(void**)p,size_one);
    //if (debugmode) std::cout<<*(char**)p<<"\n";
    //if (debugmode) std::cout<<*(char**)p<<"\n";
    delete o;
    return true;
}

DTVar* DTArray::GetElement(int n)
{
    if (debugmode) std::cout<<"DTArray::GetElement("<<n<<")\n";
    //void* p=*(void**)pData;//(char*)pData+(n*size_one);
    DTVar* o=ParseDataTypeString(type_one,NULL,NULL, NULL);
    //(*(DTMain*)(o->T)).pData=*((void**)((char*)pData+(n*size_one)));//*(void**)(p);
    //(*(DTMain*)(o->T)).pData
    if (o->dtet==dtetNative)
    {
        (*(DTMain*)(o->T)).pData=new char[size_one];
        memcpy((*(DTMain*)(o->T)).pData,((char*)pData+(n*size_one)),size_one);//*((void**)((char*)pData+(n*size_one)));//*(void**)(p);
    } else
    {
        //(*(DTMain*)(o->T)).pData=new char[size_one];
        (*(DTMain*)(o->T)).pData=*(void**)((((char*)pData+(n*size_one))));
    }
    return o;
}

char* DTArray::tostring()
{
    if (debugmode) std::cout<<"DTArray::tostring()\n";
    std::string st;
    DTVar* o;
    for(int i=0;i<count;i++)
    {
        o=GetElement(i);
        st+=(*(DTMain*)(o->T)).tostring();
        if ((i+1)!=count)
            st+=' ';
    }
    char* rs=new char[st.size()+1];
	strcpy(rs,st.c_str());
	rs[st.size()]=0;
	return rs;
}

DTVar* CalculateSum2op(DTMain* a,DTMain* b);
DTVar* CalculateDiff2op(DTMain* a,DTMain* b);
DTVar* CalculateDiff1op(DTMain* a);
DTVar* CalculateIncrement1op(DTMain* a);
DTVar* CalculateDecrement1op(DTMain* a);
DTVar* CalculateAddress1op(DTMain* a);
DTVar* CalculateDereference1op(DTMain* a);
DTVar* CalculateMul2op(DTMain* a,DTMain* b);
DTVar* CalculateDiv2op(DTMain* a,DTMain* b);
DTVar* CalculateLarger2op(DTMain* a,DTMain* b);
DTVar* CalculateLesser2op(DTMain* a,DTMain* b);
DTVar* CalculateLargerOrEqually2op(DTMain* a,DTMain* b);
DTVar* CalculateLesserOrEqually2op(DTMain* a,DTMain* b);
DTVar* CalculateEqually2op(DTMain* a,DTMain* b);
DTVar* CalculateNotEqually2op(DTMain* a,DTMain* b);
DTVar* CalculateAssignation(DTMain* a,DTMain* b, ag::list<DTVar*>* local);
DTVar* CalculateArrayGetElement(DTMain* a,DTMain* b);

DTVar* CalculateAct2op(DTMain* a,DTMain* b,char c1,char c2, ag::list<DTVar*>* local)
{
    if ((c1=='+')&&(c2==' '))
    {
        return CalculateSum2op(a,b);
    }
    if ((c1=='-')&&(c2==' '))
    {
        return CalculateDiff2op(a,b);
    }
    if ((c1=='*')&&(c2==' '))
    {
        return CalculateMul2op(a,b);
    }
    if ((c1=='/')&&(c2==' '))
    {
        return CalculateDiv2op(a,b);
    }
    if ((c1=='>')&&(c2==' '))
    {
        return CalculateLarger2op(a,b);
    }
    if ((c1=='<')&&(c2==' '))
    {
        return CalculateLesser2op(a,b);
    }
    if ((c1=='>')&&(c2=='='))
    {
        return CalculateLargerOrEqually2op(a,b);
    }
    if ((c1=='<')&&(c2=='='))
    {
        return CalculateLesserOrEqually2op(a,b);
    }
    if ((c1=='=')&&(c2=='='))
    {
        return CalculateEqually2op(a,b);
    }
    if ((c1=='!')&&(c2=='='))
    {
        return CalculateNotEqually2op(a,b);
    }
    if ((c1=='=')&&(c2==' '))
    {
        return CalculateAssignation(a,b, local);
    }
    if ((c1=='[')&&(c2==' '))
    {
        return CalculateArrayGetElement(a,b);
    }
}

DTVar* CalculateAct1op(DTMain* a,char c1,char c2)
{
    if ((c1=='-')&&(c2=='u'))
    {
        return CalculateDiff1op(a);
    }
    if ((c1=='&')&&(c2=='u'))
    {
        return CalculateAddress1op(a);
    }
    if ((c1=='*')&&(c2=='u'))
    {
        return CalculateDereference1op(a);
    }
    if ((c1=='+')&&(c2=='+'))
    {
        return CalculateIncrement1op(a);
    }
    if ((c1=='-')&&(c2=='-'))
    {
        return CalculateDecrement1op(a);
    }
}

DTVar* CalculateAssignation(DTMain* a,DTMain* b, ag::list<DTVar*>* local)
{
    if (((a->typeoftype()==b->typeoftype()))&&(a->typeoftype()==3))
    {
        a->dtmemfree();
        a->pData=b->pData;
    }else if ((a->typeoftype()==3)&&(b->typeoftype()==4))
    {
        a->dtmemfree();
        int a_size=((DTArray*)b)->count*((DTArray*)b)->size_one;
        a->pData=new char[a_size];
        memcpy(a->pData,((DTArray*)b)->pData,a_size);
    }
    else if (((a->typeoftype()==b->typeoftype()))||
         ((a->typeoftype()==2)&&(b->typeoftype()==1))||
         ((a->typeoftype()==1)&&(b->typeoftype()==2)))
    {
        //a->dtmemfree();
        rpnlist* rl=new rpnlist;
        RPNStackElement* rse=new RPNStackElement;
        rse->tp=rsetNum;
        rse->d=b;
        rl->add_tail(rse);
        DTVar* dv=ParseDataTypeString(a->DTName(),a->sIdent,rl,local);
        a->assign((DTMain*)(dv->T),true);
        if (debugmode) std::cout<<"CalculateAssignation(): a="<<a->tostring()<<"\n";
    }else
    {
        std::string ret="Uncompatible types in assignation: <";
        ret+=a->DTName();
        ret+="> and <";
        ret+=b->DTName();
        ret+=">";
        char* ret_s=new char[ret.size()+1];
        strcpy(ret_s,ret.c_str());
        ret_s[ret.size()]=0;
        throw ret_s;
    }
    return DTVar::CreateNativeDTVarFromDTMain(a);
}

DTVar* CalculateArrayGetElement(DTMain* a,DTMain* b)
{
    if ((a->typeoftype()==4)&&(b->typeoftype()==1))
    {
        return (((DTArray*)a)->GetElement(b->toint()));
    }
    if ((a->typeoftype()==3)&&(b->typeoftype()==1))
    {
        if (((DTPtr*)a)->_tp!=NULL)
        {
            DTVar* dv=ParseDataTypeString(a->DTName(),NULL,NULL,NULL);
            ((DTMain*)(dv->T))->pData=((char*)(((DTPtr*)a)->pData))+(b->toint())*(((DTMain*)(dv->T))->sizeoftype());
            return dv;
        }else
        {
        }

    }
}

DTVar* CalculateSum2op(DTMain* a,DTMain* b)
{
     //integer
     if (((a->typeoftype()==b->typeoftype())&&(a->typeoftype()==1))||((a->typeoftype()==3)&&(b->typeoftype()==1)))
     {
        if (a->typeoftype()==3)
        {
            DTPtr* k=new DTPtr(NULL, ((DTPtr*)a)->_tp, (void*)(((DTIntegerTypes*)a)->toint()+((DTIntegerTypes*)b)->toint()));
            return DTVar::CreatePointerDTVarFromDTMain(k);
        }
        else
        {
            DTBigIntegerType* k=new DTBigIntegerType(NULL, ((DTIntegerTypes*)a)->toint()+((DTIntegerTypes*)b)->toint());
            return DTVar::CreateNativeDTVarFromDTMain(k);
        }
     };
     //float
     if (((a->typeoftype()==2)&&((b->typeoftype()==1)||(b->typeoftype()==2)))||
         ((b->typeoftype()==2)&&((a->typeoftype()==1)||(a->typeoftype()==2))))
     {
         DTBigFloatType* k=new DTBigFloatType(NULL, ((DTFloatTypes*)a)->tofloat()+((DTFloatTypes*)b)->tofloat());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
}

DTVar* CalculateDiff2op(DTMain* a,DTMain* b)
{
     //integer
     if (((a->typeoftype()==b->typeoftype())&&(a->typeoftype()==1))||((a->typeoftype()==3)&&(b->typeoftype()==1)))
     {
        if (a->typeoftype()==3)
        {
            DTPtr* k=new DTPtr(NULL, ((DTPtr*)a)->_tp, (void*)(((DTIntegerTypes*)a)->toint()-((DTIntegerTypes*)b)->toint()));
            return DTVar::CreatePointerDTVarFromDTMain(k);
        }
        else
        {
            DTBigIntegerType* k=new DTBigIntegerType(NULL, ((DTIntegerTypes*)a)->toint()-((DTIntegerTypes*)b)->toint());
            return DTVar::CreateNativeDTVarFromDTMain(k);
        }
     };
     //float
     if (((a->typeoftype()==2)&&((b->typeoftype()==1)||(b->typeoftype()==2)))||
         ((b->typeoftype()==2)&&((a->typeoftype()==1)||(a->typeoftype()==2))))
     {
         DTBigFloatType* k=new DTBigFloatType(NULL, ((DTFloatTypes*)a)->tofloat()-((DTFloatTypes*)b)->tofloat());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
}

DTVar* CalculateLarger2op(DTMain* a,DTMain* b)
{
     //integer or float
     if ((ag::set("\1\2\3")%(a->typeoftype()))&&(ag::set("\1\2\3")%(b->typeoftype())))
     {
         DTBigIntegerType* k=new DTBigIntegerType(NULL, ((DTFloatTypes*)a)->tofloat()>((DTFloatTypes*)b)->tofloat());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
}

DTVar* CalculateLargerOrEqually2op(DTMain* a,DTMain* b)
{
     //integer or float
     if ((ag::set("\1\2\3")%(a->typeoftype()))&&(ag::set("\1\2\3")%(b->typeoftype())))
     {
         DTBigIntegerType* k=new DTBigIntegerType(NULL, ((DTFloatTypes*)a)->tofloat()>=((DTFloatTypes*)b)->tofloat());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
}

DTVar* CalculateLesser2op(DTMain* a,DTMain* b)
{
     //integer or float
     if ((ag::set("\1\2\3")%(a->typeoftype()))&&(ag::set("\1\2\3")%(b->typeoftype())))
     {
         DTBigIntegerType* k=new DTBigIntegerType(NULL, ((DTFloatTypes*)a)->tofloat()<((DTFloatTypes*)b)->tofloat());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
}

DTVar* CalculateLesserOrEqually2op(DTMain* a,DTMain* b)
{
     //integer or float
     if ((ag::set("\1\2\3")%(a->typeoftype()))&&(ag::set("\1\2\3")%(b->typeoftype())))
     {
         DTBigIntegerType* k=new DTBigIntegerType(NULL, ((DTFloatTypes*)a)->tofloat()<=((DTFloatTypes*)b)->tofloat());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
}

DTVar* CalculateEqually2op(DTMain* a,DTMain* b)
{
     //integer or float
     if ((ag::set("\1\2\3")%(a->typeoftype()))&&(ag::set("\1\2\3")%(b->typeoftype())))
     {
         DTBigIntegerType* k=new DTBigIntegerType(NULL, ((DTFloatTypes*)a)->tofloat()==((DTFloatTypes*)b)->tofloat());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
}

DTVar* CalculateNotEqually2op(DTMain* a,DTMain* b)
{
     //integer or float
     if ((ag::set("\1\2\3")%(a->typeoftype()))&&(ag::set("\1\2\3")%(b->typeoftype())))
     {
         DTBigIntegerType* k=new DTBigIntegerType(NULL, ((DTFloatTypes*)a)->tofloat()!=((DTFloatTypes*)b)->tofloat());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     }
}

DTVar* CalculateDiff1op(DTMain* a)
{
     //integer
     if (a->typeoftype()==1)
     {
         DTInt* k=new DTBigIntegerType(NULL, -((DTIntegerTypes*)a)->toint());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
     //float
     if (a->typeoftype()==2)
     {
         DTBigFloatType* k=new DTBigFloatType(NULL, -((DTFloatTypes*)a)->tofloat());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
}

DTVar* CalculateDecrement1op(DTMain* a)
{
     //integer
     if (a->typeoftype()==1)
     {
         DTInt* k=new DTBigIntegerType(NULL, ((DTIntegerTypes*)a)->toint()-1);
         CalculateAssignation(a,k,NULL);
         return DTVar::CreateNativeDTVarFromDTMain(k);
     }
     //float
     if (a->typeoftype()==2)
     {
         DTBigFloatType* k=new DTBigFloatType(NULL, ((DTFloatTypes*)a)->tofloat()-1);
         CalculateAssignation(a,k,NULL);
         return DTVar::CreateNativeDTVarFromDTMain(k);
     }
}

DTVar* CalculateIncrement1op(DTMain* a)
{
     //integer
     if (a->typeoftype()==1)
     {
         DTInt* k=new DTBigIntegerType(NULL, ((DTIntegerTypes*)a)->toint()+1);
         CalculateAssignation(a,k,NULL);
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
     //float
     if (a->typeoftype()==2)
     {
         DTBigFloatType* k=new DTBigFloatType(NULL, ((DTFloatTypes*)a)->tofloat()+1);
         CalculateAssignation(a,k,NULL);
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
}

DTVar* CalculateAddress1op(DTMain* a)
{
    DTVar* x=ParseDataTypeString(a->DTName(),NULL,NULL,NULL);
    if (x->dtet==dtetNative)
    {
        DTPtr* m=new DTPtr(NULL,a->DTName(),a->pData);
        return DTVar::CreatePointerDTVarFromDTMain(m);
    }
    if (x->dtet==dtetPointer)
    {
        void** x=new void*;
        *x=a->pData;
        DTPtr* m=new DTPtr(NULL,a->DTName(),x);
        return DTVar::CreatePointerDTVarFromDTMain(m);
    }
}

DTVar* CalculateDereference1op(DTMain* a)
{
     DTVar* x=ParseDataTypeString(((DTPtr*)a)->_tp,NULL,NULL,NULL);
     if(x->dtet==dtetNative)
        ((DTMain*)x->T)->pData=a->pData;
     if(x->dtet==dtetPointer)
        ((DTMain*)x->T)->pData=*(void**)a->pData;
     return x;
}

DTVar* CalculateMul2op(DTMain* a,DTMain* b)
{
     //integer
     if ((a->typeoftype()==b->typeoftype())&&(a->typeoftype()==1))
     {
         DTInt* k=new DTBigIntegerType(NULL, ((DTIntegerTypes*)a)->toint()*((DTIntegerTypes*)b)->toint());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
     //float
     if (((a->typeoftype()==2)&&((b->typeoftype()==1)||(b->typeoftype()==2)))||
         ((b->typeoftype()==2)&&((a->typeoftype()==1)||(a->typeoftype()==2))))
     {
         DTBigFloatType* k=new DTBigFloatType(NULL, ((DTFloatTypes*)a)->tofloat()*((DTFloatTypes*)b)->tofloat());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
}

DTVar* CalculateDiv2op(DTMain* a,DTMain* b)
{
     //integer
     if ((a->typeoftype()==b->typeoftype())&&(a->typeoftype()==1))
     {
         DTInt* k=new DTBigIntegerType(NULL, ((DTIntegerTypes*)a)->toint()/((DTIntegerTypes*)b)->toint());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
     //float
     if (((a->typeoftype()==2)&&((b->typeoftype()==1)||(b->typeoftype()==2)))||
         ((b->typeoftype()==2)&&((a->typeoftype()==1)||(a->typeoftype()==2))))
     {
         DTBigFloatType* k=new DTBigFloatType(NULL, ((DTFloatTypes*)a)->tofloat()/((DTFloatTypes*)b)->tofloat());
         return DTVar::CreateNativeDTVarFromDTMain(k);
     };
}


void* DoIntInitializeActions(int t,char* sDT,char* sName, rpnlist* data,DTVar* k, ag::list<DTVar*>* local)
{
    switch(t)
    {
        case 1:k->T=new DTInt(sName);break;
        case 2:k->T=new DTUInt(sName);break;
        case 3:k->T=new DTChar(sName);break;
    }
    if (data!=NULL)
    {
        if (!data->empty())
        {
            ag::stack<DTVar*>* v=CalculateRPN(data,local);
            DTMain* h=(DTMain*)(v->pop()->T);
            if(h->typeoftype()<=2)
            {
                switch(h->typeoftype())
                {
                    case 1:((DTIntegerTypes*)(k->T))->setint(((DTIntegerTypes*)h)->toint());break;
                    case 2:((DTIntegerTypes*)(k->T))->setint(((DTFloatTypes*)h)->tofloat());break;
                }
            }
            else
            {
                std::string u="Incompatible types: <";
                u+=h->DTName();
                u+="> and <";
                u+=((DTMain*)k->T)->DTName();
                u+=">";
                char* cstr = new char [u.size()+1];
                strcpy (cstr, u.c_str());
                throw cstr;
            }
            delete v;
        }else
        {
            ((DTIntegerTypes*)(k->T))->setint(0);
        }
    }
}

void* DoDoubleInitializeActions(int t,char* sDT,char* sName, rpnlist* data,DTVar* k, ag::list<DTVar*>* local)
{
    switch(t)
    {
        case 1:k->T=new DTFloat(sName);break;
        case 2:k->T=new DTDouble(sName);break;
    }
    if (data!=NULL)
    {
        if (!data->empty())
        {
            ag::stack<DTVar*>* v=CalculateRPN(data, local);
            DTMain* h=(DTMain*)(v->pop()->T);
            if (h->typeoftype()<=2)
            {
                switch(h->typeoftype())
                {
                    case 1:((DTFloatTypes*)(k->T))->setfloat(((DTIntegerTypes*)h)->toint());break;
                    case 2: ((DTFloatTypes*)(k->T))->setfloat(((DTFloatTypes*)h)->tofloat());break;
                }
            }
            else
            {
                std::string u="Incompatible types: ";
                u+=h->DTName();
                u+="and ";
                u+=((DTMain*)k->T)->DTName();
                throw u.c_str();
            }
            delete v;
        }else
        {
            ((DTFloatTypes*)(k->T))->setfloat(0);
        }
    }
}

DTVar* DTMakeCopy(DTVar* x)
{
    DTVar* m = ParseDataTypeString(((DTMain*)(x->T))->DTName(),((DTMain*)(x->T))->sIdent,NULL,NULL);
}

DTVar* ParseDataTypeString(char* sDT,char* sName, rpnlist* data, ag::list<DTVar*>* local)
{
    try{

        DTVar* k=new DTVar;
        if (debugmode) std::cout<<"sDT: "<<sDT<<"\n";
        if (sDT[strlen(sDT)-1]=='*')
        {
            if (debugmode) std::cout<<sDT<<" is pointer\n";
            char* ss=new char[strlen(sDT)];
            strcpy(ss,sDT);
            ss[strlen(ss)-1]=0;
            //trim
            while(ss[0]==' ')ss++;
            while(ss[strlen(ss)-1]==' ')ss[strlen(ss)-1]=0;

            k->dtet=dtetPointer;
            //k->T=ParseDataTypeString(ss,sName,data);
            k->T=new DTPtr(sName,ss);

//            data->add_head(k);
            return k;
        }
        if (debugmode) std::cout<<sDT<<" is native data\n";
        k->dtet=dtetNative;
        if ((strcmp(sDT,"signed int")==0)||(strcmp(sDT,"int")==0))
        {
            DoIntInitializeActions(1,sDT,sName,data,k, local);
        }else
        if (strcmp(sDT,"unsigned int")==0)
        {
            DoIntInitializeActions(2,sDT,sName,data,k, local);
        }else
        if (strcmp(sDT,"char")==0)
        {
            DoIntInitializeActions(3,sDT,sName,data,k, local);
        }else
        if (strcmp(sDT,"float")==0)
        {
            DoDoubleInitializeActions(1,sDT,sName,data,k, local);
        }else
        if (strcmp(sDT,"double")==0)
        {
            DoDoubleInitializeActions(2,sDT,sName,data,k, local);
        }else
        {
            char* K=new char[strlen("parsing: undeclarated typename: ")+strlen(sDT)];
            strcpy(K,"parsing: undeclarated typename: ");
            strcat(K,sDT);
            throw K;
        }
        if (debugmode) std::cout<<"Parsing DT finished\n";
        return k;
    }catch(char* o)
    {
        if (debugmode) std::cout<<"(ERROR) "<<o<<"\n";
    }
    catch(const char* k)
    {
        if (debugmode) std::cout<<"(Error): "<<k<<"\n";
    }
    catch(...)
    {
        if (debugmode) std::cout<<"(UERROR)\n";
    }
}
