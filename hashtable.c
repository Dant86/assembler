#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"
#include "hashtable.h"

HashTable* mkTable() {
	HashTable* t = malloc(sizeof(HashTable));
	for(int i = 0; i < 100; i++) {
		t -> buckets[i] = mkList();
	}
	return t;
}

int hash(const char* key) {
	if(key == NULL) {
		printf("NULL key.\n");
		return -1;
	}
	char* cpy = strdup(key);
	unsigned long res = 17;
	for(int i = 0; i < strlen(cpy); i++) {
		res = (37 * res) + (int)cpy[i];
	}
	return res % 100;
}

void tableReplace(HashTable* t, const char* key, int value) {
	Node* n = mkNode(strdup(key), value);
	int bucket = hash(key);
	List* curr = t -> buckets[bucket];
	Node* temp = curr -> head;
	while(temp != NULL) {
		if(strcmp(key, temp -> key) == 0) {
			temp -> value = value;
		}
		temp = temp -> next;
	}
}

void tableAdd(HashTable* t, const char* key, int value) {
	if(t == NULL || key == NULL) {
		printf("Failed to add k/v pair to hashtable.\n");
		return;
	}

	Node* n = mkNode(strdup(key), value);
	int bucket = hash(key);
	List* curr = t -> buckets[bucket];
	if(keyFind(curr, n -> key) != NULL) {
		tableReplace(t, key, value);
		return;
	}
	append(curr, n);
}

int tableSearch(HashTable* t, const char* key) {
	if(t == NULL || key == NULL) {
		return -1;
	}
	int bucket = hash(strdup(key));
	List* curr = t -> buckets[bucket];
	Node* found = keyFind(curr, strdup(key));
	if(found == NULL) {
		return -1;
	}
	return found -> value;
}

List* entries(const HashTable* t) {
	if(t == NULL) {
		return NULL;
	}
	List* ret = mkList();
	Node* cNode = NULL;
	for(int i = 0; i < 100; i++) {
		cNode = t -> buckets[i] -> head;
		while(cNode != NULL) {
			append(ret, mkNode(cNode -> key, cNode -> value));
			cNode = cNode -> next;
		}
	}
	return ret;
}