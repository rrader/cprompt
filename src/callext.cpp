#include "callext.h"
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

// cdecl

int CallIt(char* sname,void* handle,ag::list<DTVar*>* list)
{
    void* buf=new char[1024];
    *(int*)(buf)=100;
    double x=1.5;
    memcpy(buf+8,&x,4);
    memcpy(buf+4,(char*)&x+4,4);
    printf(": %f\n",*((double*)(buf+4)));
    *(int*)(buf+12)=200;
    int count = 4;
    int b;

    void* addr=dlsym(handle,sname);

    asm("\
    movl $0, %%eax;\
    movl %2,%%ebx; \
    movl %3,%%ecx; \
    l1: cmpl %%ecx, %%eax; \
    je l2;\
    pushl (%%ebx,%%eax,4); \
    addl $1,%%eax;\
    jmp l1;\
    l2: call *%1;\
    movl %%eax, %0;"
    :"=r"(b)
    : "r"(addr), "r"(buf), "g"(count)
    : "%eax"
    );
    printf("\n%d",b);
    //delete[] buf;  (%%ebx,%%eax,4)
}
