#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char peekc(FILE* fptr);
char peekn(FILE* fptr, size_t n);
char* fgetn(FILE* fptr, size_t n);
void ungetn(const char* str, FILE* fptr, size_t n);
