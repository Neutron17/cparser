#ifndef _NTR_LIST_H_
#define _NTR_LIST_H_ 1
#include <stddef.h>
#include <stdbool.h>

struct List_t {
	void **array;
	unsigned size;
	unsigned used;
};
typedef struct List_t List;

List list_create(size_t sz);
void list_add(List *l, void *element);
void *list_get(List l, unsigned index);
unsigned list_index_of(List l, void *element);
void list_destroy(List *l);
bool list_is_in(List l, const void *n);

#endif //_NTR_LIST_H_
