#include "strset.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "glob.h"

void set_add(List *l, void *element) {
	for (int i = 0; i < l->used; ++i) {
		if(strcmp(((struct Token *)l->array[i])->value, ((struct Token *)element)->value) == 0) {
			fprintf(stderr, "Multiple definitions of identifier: '%s'\n", ((struct Token *)element)->value);
			abort();
		}
	}
	list_add(l, (void *)element);
}

bool set_is_in(Set s, void *n) {
	for(int i = 0; i < s.used; i++) {
		if(strcmp(((struct Token *)s.array[i])->value, ((struct Token *)n)->value) == 0)
			return true;
	}
	return false;
}
