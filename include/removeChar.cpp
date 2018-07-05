#include "removeChar.h"

void removeChar(char* str, char c)
{
	char *pr = str, *pw = str;
	while (*pr) {
		*pw = *pr++;
		pw += (*pw != c);
	}
	*pw = '\0';
}