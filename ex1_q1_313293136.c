#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

int main()
{
	int a;
	int i;
	printf("Input: n = ");
	scanf("%d", &a);
	if (a == 0)
	{
		printf("Output:");
	}
	else
	{
		printf("Output: ");

		for (i = 1; i < a; i++)

		{
			if (a % i == 0)
			{
				printf("%d ", i);

			}
		}
		printf("%d", a);
	}
	return 0;
}