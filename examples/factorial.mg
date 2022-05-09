// computes the factorial of any given integer
func factorial(int n) -> int
{
    if (n < 2)
    {
        return 1;
    }

    return n * factorial(n-1);
}
