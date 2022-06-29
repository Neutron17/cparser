#ifndef _NTR_SET_H_
#define _NTR_SET_H_ 1
#include "list.h"

#define Set List

#define set_create(sz) list_create(sz)
void set_add(List *l, void *element);
#define set_get(L, I) list_get(L, I)
#define set_index_of(l, element) list_index_of(l, element)
#define set_destroy(l) list_destroy(l)
bool set_is_in(Set s, void *n);

#endif //_NTR_SET_H_
