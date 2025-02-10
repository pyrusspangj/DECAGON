#include "filehelp.h"

char peekc(FILE* fptr) {
	if(fptr == NULL) return EOF;
	
	int c = fgetc(fptr);
	if (c != EOF) {
		ungetc(c, fptr);
	}
	return (char)c;
}

char peekn(FILE* fptr, size_t n) {
	if(fptr == NULL) return EOF;
	
	char cstack[n];
	int i;
	char c = EOF;
	
	for (i=0; i<n; ++i) {
		int temp = fgetc(fptr);
		if (temp == EOF) {
			break;
		}
		cstack[i] = (char)temp;
	}
	
	c = cstack[n - 1];
	
	while (i > 0) {
		--i;
		ungetc(cstack[i], fptr);
	}
	
	return c;
}

char* fgetn(FILE* fptr, size_t n) {
	char* fgotLUL = (char*)malloc(n + 1);
	if (!fgotLUL) return NULL;

	int c;
	for (size_t i = 0; i < n; ++i) {
		c = fgetc(fptr);
		if (c == EOF) {
			break;
		}
		fgotLUL[i] = (char)c;
	}

	fgotLUL[n] = '\0';
	return fgotLUL;
}


void ungetn(const char* str, FILE* fptr, size_t n) {
	if(strlen(str) < n) {
		n = strlen(str);
	}
	
	for(int i=n-1; i>=0; --i) {
		ungetc(str[i], fptr);
	}
}