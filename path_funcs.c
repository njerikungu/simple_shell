#include "simpleshell.h"

/**
 * get_location - Locates a command in the PATH.
 * @cmd: The command to locate.
 *
 * Return: If an error occurs or the command cannot be located - NULL.
 *         Otherwise - the full pathname of the command.
 */
char *get_location(char *cmd)
{
	char **path, *tmp;
	list_t *dirs, *head;
	struct stat st;

	path = _getenv("PATH");
	if (!path || !(*path))
		return (NULL);

	dirs = get_pathdir(*path + 5);
	head = dirs;

	while (dirs)
	{
		tmp = malloc(_strlen(dirs->dir) + _strlen(cmd) + 2);
		if (!tmp)
			return (NULL);

		_strcpy(tmp, dirs->dir);
		_strcat(tmp, "/");
		_strcat(tmp, cmd);

		if (stat(tmp, &st) == 0)
		{
			free_list(head);
			return (tmp);
		}

		dirs = dirs->next;
		free(tmp);
	}

	free_list(head);

	return (NULL);
}

/**
 * fill_pathdir - Copies path but also replaces leading/sandwiched/trailing
 * colons (:) with current working directory.
 * @path_name: The colon-separated list of directories.
 * Return: A copy of path with any leading/sandwiched/trailing colons replaced
 *	   with the current working directory.
 */
char *fill_pathdir(char *path)
{
	int i, length = 0;
	char *path_copy, *pwd;

	pwd = *(_getenv("PWD")) + 4;
	for (i = 0; path[i]; i++)
	{
		if (path[i] == ':')
		{
			if (path[i + 1] == ':' || i == 0 || path[i + 1] == '\0')
				length += _strlen(pwd) + 1;
			else
				length++;
		}
		else
			length++;
	}
	path_copy = malloc(sizeof(char) * (length + 1));
	if (!path_copy)
		return (NULL);
	path_copy[0] = '\0';
	for (i = 0; path[i]; i++)
	{
		if (path[i] == ':')
		{
			if (i == 0)
			{
				_strcat(path_copy, pwd);
				_strcat(path_copy, ":");
			}
			else if (path[i + 1] == ':' || path[i + 1] == '\0')
			{
				_strcat(path_copy, ":");
				_strcat(path_copy, pwd);
			}
			else
				_strcat(path_copy, ":");
		}
		else
		{
			_strncat(path_copy, &path[i], 1);
		}
	}
	return (path_copy);
}

/**
 * get_pathdir - Tokenizes a colon-separated list of
 * directories into a list_s linked list.
 * @path: The colon-separated list of directories.
 * Return: A pointer to the initialized linked list.
 */
list_t *get_pathdir(char *path)
{
	int index;
	char **dirs, *path_copy;
	list_t *head = NULL;

	path_copy = fill_pathdir(path);
	if (!path_copy)
		return (NULL);
	dirs = _strtok(path_copy, ":");
	free(path_copy);
	if (!dirs)
		return (NULL);

	for (index = 0; dirs[index]; index++)
	{
		if (add_node_end(&head, dirs[index]) == NULL)
		{
			free_list(head);
			free(dirs);
			return (NULL);
		}
	}

	free(dirs);

	return (head);
}