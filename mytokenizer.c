#include "shell.h"

/**
 * **my_strtow - splits a string into words. Repeat delimiters are ignored
 * @my_str: the input string
 * @my_d: the delimiter string
 * Return: a pointer to an array of strings, or NULL on failure
 */

char **my_strtow(char *my_str, char *my_d)
{
	int my_i, my_j, my_k, my_m, my_numwords = 0;
	char **my_s;

	if (my_str == NULL || my_str[0] == 0)
		return (NULL);
	if (!my_d)
		my_d = "\t";
	for (my_i = 0; my_str[my_i] != '\0'; my_i++)
		if (!my_is_delim(my_str[my_i], my_d) && (my_is_delim(my_str[my_i + 1], my_d) || !my_str[my_i + 1]))
			my_numwords++;

	if (my_numwords == 0)
		return (NULL);
	my_s = malloc((1 + my_numwords) * sizeof(char *));
	if (!my_s)
		return (NULL);
	for (my_i = 0, my_j = 0; my_j < my_numwords; my_j++)
	{
		while (my_is_delim(my_str[my_i], my_d))
			my_i++;
		my_k = 0;
		while (!my_is_delim(my_str[my_i + my_k], my_d) && my_str[my_i + my_k])
			my_k++;
		my_s[my_j] = malloc((my_k + 1) * sizeof(char));
		if (!my_s[my_j])
		{
			for (my_k = 0; my_k < my_j; my_k++)
				free(my_s[my_k]);
			free(my_s);
			return (NULL);
		}
		for (my_m = 0; my_m < my_k; my_m++)
			my_s[my_j][my_m] = my_str[my_i++];
		my_s[my_j][my_m] = 0;
	}
	my_s[my_j] = NULL;
	return (my_s);
}

/**
 * **my_strtow2 - splits a string into words
 * @my_str: the input string
 * @my_d: the delimiter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **my_strtow2(char *my_str, char my_d)
{
	int my_i, my_j, my_k, my_m, my_numwords = 0;
	char **my_s;

	if (my_str == NULL || my_str[0] == 0)
		return (NULL);
	for (my_i = 0; my_str[my_i] != '\0'; my_i++)
		if ((my_str[my_i] != my_d && my_str[my_i + 1] == my_d) ||
		    (my_str[my_i] != my_d && !my_str[my_i + 1]) || my_str[my_i + 1] == my_d)
			my_numwords++;
	if (my_numwords == 0)
		return (NULL);
	my_s = malloc((1 + my_numwords) * sizeof(char *));
	if (!my_s)
		return (NULL);
	for (my_i = 0, my_j = 0; my_j < my_numwords; my_j++)
	{
		while (my_str[my_i] == my_d && my_str[my_i] != my_d)
			my_i++;
		my_k = 0;
		while (my_str[my_i + my_k] != my_d && my_str[my_i + my_k] && my_str[my_i + my_k] != my_d)
			my_k++;
		my_s[my_j] = malloc((my_k + 1) * sizeof(char));
		if (!my_s[my_j])
		{
			for (my_k = 0; my_k < my_j; my_k++)
				free(my_s[my_k]);
			free(my_s);
			return (NULL);
		}
		for (my_m = 0; my_m < my_k; my_m++)
			my_s[my_j][my_m] = my_str[my_i++];
		my_s[my_j][my_m] = 0;
	}
	my_s[my_j] = NULL;
	return (my_s);
}

