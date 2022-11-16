#include "simpleshell.h"

/**
 * cus_dupstr - custom string duplication; excludes beginning bytes
 * @name: string to duplicate (e.g. environmental variable PATH=/bin:/bin/ls)
 * @bytes: number of bytes to exclude (e.g. excludes "PATH=")
 * Return: string (e.g. /bin:/bin/ls)
 */

char *cus_dupstr(char *name, int bytes)
{
	char *dup_str;
	int i, len;

	if (name == NULL)
		return (NULL);
	for (len = 0; name[len]; len++)
		len++;
	len++;
	dup_str = malloc(sizeof(char) * (len - bytes));
	if (dup_str == NULL)
		return (NULL);
	for (i = 0; i < (len - bytes); i++)
		dup_str[i] = name[bytes + i];
	return (dup_str);
}

/**
 * _getenv - finds and returns a copy of the requested environmental variable
 * @name: string to store it in
 * @env: entire set of environmental variables
 * Return: copy of requested environmental variable
 */

char *_getenv(char *name, list_t *env)
{
	int j = 0, bytes = 0;

	while (env != NULL)
	{
		j = 0;
		while ((env->var)[j] == name[j])
			j++;
		if (name[j] == '\0' && (env->var)[j] == '=')
			break;
		env = env->next;
	}
	while (name[bytes] != '\0')
		bytes++;
	bytes++;
	return (cus_dupstr(env->var, bytes));
}
