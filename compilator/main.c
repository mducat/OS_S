void keep(short int a)
{
    (void)a;
}

void hello(int n){
    int hel = 0x123456;
    if (n)
        hello(--n);
    (void)hel;
}       

int main()
{
    hello(156);
    return 0;
}