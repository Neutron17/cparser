#include "list.h"
#include <stdlib.h>
#include "glob.h"

List list_create(size_t sz) {
	List ret;
	ret.array = malloc(sizeof(void *) * sz);
	ret.size = sz;
	ret.used = 0;
	return ret;
}

void list_add(List *l, void *element) {
	if(l->used >= l->size)
		l->array = realloc(l->array, sizeof(void *) * l->size * 2);
	l->array[l->used] = element;
	l->used++;
}

void *list_get(List l, unsigned index) {
	return l.array[index];
}

unsigned list_index_of(List l, void *element) {
	for(int i = 0; i < l.used; i++) {
		if(l.array[i] == element)
			return i;
	}
	return -1;
}

void list_destroy(List *l) {
	free(l->array);
	l->array = NULL;
}

bool list_is_in(List l, const void *n) {
	for(int i = 0; i < l.used; i++) {
		if(l.array[i] == n)
			return true;
	}
	return false;
}