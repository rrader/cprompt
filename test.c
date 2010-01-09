#include <math.h>
outside cdecl: int printf(char*s,double f,int x);
int main(int argv,char* argc[])
{
    int l=round(7.2);
    printf("round(%f)w = %d \n",7.2,l);
}
