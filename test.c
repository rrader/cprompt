outside cdecl: int printf(char*s, ...);
outside cdecl: int scanf(char*s, ...);
outside cdecl: double floor(double x);
outside cdecl: int sprintf (char* str, char* format, ... );
outside cdecl: void* malloc (int size);
outside cdecl: int strlen(char* s);

int main(int argc,char* argv[])
{
    char* x="1234567890";
    x=x+3;
    printf("%s",x);
}
