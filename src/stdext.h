#ifndef _NTR_STDEXT_H_
#define _NTR_STDEXT_H_ 1
#include <stdbool.h>

bool isFloat(const char *str);
bool isDouble(const char *str);
bool isLong(const char *str);
bool isUnsigned(const char *str);

void append(const char *from, char *to, const char *insert, int pos);

#endif //_NTR_STDEXT_H_
