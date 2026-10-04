// compute GCD of two positive integers by Euclidan algorithm
// author: D. J. Guan
// date: 2018/09/10

#include <stdio.h>
#include <stdlib.h>

int gcd(const int x, const int y)
{
	int a, b, r;

	a = abs(x);
	b = abs(y);
	while (b) {
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

int main()
{
	int a, b, d;

	while (scanf("%d %d", &a, &b) != EOF) {
		d = gcd(a, b);
		printf("(%d, %d) = %d\n", a, b, d);
	}
	return 0;
}
