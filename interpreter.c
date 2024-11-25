#include "interpreter.h"

LexemeType previous_lextype = NONE;

// static inlines

static inline Token string_to_token(const char *str) {
	for (int i = 0; token_map[i].str != NULL; i++) {
		if (strcmp(str, token_map[i].str) == 0) {
			return token_map[i].token;
		}
	}
	
	// implement valid literal checking
	return LITERAL;
}

static inline char peekc(FILE* fptr) {
	char c = fgetc(fptr);
	ungetc(c, fptr);
	return c;
}





// variables

int indents = 0;
size_t num_of_lexemes_thisline = 0;






// get_next_lexeme helpers
// MANAGES EASY ACCESS AND MODIFICATION OF LEXEMES ARRAY

void update_lexeme(char** str, char next) {
	size_t len = strlen((*str));
	
	char* temp = realloc((*str), sizeof(char) * len + 2);
	
	if(temp == NULL) {
		perror("Memory reallocation of lexeme failed.\n");
		exit(1);
	}
	
	*str = temp;
	
	(*str)[len] = next;
	(*str)[len+1] = '\0';
}

void insert_lexeme(Lexeme* lex, Lexeme*** lexemes, size_t len, size_t index) {	
	Lexeme** temp = realloc(*lexemes, sizeof(Lexeme*) * (len + 1));
	
	if(temp == NULL) {
		perror("Memory reallocation of lexemes INSERT_LEXEME failed.\n");
		exit(1);
	}
	
	*lexemes = temp;
	
	for(size_t i=len; i>index; --i) {
		(*lexemes)[i] = (*lexemes)[i-1];
	}
	
	(*lexemes)[index] = lex;
}

void add_lexeme(Lexeme* lex, Lexeme*** lexemes) {
	Lexeme** temp = realloc(*lexemes, sizeof(Lexeme*) * (num_of_lexemes_thisline + 1));
	
	if(temp == NULL) {
		perror("Memory reallocation of lexemes ADD_LEXEME failed.\n");
		exit(1);
	}
	
	*lexemes = temp;
	
	(*lexemes)[num_of_lexemes_thisline] = lex;
	
	// INCREMENT NUMBER OF LEXEMES THIS LINE
	++num_of_lexemes_thisline;
}





// PROCESSING LINES AND LEXEMES FROM AND FOR DECAGON FILE

char* get_next_line(FILE* fptr) {
	Lexeme** lexemes;
	int previous_num_of_indents = indents;
	
	handle_indentation(fptr, lexemes, previous_num_of_indents);
	
	do {
		add_lexeme(get_next_lexeme(fptr), &lexemes);
	} while(get_token(lexemes[num_of_lexemes_thisline-1]) != NEWLINE);
	
	
	
}

Lexeme* get_next_lexeme(FILE* fptr) {
	
	Lexeme* lex = malloc(sizeof(Lexeme));
	LexemeType lextype = NONE;
	char* lexeme;
	char next;
	bool finished = false;
	
	while((next = fgetc(fptr)) != EOF) {
		if(isspace(next)) {
			continue;
		}
		
		switch(lextype) {
			
			case NONE:
				switch(next) {
					// HANDLE COMPUTATION CASES
					case '+':
						update_lexeme(&lexeme, next);
						lextype = COMPUTATION;
						break;
					case '-':
						update_lexeme(&lexeme, next);
						lextype = COMPUTATION;
						break;
					case '*':
						update_lexeme(&lexeme, next);
						lextype = COMPUTATION;
						break;
					case '/':
						update_lexeme(&lexeme, next);
						lextype = COMPUTATION;
						break;
					case '%':
						update_lexeme(&lexeme, next);
						lextype = COMPUTATION;
						break;
					case '=':
						update_lexeme(&lexeme, next);
						lextype = COMPUTATION;
						break;
					case '!':
						update_lexeme(&lexeme, next);
						lextype = COMPUTATION;
						break;
					case '>':
						update_lexeme(&lexeme, next);
						lextype = COMPUTATION;
						break;
					case '<':
						update_lexeme(&lexeme, next);
						lextype = COMPUTATION;
						break;
					
					
					case IDENTIFIER_MODIFIER:
						update_lexeme(&lexeme, next);
						lextype = IDENTIFIER;
						break;
						
					default:
						update_lexeme(&lexeme, next);
						break;
				}
				break;
				
			case RESERVED:
			break;
			case IDENTIFIER:
			break;
			case COMPUTATION:
				switch(next) {
					case '+':
						update_lexeme(&lexeme, next);
						finished = true;
						break;
					case '-':
						update_lexeme(&lexeme, next);
						finished = true;
						break;
					case '*':
						update_lexeme(&lexeme, next);
						finished = true;
						break;
					case '/':
						update_lexeme(&lexeme, next);
						finished = true;
						break;
					case '=':
						update_lexeme(&lexeme, next);
						finished = true;
						break;
				}
				break;
			case CONSTLITERAL:
				break;
		}
		
		if(finished) {
			set_lexeme(lex, lexeme);
			assign_token_from_finished_lexeme(lex, lextype);
		}
	}
}







// Lexeme Getters

Token get_token(Lexeme* lex) {
	return lex->token;
}

char* get_lexeme(Lexeme* lex) {
	return lex->lexeme;
}







// Lexeme Setters

Lexeme* make_lexeme(Token t, char* lexeme) {
	Lexeme* lex = malloc(sizeof(Lexeme));
	
	if(lex == NULL) {
		perror("Memory allocation failed making a new lexeme.\n");
		exit(1);
	}
	
	build_lexeme(lex, t, lexeme);
	
	return lex;
}

void build_lexeme(Lexeme* lex, Token t, char* str) {
	set_token(lex, t);
	set_lexeme(lex, str);
}

void set_token(Lexeme* lex, Token t) {
	lex->token = t;
}

void set_lexeme(Lexeme* lex, char* str) {
	strcpy(lex->lexeme, str);
}

void assign_token_from_finished_lexeme(Lexeme* lex, LexemeType lextype) {
	
	set_token(lex, string_to_token(get_lexeme(lex)));
	
}







// helpers

bool is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_escape_char(char c) {
	return c == '\t' || c == '\n' || c == '\r';
}








// handlers

void handle_indentation(FILE* fptr, Lexeme** lexemes, int previous_indents) {
	indents = 0;
	while(peekc(fptr) == '\n') {
		++indents;
	}
	
	if(indents < previous_indents) {
		add_lexeme(make_lexeme(CLOSECURLY, "}"), &lexemes);
	}
}

char handle_escape_character(char c) {
	if(c == '\t') {
		++indents;
		return '\t';
	} else if(c == '\n') {
		indents = 0;
	}
}