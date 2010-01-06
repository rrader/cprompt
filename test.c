#include <cmath>
outside cdecl: int printf(double x, char* s);

int main(int argv,char* argc[])
{
    int l=round(7.2);
    int x=printf(l,"hi! %f");
}
