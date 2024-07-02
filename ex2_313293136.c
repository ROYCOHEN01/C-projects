#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <math.h>

// NOTE: Since <math.h> is included, you can use sqrt() to calculate the square-root of a number.

int q_1(int res_decimal);

double q_2(int n);

double q_3(int n);

double sqrt_q3(int n, int t);

int main()
{
	int q_num = 0, n = 0, res_decimal = 0;
	double res;

	printf("Which question would you like to check?: ");
	scanf("%d", &q_num);
	printf("Please enter a number: ");
	scanf("%d", &n);
	switch (q_num)
	{
	case 1:
		res_decimal = q_1(n);
		printf("Result = %d", res_decimal);
		break;
	case 2:
		res = q_2(n);
		printf("Result = %lf", res);
		break;
	case 3:
		res = q_3(n);
		printf("Result = %lf", res);
		break;
	default:
		printf("%d is an invalid input", q_num);
	}
	return 0;
}

int q_1(int res_decimal)
{
	if (res_decimal <= 1)
		return res_decimal;
	else
	{
		q_1(res_decimal / 2);
		return (res_decimal % 2) + 10 * q_1(res_decimal/2);
	}
}

double q_2(int n)
{
	if (n < 0)
		return 0;
	if (n == 1)
		return 1.0;
	else
		return sqrt(n + (q_2(n - 1)));
}

double q_3(int n)
{
	if (n == 1)
		return 1;
	if (n == 0)
		return 0;
	return sqrt_q3(n, 1);
}

double sqrt_q3(int n, int t)
{
	if (t == n)
		return sqrt(t);
	else
		return sqrt((t) + sqrt_q3(n,t+1));
}