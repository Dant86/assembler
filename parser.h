#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashtable.h"

#ifndef PARSER_H
#define PARSER_H

int cInst(char* line);

int lInst(char* line);

int aInst(char* line);

char* getLabel(char* line, char* instrType);

char* parseCInst(char* line, HashTable* symbolTable);

char* parseAInst(char* line, HashTable* symbolTable);

int isStringInt(char* line);

int dec_to_bin(int dec);

char* itoa(int num, char* str, int base);

char* mkBinary16(char* bin);

char* getDest(char* line);

char* getComp(char* line);

char* getJump(char* line);

#endif