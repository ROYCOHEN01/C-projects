#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main() {
	int i = 0;
	char a[100];
	char b[100];
	printf("enter a string: \n");
	scanf("%99s", a);
	printf("enter b string: \n");
	scanf("%99s", b);
	while (a[i] != '\0' && b[i] != '\0') {
		if (a[i] != b[i]) {
			printf("not equal\n");
			break;
		}
		i++;
		}
	if (a[i] == '\0' && b[i] == '\0')
		printf("equal\n");
	return 0;
}