#ifndef _NTR_SET_H_
#define _NTR_SET_H_ 1
#include "array.h"

#define Set Array

#define set_create(sz) array_create(sz)
void set_add(Array *l, void *element);
#define set_get(L, I) array_get(L, I)
#define set_index_of(l, element) array_index_of(l, element)
#define set_destroy(l) array_destroy(l)
bool set_is_in(Set s, void *n);

#endif //_NTR_SET_H_
