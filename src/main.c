//#define MEM_DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ntr/debug.h>
#include "tokenizer.h"

struct Lex {
	char *lval;
	char *rval;
	bool isValid;
};

char program[] = "int b;int b;short a = b * c ;"
				 "int b = c ;";

struct Lex parse(const char *txt);
//struct Lex strtolex(const char *txt);

int main(int argc, const char *argv[]) {
	List list = tokenize(program);
	for(int i = 0; i<list.used;i++) {
		struct Token *toks = ((struct Token *)list.array[i]);
		printf("%d %s %d\n", toks->type, toks->value, toks->subtype);
		free((struct Token *)list.array[i]);
	}
	free(list.array);
	//struct Lex lex = parse(program);
	//puts("");
	//printf("l: '%s'\nr: '%s'\n", lex.lval, lex.rval);
	//free(lex.lval);
	//free(lex.rval);
	return 0;
}
struct Lex parse(const char *txt) {
	struct Lex ret = {NULL, NULL, true};
	//ret = strtolex(txt);
	char l[8];
	strncpy(ret.lval, l, 8);
	while(ret.isValid == true) {
		printf("bef l: '%s'\nr: '%s'\n", ret.lval, ret.rval);
		//ret = strtolex(ret.rval);
		printf("aft: l: '%s'\nr: '%s'\n", ret.lval, ret.rval);
		//free(ret.rval);
		//free(ret.lval);
	}
	ret.lval = l;
	return ret;
}
/*
struct Lex strtolex(const char *txt) {
	//puts(txt);
	struct Lex ret;
	ret.lval = malloc(16);
	ret.rval = malloc(16);
	ret.isValid = true;
	for(int i = 0; i<strlen(txt); i++) {
		for(int j = 0; j < OP_SZ; j++) {
			//printf("txt: %c op: %c\n", txt[i], operators[j]);
			if(txt[i] == operators[j]) {
				strncpy(ret.lval, txt, i);
				strcpy(ret.rval, txt+i+1);
				return ret;
			}
		}
	}
	ret.isValid = false;
	return ret;
}*/
