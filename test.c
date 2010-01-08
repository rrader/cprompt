#include <math.h>
outside cdecl: int printf(int x,double f,char*s);
int main(int argv,char* argc[])
{
    int l=round(7.2);
    printf(l,7.2,"round(%f) = %d");
}
