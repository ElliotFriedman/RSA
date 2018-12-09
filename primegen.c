#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ll unsigned long long

void						ranGen(char *salt, FILE *e)
{
	int		i;

	i = 0;
	while (i < 5)
	{
		char a = (unsigned int)getc(e);
		while ((a & 127) < 32)
			a += (unsigned int)getc(e);
		salt[i] = a;
		i++;
	}
}

int createPrime(unsigned int num)
{
	int i = 2;

	while (1)
	{
		i = 2;
		//make sure we haven't found a factor,
		//then make sure the factors when multiplied together do not exceed the number we are looking for
		//naive implementation
		while (num % i != 0 && i * i < num)
			i++;
		if (i * i == num)
			return num;
		if (i != num)
			num++;
	}
}

//thank you wikipedia
unsigned long long gcd(unsigned int u, unsigned long long v)
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

// geeks for geeks privkey implementation
// C function for extended Euclidean Algorithm 
int gcdExtended(int a, int b, int *x, int *y) 
{ 
    // Base Case 
    if (a == 0) 
    { 
        *x = 0; 
        *y = 1; 
        return b; 
    } 
  
    int x1, y1; // To store results of recursive call 
    int gcd = gcdExtended(b%a, a, &x1, &y1); 
  
    // Update x and y using results of recursive 
    // call 
    *x = y1 - (b/a) * x1; 
    *y = x1; 
  
    return gcd; 
} 

//my privkey implementation
/*
ll	generatePrivKey(unsigned int e, ll phi)
{
	long long	work[3][2];
	int i;

	//zero array
	i = 0;
	while (i < 3)
	{
		int j = 0;

		while (j < 2)
		{
			work[i][j] = 0;
			j++;
		}
		i++;
	}
	work[0][0] = phi;
	work[0][1] = phi;
	work[1][0] = e;
	work[1][1] = 1;

	while (work[2][0] != 1)
	{

		//divide, multiply and subtract left side
		printf("test0 \n%lld %lld\n%lld %lld\n%lld %lld\n", work[0][0], work[0][1], work[1][0], work[1][1],work[2][0], work[2][1]);
		work[2][0] = work[0][0] / work[1][0];
		printf("test1\n");
		work[2][0] = work[2][0] * work[1][0];
		work[2][0] -= work[0][0];

		//divide, multiply and subtract right side
		work[2][1] = (work[0][0] / work[1][0]) * work[1][1];
		work[2][1] -= work[0][1];

		//check for negatives
		work[2][0] = work[2][0] < 0 ? work[2][0] + phi : work[2][0];
		work[2][1] = work[2][1] < 0 ? work[2][1] + phi : work[2][1];

		//copy upwards
		int i = 0;
		while (i < 2)
		{
			work[i][0] = work[i + 1][0];
			work[i][1] = work[i + 1][1];
			i++;
		}
		work[2][1] = 0;
		work[2][0] = 0;
	}

	//sanity check
	//make sure that d was computed correctly
	if ((e * work[2][0]) % phi != 1)
		printf("Invalid d computed, %lld\n\n\n\n\n", work[2][0]);

	return work[2][0];
}
*/
int main(int argc, char **argv)
{
	FILE		*e;
	char		*salt;
	int			*num;
	ll			n;
	ll			phi;
	int			prev;

	salt = malloc(8);
	e = fopen("/dev/urandom", "r");
	ranGen(salt, e);

	num = (int*)salt;

	//generate prime
	*num %= 10129921;
	*num *= *num < 0 ? -1 : 1;
	*num = createPrime(*num);

	printf("p: %u\n", (unsigned int)*num);

	phi = *num - 1;
	prev = *num;
	n = *num;
	ranGen(salt, e);
	fclose(e);

	num = (int*)salt;
	//printf("second random num: %d\n w/modulus %d\n", 
	*num *= *num < 0 ? -1 : 1;	
	*num = createPrime(*num % 1000002);
	printf("q: %u\n", (unsigned int)*num);

	phi *= (*num - 1);
	n *= *num;

	//check overflow	
	if (n / *num != prev)
		printf("\n\n\n\nError, integer overflow, invalid results\n\n\n\n\n");

	printf("phi: %llu\nn  : %llu\n", phi, n);

	//require that gcd(e, phi) == 1, e and phi can have NO cofactors except 1
	unsigned int e1 = 20103;
	while (gcd(e1, phi) != 1)
		e1++;
	printf("e  : %d\n", e1);


	 
	printf("d  : %llu\n", gcdExtended(e1, phi, (int*)salt, &prev));

	//printf("d  : %llu\n", gcdExtended(phi, e1, (int*)salt, &prev));
	return 0;
}
