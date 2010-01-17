outside cdecl: int printf(char*s, ...);
outside cdecl: int scanf(char*s, ...);

int main(int argc,char* argv[])
{
    for(int i=0;i<argc;i++)
    {
        printf("%s\n",argv[i]);
    }
}

