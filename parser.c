#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashtable.h"
/*
	implementation of reverse and itoa found here:
	https://www.geeksforgeeks.org/implement-itoa/
*/

int cInst(char* line) {
	return (strstr(line, ";") != NULL || strstr(line, "=") != NULL);
}

int lInst(char* line) {
	return (line[0] == '(');
}

int aInst(char* line) {
	return (line[0] == '@');
}

char* getLabel(char* line, char* instrType) {
	if(line == NULL || instrType == NULL) {
		return NULL;
	}
	int len_subStr;
	if(strcmp(instrType, "L") == 0) {
		len_subStr = strlen(line) - 2;
	}
	else if(strcmp(instrType, "A") == 0) {
		len_subStr = strlen(line) - 1;
	}
	else {
		return NULL;
	}
	char subString[len_subStr + 1];
	for(int i = 0; i < len_subStr; i++) {
		subString[i] = line[i + 1];
	}
	subString[len_subStr] = '\0';
	char* ssCpy = malloc(sizeof(char) * 20);
	strcpy(ssCpy, subString);
	return ssCpy;
}

int isStringInt(char* line) {
	for(int i = 0; i < strlen(line); i++) {
		if(!isdigit(line[i])) {
			return 0;
		}
	}
	return 1;
}

void reverse(char str[], int length) {
	int start = 0;
	int end = length -1;
	char temp = 'f';
	while (start < end) {
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

char* itoa(int num, char* str, int base) {
	int i = 0;
	int isNegative = 0;
	/* Handle 0 explicitly, otherwise empty string is printed for 0 */
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}
	// In standard itoa(), negative numbers are handled only with 
	// base 10. Otherwise numbers are considered unsigned.
	if (num < 0 && base == 10) {
		isNegative = 1;
		num = -num;
	}
	// Process individual digits
	while (num != 0) {
		int rem = num % base;
		str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
		num = num/base;
	}
	// If number is negative, append '-'
	if (isNegative)
		str[i++] = '-';
 
	str[i] = '\0'; // Append string terminator
	// Reverse the string
	reverse(str, i);
	return str;
}

char* mkBinary16(char* bin) {
	int length = strlen(bin);
	char res[17] = "";
	res[16] = '\0';
	int offset = 16 - length;
	for(int i = 0; i < offset; i++) {
		res[i] = '0';
	}
	for(int i = 0; i < length; i++) {
		res[offset + i] = bin[i];
	}
	char* retVal = malloc(sizeof(char)*20);
	strcpy(retVal, res);
	return retVal;
}

char* parseAInst(char* line, HashTable* symbolTable) {
	char* label = malloc(sizeof(char) * 20);
	label = getLabel(line, "A");
	int value = tableSearch(symbolTable, label);
	char* stredVal = malloc(sizeof(char)*17);
	itoa(value, stredVal, 2);
	return mkBinary16(stredVal);
}

char* getDest(char* line) {
	//IN PROGRESS
	if(strstr(line, "=") == NULL) {
		return NULL;
	}
	int index_of_equals = strchr(line, '=') - line;
	char res[index_of_equals + 1];
	memset(res, 0, (index_of_equals + 1) * sizeof(char));
	res[index_of_equals] = '\0';
	for(int i = 0; i < index_of_equals; i++) {
		res[i] = line[i];
	}
	char* retVal = malloc(sizeof(char) * 5);
	strcpy(retVal, res);
	return retVal;
}

char* getComp(char* line) {
	int start = 0;
	int end = strlen(line);
	if(strstr(line, "=") != NULL) {
		start = strchr(line, '=') - line + 1;
	}
	if(strstr(line, ";") != NULL) {
		end = strchr(line, ';') - line;
	}
	char res[end - start + 1];
	memset(res, 0, (end-start+1)*sizeof(char));
	res[end - start] = '\0';
	for(int i = start; i < end; i++) {
		res[i - start] = line[i];
	}
	char* retVal = malloc(sizeof(char)*6);
	strcpy(retVal, res);
	return retVal;
}

char* getJump(char* line) {
	if(strstr(line, ";") == NULL) {
		return NULL;
	}
	int index_semi = strchr(line, ';') - line;
	char res[4] = "";
	res[3] = '\0';
	for(int i = 1; i < 4; i++) {
		res[i-1] = line[i+index_semi];
	}
	char* retVal = malloc(sizeof(char)*5);
	strcpy(retVal, res);
	return retVal;
}

char* parseCInst(char* line, HashTable* symbolTable) {
	// char* symbols[] = {"0", "1", "-1", "D", "A", "!D", "!A", "D+1",
	// 				   "A+1", "D-1", "A-1", "D+A", "D-A", "A-D", "D&A",
	// 				   "D|A", "M", "!M", "-M", "M+1", "M-1", "D+M", "D-M",
	// 				   "M-D", "D&M", "D|M"};
	// char* symOut[] = {"101010", "111111", "111010", "001100", "110000",
 //                      ""}
	char res[17] = "";
	res[16] = '\0';
	res[0] = '1';
	res[1] = '1';
	res[2] = '1';
	//Handle dest
	res[10] = '0';
	res[11] = '0';
	res[12] = '0';
	char* dest = getDest(line);
	if(dest != NULL) {
		if(strstr(dest, "A") != NULL) {
			res[10] = '1';
		}
		if(strstr(dest, "M") != NULL) {
			res[12] = '1';
		}
		if(strstr(dest, "D") != NULL) {
			res[11] = '1';
		}
	}
	//Handle comp
	char* comp = getComp(line);
	res[3] = '0';
	if(strstr(comp, "M") != NULL) {
		res[3] = '1';
	}
	if(strstr(comp, "0") != NULL) {
		res[4] = '1';
		res[5] = '0';
		res[6] = '1';
		res[7] = '0';
		res[8] = '1';
		res[9] = '0';
	}
	if(strstr(comp, "1") != NULL) {
		res[4] = '1';
		res[5] = '1';
		res[6] = '1';
		res[7] = '1';
		res[8] = '1';
		res[9] = '1';
	}
	if(strstr(comp, "-1") != NULL) {
		res[4] = '1';
		res[5] = '1';
		res[6] = '1';
		res[7] = '0';
		res[8] = '1';
		res[9] = '0';
	}
	if(strstr(comp, "D") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '1';
		res[7] = '1';
		res[8] = '0';
		res[9] = '0';
	}
	if(strstr(comp, "A") != NULL) {
		res[4] = '1';
		res[5] = '1';
		res[6] = '0';
		res[7] = '0';
		res[8] = '0';
		res[9] = '0';
	}
	if(strstr(comp, "M") != NULL) {
		res[4] = '1';
		res[5] = '1';
		res[6] = '0';
		res[7] = '0';
		res[8] = '0';
		res[9] = '0';
	}
	if(strstr(comp, "!D") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '1';
		res[7] = '1';
		res[8] = '0';
		res[9] = '1';
	}
	if(strstr(comp, "!A") != NULL) {
		res[4] = '1';
		res[5] = '1';
		res[6] = '0';
		res[7] = '0';
		res[8] = '0';
		res[9] = '1';
	}
	if(strstr(comp, "-D") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '1';
		res[7] = '1';
		res[8] = '1';
		res[9] = '1';
	}
	if(strstr(comp, "-A") != NULL) {
		res[4] = '1';
		res[5] = '1';
		res[6] = '0';
		res[7] = '0';
		res[8] = '1';
		res[9] = '1';
	}
	if(strstr(comp, "D+1") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '1';
		res[7] = '1';
		res[8] = '1';
		res[9] = '0';
	}
	if(strstr(comp, "A+1") != NULL) {
		res[4] = '1';
		res[5] = '1';
		res[6] = '0';
		res[7] = '1';
		res[8] = '1';
		res[9] = '1';
	}
	if(strstr(comp, "A-1") != NULL) {
		res[4] = '1';
		res[5] = '1';
		res[6] = '0';
		res[7] = '0';
		res[8] = '1';
		res[9] = '0';
	}
	if(strstr(comp, "M-1") != NULL) {
		printf("%s\n", comp);
		res[4] = '1';
		res[5] = '1';
		res[6] = '0';
		res[7] = '0';
		res[8] = '1';
		res[9] = '0';
	}
	if(strstr(comp, "D+A") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '0';
		res[7] = '0';
		res[8] = '1';
		res[9] = '0';
	}
	if(strstr(comp, "A+D") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '0';
		res[7] = '0';
		res[8] = '1';
		res[9] = '0';
	}
	if(strstr(comp, "D-A") != NULL) {
		res[4] = '0';
		res[5] = '1';
		res[6] = '0';
		res[7] = '0';
		res[8] = '1';
		res[9] = '1';
	}
	if(strstr(comp, "A-D") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '0';
		res[7] = '1';
		res[8] = '1';
		res[9] = '1';
	}
	if(strstr(comp, "D&A") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '0';
		res[7] = '0';
		res[8] = '0';
		res[9] = '0';
	}
	if(strstr(comp, "A&D") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '0';
		res[7] = '0';
		res[8] = '0';
		res[9] = '0';
	}
	if(strstr(comp, "D|A") != NULL) {
		res[4] = '0';
		res[5] = '1';
		res[6] = '0';
		res[7] = '1';
		res[8] = '0';
		res[9] = '1';
	}
	if(strstr(comp, "A|D") != NULL) {
		res[4] = '0';
		res[5] = '1';
		res[6] = '0';
		res[7] = '1';
		res[8] = '0';
		res[9] = '1';
	}
	if(strstr(comp, "-M") != NULL) {
		res[4] = '1';
		res[5] = '1';
		res[6] = '0';
		res[7] = '0';
		res[8] = '1';
		res[9] = '1';
	}
	if(strstr(comp, "M+1") != NULL) {
		res[4] = '1';
		res[5] = '1';
		res[6] = '0';
		res[7] = '1';
		res[8] = '1';
		res[9] = '1';
	}

	if(strstr(comp, "D+M") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '0';
		res[7] = '0';
		res[8] = '1';
		res[9] = '0';
	}
	if(strstr(comp, "M+D") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '0';
		res[7] = '0';
		res[8] = '1';
		res[9] = '0';
	}
	if(strstr(comp, "D-M") != NULL) {
		res[4] = '0';
		res[5] = '1';
		res[6] = '0';
		res[7] = '0';
		res[8] = '1';
		res[9] = '1';
	}
	if(strstr(comp, "M-D") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '0';
		res[7] = '1';
		res[8] = '1';
		res[9] = '1';
	}
	if(strstr(comp, "D&M") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '0';
		res[7] = '0';
		res[8] = '0';
		res[9] = '0';
	}
	if(strstr(comp, "M&D") != NULL) {
		res[4] = '0';
		res[5] = '0';
		res[6] = '0';
		res[7] = '0';
		res[8] = '0';
		res[9] = '0';
	}
	if(strstr(comp, "D|M") != NULL) {
		res[4] = '0';
		res[5] = '1';
		res[6] = '0';
		res[7] = '1';
		res[8] = '0';
		res[9] = '1';
	}
	if(strstr(comp, "M|D") != NULL) {
		res[4] = '0';
		res[5] = '1';
		res[6] = '0';
		res[7] = '1';
		res[8] = '0';
		res[9] = '1';
	}
	//Handle jump
	char* jump = getJump(line);
	res[13] = '0';
	res[14] = '0';
	res[15] = '0';
	if(jump != NULL) {
		if(strstr(jump, "L") != NULL) {
			res[13] = '1';
		}
		if(strstr(jump, "E") != NULL) {
			res[14] = '1';
		}
		if(strstr(jump, "G") != NULL) {
			res[15] = '1';
		}
		if(strcmp(jump, "JMP") == 0) {
			res[13] = '1';
			res[14] = '1';
			res[15] = '1';
		}
	}
	char* retVal = malloc(sizeof(char) * 18);
	strcpy(retVal, res);
	return retVal;
}
