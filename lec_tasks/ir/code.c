int function2(int *a , int n)
{
    int result = 0 ;
    for (int i = 0; i < n; ++i)
    {
        int x = 2;
        int y = 1;
        if (i % 2 == 0)
        {
            result += a[i] * x * y;
        }
        else
        {
            result -= a[i] + x * y;
        }
    }
    return result;
}
