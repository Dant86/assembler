#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"
#include "hashtable.h"
#include "parser.h"

// https://stackoverflow.com/questions/1726302/removing-spaces-from-a-string-in-c
void RemoveSpaces(char* source) {
	char* i = source;
 	char* j = source;
 	while(*j != 0) {
		*i = *j++;
		if(*i != ' ') {
			i++;
		}
	}
	*i = 0;
}

int main(int argc, char* argv[]) {
	if(argc == 1) {
		printf("No arguments provided.\n");
		exit(1);
	}

	//Add predefined symbols
	HashTable* symbolTable = mkTable();
	tableAdd(symbolTable, "SP", 0);
	tableAdd(symbolTable, "LCL", 1);
	tableAdd(symbolTable, "ARG", 2);
	tableAdd(symbolTable, "THIS", 3);
	tableAdd(symbolTable, "THAT", 4);
	char buf[5];
	char* bufCpy = malloc(sizeof(char)*5);
	for(int i = 0; i < 16; i++) {
		sprintf(buf, "R%d", i);
		strcpy(bufCpy, buf);
		tableAdd(symbolTable, bufCpy, i);
	}
	tableAdd(symbolTable, "SCREEN", 16384);
	tableAdd(symbolTable, "KBD", 24576);

	//Init some variables to use suring both passes
	FILE* asmFile = fopen(argv[1], "r");
	char line[200];
	char* lineCpy = malloc(sizeof(char) * 200);
	char binaryLine[17] = "";
	int nextAvailableAddr = 16;

	//OK, time for pass 1
	int thisLine = 0;
	char* temp = malloc(sizeof(char) * 20);
	while(fgets(line, 200, asmFile) != NULL) {
		line[strlen(line)-1] = '\0';
		strcpy(lineCpy, line);
		RemoveSpaces(line);
		if(cInst(line)) {
			thisLine += 0;
		}
		else if(lInst(line)) {
			strcpy(temp, getLabel(line, "L"));
			tableAdd(symbolTable, temp, thisLine);
			thisLine--;
		}
		else if(aInst(line)) {
			strcpy(temp, getLabel(line, "A"));
			if(tableSearch(symbolTable, temp) == -1) {
				if(isStringInt(temp)) {
					tableAdd(symbolTable, temp, atoi(temp));
				}
				else {
					tableAdd(symbolTable, temp, nextAvailableAddr);
					nextAvailableAddr++;
				}
			}
		}
		else {
			if((line[0] != '/' && line[1] != '/') && strcmp(line, "") != 0) {
				printf("Invalid syntax: %s\n", line);
				return 0;
			}
			thisLine--;
		}
		thisLine++;
		temp = malloc(sizeof(char) * 20);
	}
	fclose(asmFile);

	asmFile = fopen(argv[1], "r");
	FILE* hackFile = fopen(argv[2], "w");
	//OK, now to begin parsing in pass 2
	while(fgets(line, 200, asmFile) != NULL) {
		temp = malloc(sizeof(char) * 20);
		memset(binaryLine, 0, 16);
		line[strlen(line)-1] = '\0';
		strcpy(lineCpy, line);
		RemoveSpaces(line);
		if(cInst(line)) {
			temp = parseCInst(line, symbolTable);
			fprintf(hackFile, "%s\n", temp);
		}
		else if(aInst(line)) {
			temp = parseAInst(line, symbolTable);
			fprintf(hackFile, "%s\n", temp);
		}
		else if(lInst(line)) {
		}
		else {
			if((line[0] != '/' && line[1] != '/') && strcmp(line, "") != 0) {
				printf("Invalid syntax: %s\n", line);
				return 0;
			}
		}
	}
	fclose(asmFile);
	return 0;
}