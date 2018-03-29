#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

typedef struct hashtable {
	List* buckets[100];
} HashTable;

HashTable* mkTable();

int hash(const char* key);

void tableAdd(HashTable* t, const char* key, int value);

int tableSearch(HashTable* t, const char* key);

List* entries(const HashTable* t);

#endif