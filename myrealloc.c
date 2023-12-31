#include "myshell.h"

/**
 * my_memset - fills memory with a constant byte
 * @my_s: the pointer to the memory area
 * @my_b: the byte to fill *my_s with
 * @my_n: the amount of bytes to be filled
 * Return: (my_s) a pointer to the memory area my_s
 */
char *my_memset(char *my_s, char my_b, unsigned int my_n)
{
	unsigned int my_i;

	for (my_i = 0; my_i < my_n; my_i++)
		my_s[my_i] = my_b;
	return (my_s);
}

/**
 * my_ffree - frees a string of strings
 * @my_pp: string of strings
 */
void my_ffree(char **my_pp)
{
	char **my_a = my_pp;

	if (!my_pp)
		return;
	while (*my_pp)
		free(*my_pp++);
	free(my_a);
}

/**
 * my_realloc - reallocates a block of memory
 * @my_ptr: pointer to previous malloc'ated block
 * @my_old_size: byte size of previous block
 * @my_new_size: byte size of new block
 * Return: pointer to da ol'block nameen.
 */
void *my_realloc(void *my_ptr, unsigned int my_old_size,
		unsigned int my_new_size)
{
	char *my_p;

	if (!my_ptr)
		return (malloc(my_new_size));
	if (!my_new_size)
		return (free(my_ptr), NULL);
	if (my_new_size == my_old_size)
		return (my_ptr);

	my_p = malloc(my_new_size);
	if (!my_p)
		return (NULL);

	my_old_size = my_old_size < my_new_size ? my_old_size : my_new_size;
	while (my_old_size--)
		my_p[my_old_size] = ((char *)my_ptr)[my_old_size];
	free(my_ptr);
	return (my_p);
}
