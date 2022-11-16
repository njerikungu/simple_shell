#include "simpleshell.h"

/**
 * clear_exit - frees user's typed command and linked
 * list before exiting
 * @str: user's typed command
 * @env: input the linked list of envirnment
 */

void clear_exit(char **str, list_t *env)
{
	freeptr(str);
	free_linked_list(env);
	_exit(0);
}

/**
 * _execve - execute command user typed into shell
 * @env: environmental variable
 * @num: nth user command; to be used in error message
 * @str: typed command
 * Return: 0 on success
 */

int _execve(char **str, list_t *env, int num)
{
	char *holder;
	int status = 0, i = 0;
	pid_t pid;

	if (access(str[0], F_OK) == 0)
	{
		holder = str[0];
		i = 1;
	}
	else
		holder = _which(str[0], env);
	if (access(holder, X_OK) != 0)
	{
		not_found(str[0], num, env);
		freeptr(str);
		return (127);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			if (execve(holder, str, NULL) == -1)
			{
				not_found(str[0], num, env);
				clear_exit(str, env);
			}
		}
		else
		{
			wait(&status);
			freeptr(str);
			if (i == 0)
				free(holder);
		}
	}
	return (0);
}
