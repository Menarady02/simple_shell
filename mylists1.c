#include "shell.h"

/**
 * my_list_len - determines length of linked list
 * @my_h: pointer to first node
 *
 * Return: size of list
 */
size_t my_list_len(const list_t *my_h)
{
	size_t i = 0;

	while (my_h)
	{
		my_h = my_h->next;
		i++;
	}
	return (i);
}

/**
 * my_list_to_strings - returns an array of strings of the my_list->str
 * @my_head: pointer to first node
 *
 * Return: array of strings
 */
char **my_list_to_strings(list_t *my_head)
{
	list_t *node = my_head;
	size_t i = my_list_len(my_head), j;
	char **strs;
	char *str;

	if (!my_head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(_my_strlen(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = _my_strcpy(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}

/**
 * my_print_list - prints all elements of a list_t linked list
 * @my_h: pointer to first node
 *
 * Return: size of list
 */
size_t my_print_list(const list_t *my_h)
{
	size_t i = 0;

	while (my_h)
	{
		_my_puts(convert_number(my_h->num, 10, 0));
		_my_putchar(':');
		_my_putchar(' ');
		_my_puts(my_h->str ? my_h->str : "(nil)");
		_my_puts("\n");
		my_h = my_h->next;
		i++;
	}
	return (i);
}

/**
 * my_node_starts_with - returns node whose string starts with prefix
 * @my_node: pointer to my_list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
list_t *my_node_starts_with(list_t *my_node, char *prefix, char c)
{
	char *p = NULL;

	while (my_node)
	{
		p = my_starts_with(my_node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (my_node);
		my_node = my_node->next;
	}
	return (NULL);
}

/**
 * my_get_node_index - gets the index of a node
 * @my_head: pointer to my_list head
 * @my_node: pointer to the my_node
 *
 * Return: index of my_node or -1
 */
ssize_t my_get_node_index(list_t *my_head, list_t *my_node)
{
	size_t i = 0;

	while (my_head)
	{
		if (my_head == my_node)
			return (i);
		my_head = my_head->next;
		i++;
	}
	return (-1);
}

