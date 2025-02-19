#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define IDENTIFIER_MODIFIER '$'
#define EMPHASIS_MODIFIER '@'
#define IDENTIFIER_MODIFIER_STR "$"
#define EMPHASIS_MODIFIER_STR "@"

typedef enum {
	// actions
	INDENT, NEWLINE,
	
	// reserved words
	SET, IF, ELSE, WHILE, FOR, DEF, AREA,
	LAST, FIRST, OG, GOTO,
	
	// reserved method call words
	PRINT, PRINTLN, 
	
	// action tokens & literal
	IDENT, EMPH, LITERAL,
	
	// computations
	ADD, SUB, MUL, EXPMUL, DIV, FLRDIV, MOD,
	ADDADD, SUBSUB,
	ASSOPR, ADDASSOPR, SUBASSOPR, MULASSOPR, DIVASSOPR, MODASSOPR,
	EQL, NEQ, GRTTHN, LSSTHN, GRTTHNEQL, LSSTHNEQL, 
	
	// conditionals
	NOT, AND, OR, XOR, 
	
	// single chars
	COLON, DOT, COMMA,
	OPENPAREN, CLOSEPAREN, OPENBRACKET, CLOSEBRACKET, OPENCURLY, CLOSECURLY,
	
	DONE, ERROR, UNKNOWN,
} Token;

typedef enum {
	ERR,
	NONE,
	END,
	
	RESERVED,
	
	IDENTIFIER,
	EMPHASIZER,
	
	COMPUTATION,
	CONDITIONAL,
	
	CONSTLITERAL,
	SINGLE_CHAR
} LexemeType;


// TOKEN MAPPING
typedef struct {
	const char *str;
	Token token;
} TokenMap;

static const TokenMap token_map[] = {
	{"DONE", DONE},
	
	{"\t", INDENT},
	{"\n", NEWLINE},
	{"set", SET},
	{"if", IF},
	{"else", ELSE},
	{"while", WHILE},
	{"for", FOR},
	{"def", DEF},
	{"area", AREA},
	{"print", PRINT},
	{"println", PRINTLN},
	
	{IDENTIFIER_MODIFIER_STR, IDENT},
	{EMPHASIS_MODIFIER_STR, EMPH},
	
	{"+", ADD},
	{"-", SUB},
	{"*", MUL},
	{"**", EXPMUL},
	{"/", DIV},
	{"//", FLRDIV},
	{"%", MOD},
	{"++", ADDADD},
	{"--", SUBSUB},
	{"=", ASSOPR},
	{"+=", ADDASSOPR},
	{"-=", SUBASSOPR},
	{"*=", MULASSOPR},
	{"/=", DIVASSOPR},
	{"%=", MODASSOPR},
	{"==", EQL},
	{"!=", NEQ},
	{">", GRTTHN},
	{"<", LSSTHN},
	{">=", GRTTHNEQL},
	{"<=", LSSTHNEQL},
	{"!", NOT},
	{"and", AND},
	{"or", OR},
	{"xor", XOR},
	{":", COLON},
	{".", DOT},
	{",", COMMA},
	{"(", OPENPAREN},
	{")", CLOSEPAREN},
	{"[", OPENBRACKET},
	{"]", CLOSEBRACKET},
	{"{", OPENCURLY},
	{"}", CLOSECURLY},
	{NULL, LITERAL}
};

static inline Token string_to_token(const char* str);
static inline char* token_to_string(Token t);

typedef struct {
	Token token;
	char* lexeme;
} Lexeme;

static inline Lexeme* find_word(const char* str, FILE* fptr, Lexeme* lex, char** lexeme, LexemeType* lextype, LexemeType set_to);
static inline LexemeType find_reserved_word(char next, FILE* fptr, Lexeme** lex, char** lexeme);


// updaters and operations

void assign_lexeme(char** str, const char* value);
void update_lexeme(char** str, char c);
void insert_lexeme(Lexeme* lex, Lexeme*** lexemes, size_t len, size_t index);
void add_lexeme(Lexeme* lex, Lexeme*** lexemes);


// getters
char* get_next_line(FILE* fptr);
Lexeme* get_next_lexeme(FILE* fptr);
Lexeme** get_all_lexemes(FILE* fptr);

Token get_token(Lexeme* lex);
char* get_lexeme(Lexeme* lex);


// setters
Lexeme* make_lexeme(Token t, char* lexeme);
void build_lexeme(Lexeme* lex, Token t, char* str);
void set_token(Lexeme* lex, Token t);
void set_lexeme(Lexeme* lex, char* str);

void assign_token_from_finished_lexeme(Lexeme* lex);
Lexeme* done(Lexeme* lex, const char* lexeme);



// helpers
bool is_alpha(char c);
bool is_escape_char(char c);



// handlers (methods that handle circumstances)
void handle_indentation(FILE* fptr, Lexeme** lexemes, int previous_indents);
char handle_escape_character(char c);
