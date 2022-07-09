#include "tokenizer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include "stdext.h"
#include "strset.h"
#include "glob.h"

const char *type_keywords[7] = {
		"int","char","float","double","long","string","short"
};

const char *keywords[7] = {
		"if","else","while","for","do","until","elif"
};

const char *operators[7] = {
		"=","+","-","*","/","%","&"
};
const char *separators[7] = {
		";","(",")","{","}","[","]"
};

static void insert_space(char *txt);

Set identifiers;

Array tokenize(char *txt) {
	//insert_space(txt);
	//printf("%s\n", txt);
	Array ret = array_create(30);
	identifiers = set_create(10);
	char *line, *line_sp, *elem_sp;
	for( line = strtok_r(txt, ";", &line_sp);
		 line != NULL;
		 line = strtok_r(NULL, ";", &line_sp)) {

		char *symbol;
		for( symbol = strtok_r(line, " \t\n", &elem_sp);
			 symbol != NULL;
			 symbol = strtok_r(NULL, " \t\n", &elem_sp)) {

			char tmp = symbol[0];
			while(isspace(tmp)) {
				symbol++;
				tmp = symbol[0];
			}

			bool hasFound = false;
			for (int i = 0; i < 7; i++) {
				if (hasFound)
					continue;

				if (strncmp(symbol, keywords[i], 5) == 0) {
					enum KeyWordType type;
					type = strtotype(symbol, keywords);
					array_add(&ret, (void *) token(T_KEYWORD, type, symbol));
					hasFound = true;
				} else if(strncmp(symbol, "//", 2) == 0) {
					goto comment;
				} else if (strncmp(symbol, type_keywords[i], 6) == 0) {
					enum KeyWordType type;
					type = strtotype(symbol, type_keywords);
					array_add(&ret, (void *) token(T_TYPE_KEYWORD, type, symbol));
					hasFound = true;
				} else if (strncmp(symbol, operators[i], 1) == 0) {
					enum KeyWordType type;
					type = strtotype(symbol, operators);
					array_add(&ret, (void *) token(T_OPERATOR, type, symbol));
					hasFound = true;
				} else if (strncmp(symbol, separators[i], 1) == 0) {
					enum KeyWordType type;
					type = strtotype(symbol, separators);
					array_add(&ret, (void *) token(T_SEPARATOR, type, symbol));
					hasFound = true;
				}
			}
			if (!hasFound) { // literal or identifier
				if(strncmp(symbol, "true", 4) == 0) {
					array_add(&ret, (void *) token(T_LITERAL, L_TRUE, symbol));
				} else if(strncmp(symbol, "false", 4) == 0) {
					array_add(&ret, (void *) token(T_LITERAL, L_FALSE, symbol));
				} else if(isDouble(symbol)/*({ strtol(symbol, NULL, 10), errno; }) != EINVAL*/) {
					if(isFloat(symbol))
						array_add(&ret, (void *) token(T_LITERAL, L_FLOAT, symbol));
					else if(isDouble(symbol))
						array_add(&ret, (void *) token(T_LITERAL, L_DOUBLE, symbol));
					else if(isLong(symbol))
						array_add(&ret, (void *) token(T_LITERAL, L_LONG, symbol));
					else if(isUnsigned(symbol))
						array_add(&ret, (void *) token(T_LITERAL, L_UINT, symbol));
					else
						array_add(&ret, (void *) token(T_LITERAL, L_INT, symbol));
				} else {
					for(int i = 0; i < strlen(symbol); i++) {
						if(isspace(symbol[i])) {
							fprintf(stderr, "Invalid variable name: '%s'\n", symbol);
							abort();
						}
					}
					array_add(&ret, (void *) token(T_IDENTIFIER, 0, symbol));
				}
			}
		}
		array_add(&ret, (void *) token(T_SEPARATOR, S_SEMICOL, ";"));
comment:;
	}
	// rules
	for(int i = 0; i < ret.used; i++) {
		struct Token *curr = ((struct Token *) ret.array[i]);
		struct Token *prev = ((struct Token *) ret.array[i - 1]);
		struct Token *next = ((struct Token *) ret.array[i + 1]);
		//printf("%d %d %s\n", curr->type, curr->subtype, curr->value);
		switch (curr->type) {
			case T_KEYWORD: {
				switch (((struct Token *) ret.array[i + 1])->subtype) {
					case K_IF: {
						if (next->type == T_SEPARATOR &&
							next->subtype == S_PRENTH_BEGIN) {}
						else {
							fprintf(stderr, "'if' must be followed by '('\n");
							abort();
						}
						break;
					}
					case K_ELSE: {
						if((next->type == T_SEPARATOR &&
								next->subtype == S_BRACE_BEGIN) ||
							   (next->type == T_KEYWORD &&
								next->subtype == K_IF)){}
						else {
							fprintf(stderr, "'else' must be followed by '(' or 'if'\n");
							abort();
						}
						break;
					}
					case K_ELIF: {
						if (next->type == T_SEPARATOR &&
							next->subtype == S_PRENTH_BEGIN) {}
						else {
							fprintf(stderr, "'if' must be followed by '('\n");
							abort();
						}
						break;
					}
					case K_UNTIL: {
						break;
					}
					case K_WHILE: {
						if(next->type == T_SEPARATOR &&
							   next->subtype == S_PRENTH_BEGIN) {}
						else {
							fprintf(stderr, "'while' must be followed by '('\n");
							abort();
						}
						break;
					}
					case K_FOR: {
						if(next->type == T_SEPARATOR &&
							   next->subtype == S_PRENTH_BEGIN) {}
						else {
							fprintf(stderr, "'for' must be followed by '('\n");
							abort();
						}
						break;
					}
					case K_DO: {
						if(next->type == T_SEPARATOR &&
							   next->subtype == S_BRACE_BEGIN) {}
						else {
							fprintf(stderr, "'do' must be followed by '{'\n");
							abort();
						}
						break;
					}
				}
				break;
			}
			case T_TYPE_KEYWORD: {
				if(next->type != T_IDENTIFIER) {
					fprintf(stderr, "Keyword: '%s' must be followed by an an identifier\n", curr->value);
					abort();
				}
				break;
			}
			case T_SEPARATOR: {
				break;
			}
			case T_OPERATOR: {
				// before and after an operator there must be an identifier or a literal or ( / { / [
				if( (prev->type == T_IDENTIFIER ||
					((prev->type == T_SEPARATOR) &&
							(prev->subtype == S_PRENTH_BEGIN)) ||
							(prev->subtype == S_BRACE_BEGIN) ||
							(prev->subtype == S_BRACKET_BEGIN))) {}
				else {
					fprintf(stderr,
							"'%s' not accepted before operator: '%c'\nmust be an identifier or literal or '(' or '{' or '['\n",
							prev->value, ((struct Token *)ret.array[i])->value[0]);
					abort();
				}
				if( (next->type == T_IDENTIFIER ||
					(next->type == T_LITERAL) ||
					((next->type == T_SEPARATOR) &&
							(next->subtype == S_PRENTH_BEGIN)) ||
							(next->subtype == S_BRACE_BEGIN) ||
							(next->subtype == S_BRACKET_BEGIN)) ) {}
				else {
					fprintf(stderr,
							"'%s' not accepted after operator: '%c'\nmust be an identifier or literal or '(' or '{' or '['\n",
							next->value, ((struct Token *)ret.array[i])->value[0]);
					abort();
				}
				break;
			}
			case T_IDENTIFIER: {
				// if the prev token was a type, then the next must be an assign or an end of line
				if (prev->type == T_TYPE_KEYWORD) {

					//set_add(&identifiers, (void *)curr);
					if ((next->type == T_OPERATOR &&
						next->subtype == O_EQUALS) ||

						(next->type == T_SEPARATOR &&
						next->subtype == S_SEMICOL)) {

						curr->subtype = I_VAR;
					} else if (next->type == T_SEPARATOR &&
							   next->subtype == S_PRENTH_BEGIN) {
						curr->subtype = I_FUNC;
					} else {
						fprintf(stderr,
								"'%s' not accepted after identifier: '%s'\nMust be an assign(=), end of line (;) or function declaration()\n",
								curr->value, next->value);
						abort();
					}
				} else {
					if(set_is_in(identifiers, curr)) {
						continue;
					} else {
						fprintf(stderr, "Use of undeclared identifier: '%s'\n", curr->value);
						abort();
					}
				}
				set_add(&identifiers, (void *)curr);
				break;
			}
			case T_LITERAL: {
				/*assert(prev->type == T_OPERATOR &&
					   prev->subtype == O_EQUALS);*/
				break;
			}
			case T_COMMENT: {
				break;
			}
			default: {
			}
		}
	}
	for (int i = 0; i < identifiers.used; ++i)
		printf("%s\n", ((struct Token *)identifiers.array[i])->value);
	set_destroy(&identifiers);
	return ret;
}

static void insert_space(char *txt) {
	for(int i = 0; i < strlen(txt); i++) {
		if(txt[i] == '{' || txt[i] == '}' || txt[i] == '(' || txt[i] == ')' ||
		   txt[i] == '[' || txt[i] == ']' || txt[i] == '+' || txt[i] == '-' ||
		   txt[i] == '*' || txt[i] == '/' || txt[i] == '=' || txt[i] == '%' ||
		   txt[i] == '&' || txt[i] == '|' || txt[i] == '!' || txt[i] == '?') {
			//append(txt, " ", i);
			//append(txt, " ", i+1);
		}
	}
}

struct Token *token(enum TType type, int subtype, char *value){
	struct Token *ret = malloc(sizeof (struct Token));
	ret->type = type;
	ret->subtype = subtype;
	strncpy(ret->value, value, 7);
	return ret;
}

int strtotype(const char *restrict str, const char *restrict arr[7]) {
	for(int i = 0; i < 7; i++) {
		if(strncmp(str, arr[i], 5) == 0)
			return i;
	}
	return -1;
}
