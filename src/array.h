#ifndef _NTR_LIST_H_
#define _NTR_LIST_H_ 1
#include <stddef.h>
#include <stdbool.h>

struct Array_t {
	void **array;
	unsigned size;
	unsigned used;
};
typedef struct Array_t Array;

Array array_create(size_t sz);
void array_add(Array *l, void *element);
void *array_get(Array l, unsigned index);
unsigned array_index_of(Array l, void *element);
void array_destroy(Array *l);
bool array_is_in(Array l, const void *n);

#endif //_NTR_LIST_H_
