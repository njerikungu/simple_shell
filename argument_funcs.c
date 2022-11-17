#include "simpleshell.h"

/**
 * _getargs - Gets a command from standard input.
 * @line: A buffer to store the command.
 * @ret_val: The return value of the last executed command.
 * Return: If an error occurs - NULL. Otherwise - a pointer to the stored command.
 */
char *_getargs(char *line, int *ret_val)
{
	size_t n = 0;
	ssize_t read;
	char *prompt = "$ ";

	if (line)
		free(line);

	read = _getline(&line, &n, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (_getargs(line, ret_val));
	}

	line[read - 1] = '\0';
	variable_replacement(&line, ret_val);
	handle_line(&line, read);

	return (line);
}

/**
 * call_args - Partitions operators from commands and calls them.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 * @ret_val: The return value of the parent process' last executed command.
 * Return: The return value of the last executed command.
 */
int call_args(char **args, char **front, int *ret_val)
{
	int ret, index;

	if (!args[0])
		return (*ret_val);
	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], "||", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = replace_aliases(args);
			ret = run_args(args, front, ret_val);
			if (*ret_val != 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
		else if (_strncmp(args[index], "&&", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = replace_aliases(args);
			ret = run_args(args, front, ret_val);
			if (*ret_val == 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
	}
	args = replace_aliases(args);
	ret = run_args(args, front, ret_val);
	return (ret);
}

/**
 * run_args - Calls the execution of a command.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 * @exe_ret: The return value of the parent process' last executed command.
 * Return: The return value of the last executed command.
 */
int run_args(char **args, char **front, int *ret_val)
{
	int ret, i;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin(args[0]);

	if (builtin)
	{
		ret = builtin(args + 1, front);
		if (ret != EXIT)
			*ret_val = ret;
	}
	else
	{
		*ret_val = execute(args, front);
		ret = *ret_val;
	}

	hist++;

	for (i = 0; args[i]; i++)
		free(args[i]);

	return (ret);
}

/**
 * handle_args - Gets, calls, and runs the execution of a command.
 * @ret_val: The return value of the parent process' last executed command.
 * Return: If an end-of-file is read - END_OF_FILE (-2).
 * If the input cannot be tokenized - -1.
 * O/w - The exit value of the last executed command.
 */
int handle_args(int *ret_val)
{
	int ret = 0, index;
	char **args, *line = NULL, **front;

	line = _getargs(line, ret_val);
	if (!line)
		return (END_OF_FILE);

	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (ret);
	if (check_args(args) != 0)
	{
		*ret_val = 2;
		free_args(args, args);
		return (*ret_val);
	}
	front = args;

	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], ";", 1) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			ret = call_args(args, front, ret_val);
			args = &args[++index];
			index = 0;
		}
	}
	if (args)
		ret = call_args(args, front, ret_val);

	free(front);
	return (ret);
}

/**
 * check_args - Checks if there are any leading ';', ';;', '&&', or '||'.
 * @args: 2D pointer to tokenized commands and arguments.
 * Return: If a ';', '&&', or '||' is placed at an invalid position - 2.
 * Otherwise - 0.
 */
int check_args(char **args)
{
	size_t i;
	char *cur, *nex;

	for (i = 0; args[i]; i++)
	{
		cur = args[i];
		if (cur[0] == ';' || cur[0] == '&' || cur[0] == '|')
		{
			if (i == 0 || cur[1] == ';')
				return (create_error(&args[i], 2));
			nex = args[i + 1];
			if (nex && (nex[0] == ';' || nex[0] == '&' || nex[0] == '|'))
				return (create_error(&args[i + 1], 2));
		}
	}
	return (0);
}