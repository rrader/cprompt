int main(int argc,char* argv[])
{
    double k1=4;
    int k2=4;
    if (k1/k2)
    {
        #TEST out k1
        k1=10;
    }else
    {
        #TEST out k1
        k1=20;
    }

    if (k1==20)
    {
        #TEST out k1
        k1=100;
    }else
    {
        #TEST out k1
        k1=200;
    }

    #TEST out k1
}
