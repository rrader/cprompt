outside cdecl: int printf(char*s, ...);
outside cdecl: int scanf(char*s, ...);

int main(int argc,char* argv[])
{
    float x=1;
    scanf("%f",&x);
    printf("%f\n",x);
}

