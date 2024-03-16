#include "utils.h"

#include <stdlib.h>

size_t countChar(const char *const string, const char character)
{
	size_t count = 0;

	for (char *s = (char *) string; *s != '\0'; s++) {
		if (*s != character) continue;

		++count;
	}

	return count;
}

size_t countNChar(
		const char *const string,
		const char character,
		const size_t nBytes
		)
{
	size_t count = 0;

	size_t index = 0;
	for (
			char *s = (char *) string;
			index < nBytes && *s != '\0';
			s++, index++
	    ) {
		if (*s != character) continue;

		++count;
	}

	return count;
}
