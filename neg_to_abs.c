//Write a program how asked for 10 integers, and replace each negative number with his ABS value using the last function.

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define SIZE 10

int main() {
	int i = 0;
	int arr[SIZE];
	int arr2[SIZE];
	printf("Enter 10 integers: \n");
	for (i = 0; i < SIZE; i++) {
		scanf("%d", &arr[i]);
	}
	for (i = 0; i < SIZE; i++) {
		if (arr[i] < 0)
			arr2[i] = -arr[i];
		else
			arr2[i] = arr[i];
	}
	printf("The new array is: \n");
	for (i = 0; i < SIZE; i++) {
		printf("%d ", arr2[i]);
	}
	return 0;
}