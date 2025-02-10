#include "interpreter.h"

#include "Helper/filehelp.h"
#include "Helper/stringhelp.h"

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


static inline Lexeme* find_word(const char* str, FILE* fptr, Lexeme* lex, char** lexeme, LexemeType* lextype, LexemeType set_to) {
	//printf("1. attempting to find %s\n", str);
	
	short forward = strlen(str) - 1;

	char workable_str[forward + 1];  
	for (int i = 0; i < forward; ++i) {
		workable_str[i] = str[i + 1];
	}
	workable_str[forward] = '\0';

	//printf("3. attempting to find %s\n", workable_str);
	
	char* placeholder = fgetn(fptr, forward);
	if (!placeholder) {
		fprintf(stderr, "Error: fgetn() failed\n");
		return NULL;
	}

	//printf("4. comparing %s to %s\n", placeholder, workable_str);
	
	if (strcmp(placeholder, workable_str) == 0) {
		//printf("5. updating lexeme %s to %s\n", *lexeme, str);	

		if (*lexeme) {
			printf("len of lexeme: %d\n", (int)strlen(*lexeme));
			free(*lexeme);
			*lexeme = NULL;
		}

		*lexeme = (char*)malloc(strlen(str) + 1);
		if (!*lexeme) {
			fprintf(stderr, "Memory allocation failed!\n");
			free(placeholder);
			return NULL;
		}

		//printf("BRUHBRUH BRUH ALLOCATED YES!!!\n");

		strcpy(*lexeme, str);

		//printf("6. lexeme set to %s\n", *lexeme);

		*lextype = set_to;

		//printf("%s FOUND! RETURNING ITS LEXEME!\n", str);

		free(placeholder);
		return done(lex, *lexeme);
	}
	
	ungetn(placeholder, fptr, forward);
	
	free(placeholder);
	return NULL;
}

static inline LexemeType find_reserved_word(char next, FILE* fptr, Lexeme** lex, char** lexeme) {
	
	Lexeme* temp = NULL;
	LexemeType lextype = NONE;

	switch(next) {
	
		case 'a':
						
			temp = find_word("and", fptr, *lex, &lexeme, &lextype, CONDITIONAL);
			if(temp != NULL) {
				*lex = temp;
				return lextype;
			}
			
			temp = find_word("area", fptr, *lex, &lexeme, &lextype, RESERVED);
			if(temp != NULL) {
				*lex = temp;
				return lextype;
			}
			
			break;
					
		case 'i':
			
			temp = find_word("if", fptr, *lex, &lexeme, &lextype, RESERVED);
			if(temp != NULL) {
				*lex = temp;
				return lextype;
			}
			
			break;
			
		case 'n':
				
			temp = find_word("not", fptr, lex, &lexeme, &lextype, RESERVED);
			if(temp != NULL) {
				*lex = temp;
				return lextype;	
			}
			
			break;
					
		case 's':
						
			temp = find_word("set", fptr, lex, &lexeme, &lextype, RESERVED);
			if(temp != NULL) {
				*lex = temp;
				return lextype;
			}
						
			break;
			
		default:
				
			break;
	
	}
	
	return NONE;

}










// variables

int indents = 0;
size_t num_of_lexemes_thisline = 0;






// get_next_lexeme helpers
// MANAGES EASY ACCESS AND MODIFICATION OF LEXEMES ARRAY

void assign_lexeme(char** str, const char* value) {
	size_t len = strlen(value) + 1;
	char* temp = realloc(*str, len);
	if (!temp) {
		perror("Memory allocation failed in assign_lexeme");
		exit(1);
	}

	*str = temp;
	strcpy(*str, value);
}


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
	
	//printf("IN GET_NEXT_LEXEME about to init data\n");
	
	Lexeme* lex = (Lexeme*)malloc(sizeof(Lexeme));
	//printf("made lex\n");
	
	LexemeType lextype = NONE;
	//printf("made lextype\n");
	
	char* lexeme = new_lexeme();
	//printf("made lexeme\n");
	
	char next;
	bool finished = false;
	
	printf("IN GET_NEXT_LEXEME about to enter while loop\n");
	
	while((next = fgetc(fptr)) != EOF) {
		printf("next!: '%c' (ASCII: %d)\n", next, next);
		printf("lexeme: %s\n", lexeme);
		
		if(isspace(next)) {
			
			if(lextype != NONE) {
				return done(lex, lexeme);
			}
			
			if(next == '\n') {
				if(lextype == NONE) {
					printf("is newline! returning\n");
					update_lexeme(&lexeme, next);
				} else {
					ungetc(next, fptr);
				}
				
				return done(lex, lexeme);
			}
			
			printf("is space, continuing\n");
			continue;
		}
		
		switch(lextype) {
			
			case NONE:
				switch(next) {
					// HANDLE COMPUTATION CASES
					case '+':
					case '-':
					case '*':
					case '/':
					case '%':
					case '=':
					case '!':
					case '>':
					case '<':
						update_lexeme(&lexeme, next);
						lextype = COMPUTATION;
						break;
					
					
					case IDENTIFIER_MODIFIER:
						update_lexeme(&lexeme, next);
						lextype = IDENTIFIER;
						break;
						
					case EMPHASIS_MODIFIER:
						return done(lex, lexeme);
						
					
					default:
						
						lextype = find_reserved_word(next, fptr, lex, lexeme);
						
						if(lextype != NONE) {
							return lex;
						}
						
						break;
				}
				break;
				
			case RESERVED:
			break;
			case IDENTIFIER:
				update_lexeme(&lexeme, next);
				break;
			case COMPUTATION:
				switch(next) {
					case '+':
					case '-':
					case '*':
					case '/':
					case '=':
						update_lexeme(&lexeme, next);
						
					default:
						return done(lex, lexeme);
				}
				break;
			case CONSTLITERAL:
				break;
			
			default:
				return done(lex, lexeme);
		}
		
	}
	
	return done(lex, "DONE");
	
}



Lexeme** get_all_lexemes(FILE* fptr) {
    printf("Starting get_all_lexemes()\n");

    Lexeme** all_lexemes = malloc(sizeof(Lexeme*));
    if (!all_lexemes) {
        perror("Memory allocation failed for all_lexemes");
        exit(1);
    }

    Lexeme* next_lexeme;
    do {
        printf("Fetching next lexeme...\n");
        next_lexeme = get_next_lexeme(fptr);
        
        if (!next_lexeme) {
            printf("Error: get_next_lexeme() returned NULL!\n");
            break;
        }

        printf("Lexeme received: %s\n", get_lexeme(next_lexeme));
        add_lexeme(next_lexeme, &all_lexemes);
    } while (get_token(next_lexeme) != DONE);

    printf("Finished lexeme processing.\n");
    return all_lexemes;
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
    lex->lexeme = malloc(strlen(str) + 1);
    if (!lex->lexeme) {
        perror("Memory allocation failed for lexeme");
        exit(1);
    }
    strcpy(lex->lexeme, str);
}

void assign_token_from_finished_lexeme(Lexeme* lex) {
	
	set_token(lex, string_to_token(get_lexeme(lex)));
	
}

Lexeme* done(Lexeme* lex, const char* lexeme) {
	set_lexeme(lex, lexeme);
	assign_token_from_finished_lexeme(lex);
	
	return lex;
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