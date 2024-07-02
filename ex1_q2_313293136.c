#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

int main()
{		
	int num;
	int i;
	printf("Enter a number to reverse: ");
	scanf("%d", &num);
	printf("Reverse of number is: ");
	if (num < 0)
	{
		printf("-");
		num = -1 * num;
	}
	if (num == 0)
		printf("0");
	while (num % 10 == 0)
		num = num / 10;
	while (num > 0)
	{
		i = num % 10;
		printf("%d", i);
		num = num / 10;
	}
	return 0;
}