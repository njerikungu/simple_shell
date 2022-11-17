#include "simpleshell.h"

/**
 * _strlen - Returns the length of a string.
 * @str: A pointer to the characters string.
 * Return: The length of the character string.
 */
int _strlen(const char *str)
{
	int length = 0;

	if (!str)
		return (length);
	for (length = 0; str[length]; length++)
		;
	return (length);
}

/**
 * _strcpy - Copies the string pointed to by src, including the
 *           terminating null byte, to the buffer pointed by des.
 * @dest: Pointer to the destination of copied string.
 * @src: Pointer to the src of the source string.
 * Return: Pointer to dest.
 */
char *_strcpy(char *dest, const char *src)
{
	size_t i;

	for (i = 0; src[i] != '\0'; i++)
		dest[i] = src[i];
	dest[i] = '\0';
	return (dest);
}

/**
 * _strcat - Concantenates two strings.
 * @dest: Pointer to destination string.
 * @src: Pointer to source string.
 * Return: Pointer to destination string.
 */
char *_strcat(char *dest, const char *src)
{
	char *dest_copy;
	const char *src_copy;

	dest_copy = dest;
	src_copy =  src;

	while (*dest_copy != '\0')
		dest_copy++;

	while (*src_copy != '\0')
		*dest_copy++ = *src_copy++;
	*dest_copy = '\0';
	return (dest);
}

/**
 * _strncat - Concantenates two strings where n number
 * of bytes are copied from source.
 * @dest: Pointer to destination string.
 * @src: Pointer to source string.
 * @n: n bytes to copy from src.
 * Return: Pointer to destination string.
 */
char *_strncat(char *dest, const char *src, size_t n)
{
	size_t d_len = _strlen(dest);
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[d_len + i] = src[i];
	dest[d_len + i] = '\0';

	return (dest);
}