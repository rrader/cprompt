

int pow(int base,int k) {
    if (k>0)
        return base*pow(base,k-1);
    else
        return 1;
}

int main(int argc,char* argv[])
{
    int x=pow(3,2);
    char* y;
//    y=malloc(4);
    #TEST out x
}
