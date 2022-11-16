#include "simpleshell.h"

/**
 * _getline - stores into malloced buffer the user's command into shell
 * @str: buffer
 * Return: number of characters read
 */
size_t _getline(char **str)
{
	ssize_t nread = 0, size = 0, t = 0, t2 = 0, n = 0;
	char buff[1024];

	nread = read(STDIN_FILENO, buff, 1024 - 1);
	while (t2 == 0 && nread)
	{
		if (nread == -1)
			return (-1);
		buff[nread] = '\0';
		n = 0;
		while (buff[n] != '\0')
		{
			if (buff[n] == '\n')
				t2 = 1;
			n++;
		}

		if (t == 0)
		{
			nread++;
			*str = malloc(sizeof(char) * nread);
			*str = _strcpy(*str, buff);
			size = nread;
			t = 1;
		}
		else
		{
			size += nread;
			*str = _strcat(*str, buff);
		}
	}
	return (size);
}
