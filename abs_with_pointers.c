//check with pointers how many negative numbers are in the array
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define SIZE 10
int split_by_pointers(int num[], int* ptr[], int size) {
	int count = 0;
	int* p = NULL;
	int* last = num + size;
	for (p = num; p < last; p++)
	{
		if(*p < 0)
		{
			ptr[count] = p;
			count++;
		}
	}
	return count;
}

int main() {
	int num[SIZE] = { 0 };
	int i = 0;
	int* ptr[SIZE] = { 0 };
	printf("Enter 10 numbers: \n");
	for (i = 0; i < SIZE; i++)
	{
		printf("Enter number %d: ", i + 1);
		scanf("%d", &num[i]);
	}
	int count = split_by_pointers(num, ptr, SIZE);
	printf("The number of negative numbers is %d\n", count);

	int new_arr[SIZE] = { 0 };
	for (i = 0; i < count; i++) {
		(*ptr[i]) = (*ptr[i]) * (-1);
	}
	printf("The array after changing the negative numbers to positive is: ");
	for (i = 0; i < SIZE; i++)
	{
		printf("%d ", num[i]);
	}
	return 0;

}
