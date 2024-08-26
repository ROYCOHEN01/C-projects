#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int value;
    struct node* next;
}Node;

void print_list(Node const* head);

Node* allocate_node(int value, Node* next);

Node* push(int value, Node* head);

void free_list(Node* head);

void NoDuplicates(struct node* head);

int main()
{
    Node* head = NULL;

    int number_imput = 0;

    printf("Linked list before duplicate removal:\n");
    scanf("%d", &number_imput);
    while (number_imput >= 0)
    {
        head = push(number_imput, head);
        scanf("%d", &number_imput);
    }

    NoDuplicates(head);

    printf("Linked list after duplicate removal:\n");
    print_list(head);
    free_list(head);


    return 0;
}


void print_list(Node const* head)
{
    for (; head; head = head->next)
        printf("%d ", head->value);
    printf("\n");
}

Node* allocate_node(int value, Node* next)
{
    Node* n = malloc(sizeof * n);
    if (!n) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    n->value = value;
    n->next = next;
    return n;
}

Node* push(int value, Node* head)
{
    return allocate_node(value, head);
}

void free_list(Node* head)
{
    while (head) {
        Node* old = head;
        head = head->next;
        free(old);
    }
}

void NoDuplicates(struct node* head)
{
    Node* current_node = head;
    Node* next_node = head->next;
    Node* temp = NULL;
    if (head == NULL)
		return;
    while (current_node->next != NULL)
	{
		if (current_node->value == next_node->value)
		{
			temp = next_node->next;
			free(next_node); 
            current_node->next = temp;
            next_node = temp;
		}
		else
		{
            current_node = next_node;
            next_node = next_node->next;
		}
	}
    return head;
}