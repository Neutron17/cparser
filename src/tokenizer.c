#include "tokenizer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

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

List tokenize(char *txt) {
	List ret = list_create(30);
	char *line, *line_sp, *elem_sp;
	for(line = strtok_r(txt, ";", &line_sp);
		line != NULL;
		line = strtok_r(NULL, ";", &line_sp)) {

		char *element;
		for(element = strtok_r(line, " ", &elem_sp);
			element != NULL;
			element = strtok_r(NULL, " ", &elem_sp)) {

			bool hasFound = false;
			for (int i = 0; i < 7; i++) {
				if (hasFound)
					continue;

				if (strncmp(element, keywords[i], 5) == 0) {
					enum KeyWordType type;
					type = strtotype(element, keywords);
					list_add(&ret, (void *) token(T_KEYWORD, type, element));
					hasFound = true;
				} else if (strncmp(element, type_keywords[i], 6) == 0) {
					enum KeyWordType type;
					type = strtotype(element, type_keywords);
					list_add(&ret, (void *) token(T_TYPE_KEYWORD, type, element));
					hasFound = true;
				} else if (strncmp(element, operators[i], 1) == 0) {
					enum KeyWordType type;
					type = strtotype(element, operators);
					list_add(&ret, (void *) token(T_OPERATOR, type, element));
					hasFound = true;
				} else if (strncmp(element, separators[i], 1) == 0) {
					enum KeyWordType type;
					type = strtotype(element, separators);
					list_add(&ret, (void *) token(T_SEPARATOR, type, element));
					hasFound = true;
				}
			}
			if (!hasFound) {
				list_add(&ret, (void *) token(T_IDENTIFIER, 0, element));
			}
		}
		list_add(&ret, (void *) token(T_SEPARATOR, S_SEMICOL, ";"));
	}
	// rules
	for(int i = 0; i < ret.used; i++) {
		switch (((struct Token *)ret.array[i])->type) {
			case T_KEYWORD: {
				break;
			}
			case T_TYPE_KEYWORD: {
				break;
			}
			case T_SEPARATOR: {
				break;
			}
			case T_OPERATOR: {
				break;
			}
			case T_IDENTIFIER: {
				//printf("%d %d\n",((struct Token *)ret.array[i+1])->type, ((struct Token *)ret.array[i+1])->subtype);

				// if the prev token was a type, then the next must be an assign or an end of line
				if(((struct Token *)ret.array[i-1])->type == T_TYPE_KEYWORD)
					assert((((struct Token *)ret.array[i+1])->type == T_OPERATOR && ((struct Token *)ret.array[i+1])->subtype == O_EQUALS) ||
							((struct Token *)ret.array[i+1])->type == T_SEPARATOR && ((struct Token *)ret.array[i+1])->subtype == S_SEMICOL);
				break;
			}
			case T_LITERAL: {
				break;
			}
			case T_COMMENT: {
				break;
			}
			default:{}
		}
	}
	return ret;
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