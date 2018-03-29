#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

Node* mkNode(const char* key, int newVal) {
	if(key == NULL) {
		return NULL;
	}
	Node* n = malloc(sizeof(Node));
	n -> key = malloc(sizeof(char*) * strlen(key));
	strcpy(n -> key, key);
	n -> value = newVal;
	n -> next = NULL;
	return n;
}

List* mkList() {
	List* l = malloc(sizeof(List));
	l -> head = NULL;
	l -> tail = NULL;
	l -> length = 0;
	return l;
}

void prepend(List* l, Node* n) {
	if(l == NULL || n == NULL) {
		printf("Failed to add node to list.\n");
		return;
	}
	if(l -> length == 0) {
		l -> length++;
		l -> head = n;
		l -> tail = n;
		return;
	}
	l -> length++;
	n -> next = l -> head;
	l -> head = n;
	return;
}

void append(List* l, Node* n) {
	if(l == NULL || n == NULL) {
		printf("Failed to add node to list.\n");
		return;
	}
	if(l -> length == 0) {
		l -> length++;
		l -> head = n;
		l -> tail = n;
		return;
	}
	l -> length++;
	l -> tail -> next = n;
	l -> tail = n;
	return;
}

void add(List* l, Node* n, int index) {
	if(l == NULL || n == NULL) {
		printf("Failed to add node to list.\n");
		return;
	}
	if(index < 0 || index > l -> length) {
		printf("Failed to add node to list.\n");
		return;
	}
	if(index == 0) {
		prepend(l, n);
		return;
	}
	if(index == l -> length) {
		append(l, n);
		return;
	}
	if(l -> length == 0) {
		printf("Failed to add node to list.\n");
		return;
	}
	l -> length++;
	Node* temp = l -> head;
	for(int i = 0; i < index - 1; i++) {
		temp = temp -> next;
	}
	n -> next = temp -> next;
	temp -> next = n;
}

void set(List* l, Node* n, int index) {
	if(l == NULL || n == NULL) {
		printf("Failed to add node to list.\n");
		return;
	}
	if(l -> length == 0) {
		printf("Failed to add node to list.\n");
		return;
	}
	if(index < 0 || index >= l -> length) {
		printf("Failed to add node to list.\n");
		return;
	}
	Node* temp = l -> head;
	for(int i = 0; i < index; i++) {
		temp = temp -> next;
	}
	temp -> value = n -> value;
}

void rmFront(List* l) {
	if(l == NULL) {
		return;
	}
	l -> length--;
	l -> head = l -> head -> next;
}

void rmBack(List* l) {
	if(l == NULL) {
		return;
	}
	l -> length--;
	Node* temp = l -> head;
	for(int i = 0; i < l -> length - 1; i++) {
		temp = temp -> next;
	}
	temp -> next = NULL;
}

void rmList(List* l, int index) {
	if(l == NULL) {
		return;
	}
	if(l -> length == 0) {
		printf("Failed to remove node from list.\n");
		return;
	}
	if(index < 0 || index >= l -> length) {
		printf("Failed to remove node from list.\n");
		return;
	}
	Node* temp = l -> head;
	for(int i = 0; i < index - 1; i++) {
		temp = temp -> next;
	}
	temp -> next = NULL;
}

Node* keyFind(List* l, const char* key) {
	if(l == NULL || key == NULL) {
		return NULL;
	}
	char* key_ = strdup(key);
	Node* temp = l -> head;
	while(temp != NULL) {
		if(strcmp(key_, temp -> key) == 0) {
			return temp;
		}
		temp = temp -> next;
	}
	return NULL;
}

Node* indexFind(List* l, int index) {
	if(l == NULL) {
		return NULL;
	}
	if(index < 0 || index >= l -> length) {
		printf("Failed to remove node from list.\n");
		return NULL;
	}
	Node* temp = l -> head;
	for(int i = 0; i < index; i++) {
		temp = temp -> next;
	}
	return temp;
}
