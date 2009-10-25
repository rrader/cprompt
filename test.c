signed int test(signed int k)
{
    signed int y=k+5;
    y=1-y;
    return y;
}

signed int main(signed int argc,char* argv[])
{
    signed int k1;
    k1=test(10);
    #TEST out k1
}

