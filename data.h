#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define DICTIONARY_ROWS 16
#define DICTIONARY_COLS 16

typedef struct {
	char* key;
	void* value;
} KVPair;

typedef struct {
	KVPair kvpairs[DICTIONARY_ROWS][DICTIONARY_COLS];
} LexemeDictionary;