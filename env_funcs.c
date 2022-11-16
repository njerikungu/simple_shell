#include "simpleshell.h"

/**
 * _findenv - find given environmental variable in linked list
 * @env: environmental variable linked list
 * @name: variable name
 * Return: index of node in linked list
 */

int _findenv(list_t *env, char *name)
{
	int j, index;

	for (index = 0; env != NULL; index++)
	{
		j = 0;
		while ((env->var)[j] == name[j])
			j++;
		if (name[j] == '\0')
			break;
		env = env->next;
	}

	if (env == NULL)
		return (-1);
	return (index);
}

/**
 * _unsetenv - remove node in environmental linked list
 * @env: linked list
 * @str: user's typed in command (e.g. "unsetenv MAIL")
 * Return: 0 on success
 */

int _unsetenv(list_t **env, char **str)
{
	int index = 0, j = 0;

	if (str[1] == NULL)
	{
		write(STDOUT_FILENO, "Too few arguments\n", 18);
		freeptr(str);
		return (-1);
	}
	index = _findenv(*env, str[1]);
	freeptr(str);
	if (index == -1)
	{
		write(STDOUT_FILENO, "Cannot find\n", 12);
		return (-1);
	}
	j = delete_nodeint_at_index(env, index);
	if (j == -1)
	{
		write(STDOUT_FILENO, "Cannot find\n", 12);
		return (-1);
	}
	return (0);
}

/**
 * _setenv - create or modify existing environmental variable in linked list
 * @env: linked list
 * @str: user's typed in command (e.g. "setenv USER Superman")
 * Return: 0 on success, 1 on fail
 */

int _setenv(list_t **env, char **str)
{
	int index = 0, j = 0;
	char *cat_str;
	list_t *holder;

	if (str[1] == NULL || str[2] == NULL)
	{
		write(STDOUT_FILENO, "Too few arguments\n", 18);
		freeptr(str);
		return (-1);
	}
	cat_str = _strdup(str[1]);
	cat_str = _strcat(cat_str, "=");
	cat_str = _strcat(cat_str, str[2]);
	index = _findenv(*env, str[1]);
	if (index == -1)
	{
		add_end_node(env, cat_str);
	}
	else
	{
		holder = *env;
		while (j < index)
		{
			holder = holder->next;
			j++;
		}
		free(holder->var);
		holder->var = _strdup(cat_str);
	}
	free(cat_str);
	freeptr(str);
	return (0);
}

/**
 * env_linked_list - creates a linked list from environmental variables
 * @env: environmental variables
 * Return: linked list
 */
list_t *env_linked_list(char **env)
{
	list_t *head;
	int i = 0;

	head = NULL;
	while (env[i] != NULL)
	{
		add_end_node(&head, env[i]);
		i++;
	}
	return (head);
}

/**
 * _env - prints environmental variables
 * @str: user's command into shell (i.e. "env")
 * @env: environmental variables
 * Return: 0 on success
 */
int _env(char **str, list_t *env)
{
	freeptr(str);
	_printlist(env);
	return (0);
}
