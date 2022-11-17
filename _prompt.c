#include "test_shell.h"

/**
 * ctrl_c - ignore Ctrl-C input and prints prompt again
 * @n: takes in int from signal
 */
void ctrl_c(int n)
{
	(void)n;
	write(STDOUT_FILENO, "\n$ ", 3);
}

/**
 * built_in - handles builtins (exit, env, cd)
 * @token: user's typed command
 * @env: environmental variable
 * @num: take in nth user command typed to write error message
 * @command: bring in command to free
 * Return: 1 if acted on builtin, 0 if not
 */
int built_in(char **token, list_t *env, int num, char **command)
{
	int i = 0;

	if (_strcmp(token[0], "exit") == 0)
	{
		i = __exit(token, env, num, command);
	}
	else if (_strcmp(token[0], "env") == 0)
	{
		_env(token, env);
		i = 1;
	}
	else if (_strcmp(token[0], "cd") == 0)
	{
		i = _cd(token, env, num);
	}
	else if (_strcmp(token[0], "setenv") == 0)
	{
		_setenv(&env, token);
		i = 1;
	}
	else if (_strcmp(token[0], "unsetenv") == 0)
	{
		_unsetenv(&env, token);
		i = 1;
	}
	return (i);
}

/**
 * ignore_space - return string without spaces in front
 * @str: string
 * Return: new string
 */
char *ignore_space(char *str)
{
	while (*str == ' ')
		str++;
	return (str);
}

/**
 * ctrl_D - exits program if Ctrl-D was pressed
 * @i: characters read via get_line
 * @command: user's typed in command
 * @env: environmental variable linked list
 */
void ctrl_D(int i, char *command, list_t *env)
{
	if (i == 0)
	{
		free(command);
		free_linked_list(env);
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "\n", 1);
		exit(0);
	}
}

/**
 * prompt - repeatedly prompts user and executes user's cmds if applicable
 * @en: envrionmental variables
 * Return: 0 on success
 */
int prompt(char **en)
{
	list_t *env;
	size_t i = 0, n = 0;
	int command_line_no = 0, exit_stat = 0;
	char *command, *n_command, **token;

	env = env_linked_list(en);
	do {
		command_line_no++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);
		else
			non_interactive(env);
		signal(SIGINT, ctrl_c);
		command = NULL;
		i = 0;
		i = _getline(&command);
		ctrl_D(i, command, env);
		n_command = command;
		command = ignore_space(command);
		n = 0;
		while (command[n] != '\n')
			n++;
		command[n] = '\0';
		if (command[0] == '\0')
		{
			free(n_command);
			continue;
		}
		token = NULL;
		token = _str_tok(command, " ");
		if (n_command != NULL)
			free(n_command);
		exit_stat = built_in(token, env, command_line_no, NULL);
		if (exit_stat)
			continue;
		exit_stat = _execve(token, env, command_line_no);
	} while (1);
	return (exit_stat);
}
