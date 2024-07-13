//divide sentence to words by pointers
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
	char sentence[100];
	char* p;
	printf("Enter a sentence: \n");
	fgets(sentence, sizeof(sentence), stdin);
	p = sentence;
	while (*p != '\0') {
		if (*p == ' ')
			printf("\n");
		else
			printf("%c", *p);
		p++;
	}
	return 0;
}