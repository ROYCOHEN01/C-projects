#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <math.h>

#define MAX_SIZE 30

int q_1(char arr[]);

int q_2(char arr[], int k);

int q_3(char arr[]);

int q_4(char arr[]);


int main()
{
	int q_num = 0, n = 0, res = 0, k = 0;
	char arr[MAX_SIZE];

	printf("Which question would you like to check?: ");
	scanf("%d", &q_num);
	printf("Please enter the input array: ");
	scanf("%s", arr);
	switch (q_num)
	{
	case 1:
		res = q_1(arr);
		printf("Result = %d", res);
		break;
	case 2:
		printf("Please enter k: ");
		scanf("%d", &k);
		res = q_2(arr, k);
		printf("Result = %d", res);
		break;
	case 3:
		res = q_3(arr);
		printf("Result = %d", res);
		break;
	case 4:
		res = q_4(arr);
		printf("Result = %d", res);
		break;
	default:
		printf("%d is an invalid input", q_num);
	}
	return 0;
}
int string_length(char arr[]) { //counting the numbers at the string.
	int length = 0;
	int i = 0;
	if (arr[0] == '\0')
		return 0;
	while (arr[i] != '\0') {
		if (arr[i] == ',')
			length++;
		i++;
	}
	return length + 1;
}

int char_to_int(char* input, int* number)
{
	int num = 0;
	int i = 0;
	int neg = 0;
	int size = 0;
	while (input[i] != '\0' && input[i] != '\n') {
		if (input[i] == '-')
			neg = 1;
		else if (input[i] >= '0' && input[i] <= '9')
			num = num * 10 + (input[i] - '0');
		else if (input[i] == ',')
		{
			if (neg == 1) {
				num = -num;
				neg = 0;
			}
			number[size++] = num;
			num = 0;
		}
		i++;
	}
	if (neg == 1)
		num = -num;
	number[size++] = num;
	return size;
}


int q_1(char arr[])
{
	int max = 0;
	int MaxLength = 0;
	int i, j;
	int SIZE = string_length(arr);
	int numbers[MAX_SIZE] = { 0 }; //put in  SIZE or MAXSIZE exchange of MAX_SIZE 
	int parsed_count = char_to_int(arr, numbers); 
	
	for (i = 0; i < parsed_count; i++) {
		int sum = 0;
		for (j = i; j < parsed_count; j++)
		{
			sum += numbers[j];
			if (sum > max) {
				max = sum;
				MaxLength = j - i + 1;
			}				
		}
	}
	return MaxLength;
}

int q_2(char arr[], int k)// i have a problem with 1,2,3,1,1,5,8 k=2.
{
	int i = 0, j = 0, indx = 0, swap = 0, Min_Swap = 0;
	int numbers[MAX_SIZE] = { 0 };
	int parse = char_to_int(arr, numbers);

	for (i = 0; i < parse; i++)
	{
		if (numbers[i] <= k)
			indx++;
	}
	for (i = 0; i < indx; i++)
	{
		if (numbers[i] > k)
			swap++;
	}
	return Min_Swap = swap;
	
	//for (i = 0, j = indx; j < parse; i++, j++)
	//{
	//	if (numbers[i] > k)
	//		swap--;
	//	if (numbers[j] > k)
	//		swap++;
	//	if (swap < Min_Swap)
	//		Min_Swap = swap; //this is ruining my code.
	//}
	//return Min_Swap;

}

int q_3(char arr[])
{
	int i = 0, j = 0, num = 0;
	int numbers[MAX_SIZE] = { 0 };
	int parse = char_to_int(arr, numbers);
	for (i = 0; i < parse; i++)
	{
		for( j = i+1;j<parse;j++)
		{
			if (numbers[i] == numbers[j])
			{
				return 1;
			}
		}
	}
	return -1;
}


int q_4(char arr[])
{
	int i = 0, j = 0, sum = 0,lowest_tower = 0, max = 0;
	int numbers[MAX_SIZE] = { 0 };
	int parse = char_to_int(arr, numbers);
	if (parse < 3) {
		return 0;
	}

	int left_max[MAX_SIZE] = { 0 };
	int right_max[MAX_SIZE] = { 0 };

	left_max[0] = numbers[0];
	for (i = 1; i < parse; i++)
	{
		if (numbers[i] > left_max[i - 1])
		{
			left_max[i] = numbers[i];
		}
		else
		{
			left_max[i] = left_max[i - 1];
		}
	}
	right_max[parse - 1] = numbers[parse - 1];
	for (i = parse - 2; i >= 0; i--)
	{
		if (numbers[i] > right_max[i + 1])
		{
			right_max[i] = numbers[i];
		}
		else
		{
			right_max[i] = right_max[i + 1];
		}
	}
	for (i = 0; i < parse; i++)
	{
		lowest_tower = fmin(left_max[i], right_max[i]);
		if (lowest_tower - numbers[i] > 0)
		{
			sum += lowest_tower - numbers[i];
		}
	}
	return sum;
}