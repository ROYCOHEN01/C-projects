#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>

void print_quadratic(double a, double b, double c);

int find_roots(double a, double b, double c, double* p1, double* p2);

int main()
{
    double a, b, c;
    printf("Please enter three numbers:\n");
    scanf("%lf%lf%lf", &a, &b, &c);

    double r1, r2;
    int n = find_roots(a, b, c, &r1, &r2);

    print_quadratic(a, b, c);
    printf(" = 0 has ");

    switch (n) {
    case 0:
        printf("no real roots\n");
        break;
    case 1:
        printf("one real root: %g\n", r1);
        break;
    case 2:
        printf("two real roots: %g and %g\n", r1, r2);
        break;
    case -1:
        printf("infinite real roots\n");
        break;
    default:
        printf("%d roots? impossible!\n", n);
    }

    return 0;
}

void print_quadratic(double a, double b, double c)
{
    if (a == 0 && b == 0 && c == 0)
		printf("0");
	if (a != 0)
        {
        if(a==1)
			printf("x^2");
		if (a== -1)
			printf("-x^2");
		else if (a != 1 && a != -1)
		printf("%gx^2", a);
		}
    if (b != 0)
    {
        if ((b>0) && (a != 0))
			printf("+");
		if (b == 1)
			printf("x");
        if (b == -1)
            printf("-x");
        else if(b != 1 && b != -1)
            printf("%gx", b);
    }
    if (c != 0){
		if (c > 0)
		{
			if (a != 0 || b != 0)
				printf("+", c);
			printf("%g", c);
		}
		else if (c < 0)
			printf("%g", c);
    }


}
// 000 doesn't work for me it doesnt show the value of the root
int find_roots(double a, double b, double c, double* p1, double* p2) //how to calculate only 4 points after the decimal point?
{
    if (a ==0)
	{
		if (b == 0)
		{
			if (c == 0)
			{
				*p1 = 0;
				return -1;
			}
			else
				return 0;
		}
		else
		{
			*p1 = -c / b;
			return 1;
		}
	}
	else
	{
		double sqr = b * b - 4 * a * c;
		if (sqr < 0)
			return 0;
		else if (sqr == 0)
		{
			if (b == 0)
				*p1 = 0;
			else
				*p1 = -b / (2 * a);
			return 1;
		}
		else
		{
			*p1 = (-b - sqrt(sqr)) / (2 * a);
			*p2 = (-b + sqrt(sqr)) / (2 * a);
			return 2;
		}
	}
}