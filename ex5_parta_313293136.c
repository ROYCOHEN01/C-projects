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

Node* EvenOdd(Node* head);


int main()
{
    Node* head = NULL;

    int number_imput = 0;

    printf("Original Linked list:\n");
    scanf("%d", &number_imput);
    while (number_imput >= 0)
    {
        head = push(number_imput, head);
        scanf("%d", &number_imput);
    }

    head = EvenOdd(head);

    printf("Modified Linked list:\n");
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



Node* EvenOdd(Node* head)
{
    Node* Evenhead = NULL;
    Node* Oddhead = NULL;
    Node* EvenList = Evenhead;
    Node* OddList = Oddhead;
    Node* NewList = NULL;

    while (head != NULL)
    {
        if (head->value % 2 == 0)
        {
            EvenList = push(head->value, EvenList);
            head = head->next;
        }
        else if (head->value % 2 != 0)
        {
            OddList = push(head->value, OddList);
            head = head->next;
        }
    }

    if (EvenList == NULL) {
        Oddhead = OddList;
        while (Oddhead != NULL) {
            NewList = push(Oddhead->value, NewList);
            Oddhead = Oddhead->next;
        }
        return NewList;

    }
    if (OddList == NULL) {
        Evenhead = EvenList;
        while (Evenhead != NULL) {
            NewList = push(Evenhead->value, NewList);
            Evenhead = Evenhead->next;
        }
        return NewList;

    }
    Oddhead = OddList;
    while (Oddhead != NULL) {
        NewList = push(Oddhead->value, NewList);
        Oddhead = Oddhead->next;
    }
    Evenhead = EvenList;
    while (Evenhead != NULL) {
        NewList = push(Evenhead->value, NewList);
        Evenhead = Evenhead->next;
    }
    return NewList;
}
