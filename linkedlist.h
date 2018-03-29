#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	char* key;
	int value;
	struct node* next;
} Node;

Node* mkNode(const char* key, int newVal);

typedef struct list {
	Node* head;
	Node* tail;
	int length;
} List;

List* mkList();

void prepend(List* l, Node* n);

void append(List* l, Node* n);

void add(List* l, Node* n, int index);

void set(List* l, Node* n, int index);

void rmFront(List* l);

void rmBack(List* l);

void rmList(List* l, int index);

Node* keyFind(List* l, const char* key);

Node* indexFind(List* l, int index);

#endif