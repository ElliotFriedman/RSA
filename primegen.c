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

	printf("prime: %u\n", (unsigned int)*num);
	
	phi = *num - 1;
	prev = *num;
	n = *num;
	ranGen(salt, e);

	num = (int*)salt;
	*num = createPrime(*num);
	printf("prime: %u\n", (unsigned int)*num);

	phi *= (*num - 1);
	n *= *num;
	if (n / *num != prev)
		printf("\n\n\n\nError, integer overflow, invalid results\n\n\n\n\n");
	printf("phi: %llu\nn  : %llu\n", phi, n);
	fclose(e);

	return 0;
}
