#include "myshell.h"

/**
 * my_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int my_history(info_t *info)
{
	my_print_list(info->my_history);
	return (0);
}

/**
 * my_unset_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int my_unset_alias(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = my_delete_node_at_index(&(info->my_alias),
my_get_node_index(info->my_alias,
	my_node_starts_with(info->my_alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * my_set_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int my_set_alias(info_t *info, char *str)
{
	char *p;

	p = strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (my_unset_alias(info, str));

	my_unset_alias(info, str);
	return (my_add_node_end(&(info->my_alias), str, 0) == NULL);
}

/**
 * my_print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int my_print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			putchar(*a);
		putchar('\'');
		puts(p + 1);
		puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * my_alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int my_alias(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->my_argc == 1)
	{
		node = info->my_alias;
		while (node)
		{
			my_print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->my_argv[i]; i++)
	{
		p = strchr(info->my_argv[i], '=');
		if (p)
			my_set_alias(info, info->my_argv[i]);
		else
			my_print_alias(my_node_starts_with(info->my_alias, info->my_argv[i], '='));
	}

	return (0);
}

