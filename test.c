outside cdecl: int printf(char*s, ...);
outside cdecl: int scanf(char*s, ...);
outside cdecl: double floor(double x);
outside cdecl: int sprintf (char* str, char* format, ... );
outside cdecl: void* malloc (int size);
outside cdecl: int strlen(char* s);

double round(double value )
{
  return floor( value + 0.5 );
}

int main(int argc,char* argv[])
{
    float x=1;
    int l=round(x);
    printf("%f = %d;\n",x,l);
}

