#include <stdlib.h>
#include <stdio.h>

#include "interpreter.h"

void decagon(FILE* decf) {
		
	Lexeme** program_lexemes = get_all_lexemes(decf);
	
}

int main(int argc, char* argv[]) {
	if(argc == 1) {
		printf("argc length is ONE (1); Input file name has not been provided.\n");
		return 1;
	}
	
	FILE* decagonfile;
	decagonfile = fopen(argv[1], "r");
	
	if(decagonfile == NULL) {
		printf("Filename argument \"%s\" is invalid or could not be read.\n", argv[1]);
	}
	
	decagon(decagonfile);
	
	fclose(decagonfile);
	
	return 0;
}