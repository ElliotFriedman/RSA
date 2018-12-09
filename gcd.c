#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//thank you wikipedia
unsigned int gcd(unsigned int u, unsigned int v)
{
    // simple cases (termination)
    if (u == v)
        return u;

    if (u == 0)
        return v;

    if (v == 0)
        return u;

    // look for factors of 2
    if (~u & 1) // u is even
    {
        if (v & 1) // v is odd
            return gcd(u >> 1, v);
        else // both u and v are even
            return gcd(u >> 1, v >> 1) << 1;
    }

    if (~v & 1) // u is odd, v is even
        return gcd(u, v >> 1);

    // reduce larger argument
    if (u > v)
        return gcd((u - v) >> 1, v);

    return gcd((v - u) >> 1, u);
}

int main(int argc, char **argv)
{

	if (argc == 3)
	{
		int x = atoi(argv[1]);
		int y = atoi(argv[2]);
		//factor GCD
		printf("GCD of %d, %d = %d\n", x, y, gcd(x, y));
	}
	else
		printf("Usage: ./GCD x y\n");

	return 0;
}
