#include "test_shell.h"

/**
 * freeptr - free malloced arrays
 * @str: array of strings
 */

void freeptr(char **str)
{
	int i;

	for (i = 0; str[i] != NULL; i++)
	{
		free(str[i]);
	}
	free(str);
}
