outside cdecl: int printf(char*s, ...);

int main(int argc,char* argv[])
{
    char* x="1234567890";
    x=x+3;
    printf("123%s",x);
}
