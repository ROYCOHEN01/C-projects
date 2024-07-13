#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
	int i = 0;
	char str[100];
	scanf("%99s", str);
	int j = strlen(str) - 1;
	if (j % 2 == 0)
	{
		while (i != j)
		{

			if (str[i] != str[j]) {
				printf("not polidrome\n");
				break;
			}
			i++;
			j--;
		}
		if (i == j)
			printf("polidrome\n");
	}
		else
	{
		while (i != j-1)
		{
			if (str[i] != str[j]) {
				printf("not polidrome\n");
				break;
			}
			i++;
			j--;
		}
		if (i == j - 1)
		{
			if (str[i] == str[j])
				printf("polidrome\n");
			else
				printf("not polidrome\n");
		}
	}
	
}