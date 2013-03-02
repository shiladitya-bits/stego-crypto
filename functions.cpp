#include "stegosecure.h"
int getBit(int n,int i)
{
	if(n&(1<<i))
		return 1;
	else
		return 0;
}
int digSum(long n)
{
	int sum=0;
	while(n>0)
	{
		sum+=n%10;
		n/=10;
	}
	return sum;
}
int checkPrime(long n)
{
	for(int i=2;i<=n/2;i++)
	{
		if(n%i==0) return 0;
	}
	return 1;
}
long nextPrime(long n)
{
	while(!checkPrime(n))
		n++;
	return n;
}
