#include "stringhelp.h"

void substr(char** str, size_t start, size_t end_exclude) {
	if (!str || !(*str) || start >= end_exclude || end_exclude > strlen(*str)) {
		fprintf(stderr, "Invalid substring range!\n");
		return;
	}

	size_t length = end_exclude - start;
	char* new_str = (char*)malloc((length + 1) * sizeof(char));
	if (!new_str) {
		perror("Memory allocation for substring failed!");
		return;
	}

	strncpy(new_str, *str + start, length);
	new_str[length] = '\0';

	free(*str);
	*str = new_str;
}

char* new_lexeme() {
	// Allocate a 1-byte buffer for an empty string
	char* lexeme = (char*)malloc(1);
	if (!lexeme) {
		perror("Failed to allocate lexeme buffer.");
		exit(1);
	}
	lexeme[0] = '\0'; // Ensure it starts as an empty string
	return lexeme;
}

