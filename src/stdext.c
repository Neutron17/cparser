#include "stdext.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

bool isFloat(const char *str) {
	if(isDouble(str) && str[strlen(str)] == 'f')
		return true;
	return false;
}
bool isLong(const char *str) {
	if(({ strtol(str, NULL, 10), errno; }) != EINVAL && str[strlen(str)] == 'l')
		return true;
	return false;
}
bool isDouble(const char *str) {
	int len;
	double tmp = 0.0f;
	if (sscanf(str, "%lf %n", &tmp, &len) == 1 && len == (int)strlen(str))
		return true;
	return false;
}
bool isUnsigned(const char *str) {
	int len;
	unsigned tmp = 0.0f;
	if (sscanf(str, "%u %n", &tmp, &len) == 1 && len == (int)strlen(str) && str[strlen(str)] == 'u')
		return true;
	return false;
}
