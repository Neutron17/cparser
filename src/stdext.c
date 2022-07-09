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

void append(const char *from, char *to, const char *insert, int pos) {
	char *buf = malloc(512); // 100 so that it's big enough. fill with zeros
	// or you could use malloc() to allocate sufficient space
	// e.g. char *buf = (char*)malloc(strlen(subject) + strlen(insert) + 2);
	// to fill with zeros: memset(buf, 0, 100);

	strncpy(buf, from, pos); // copy at most first pos characters
	size_t len = strlen(buf);
	strcpy(buf+len, insert); // copy all of insert[] at the end
	len += strlen(insert);  // increase the length by length of insert[]
	strcpy(buf+len, from+pos); // copy the rest

	strcpy(to, buf);   // copy it back to subject
	free(buf);
	// Note that subject[] must be big enough, or else segfault.
	// deallocate buf[] here, if used malloc()
	// e.g. free(buf);
}
