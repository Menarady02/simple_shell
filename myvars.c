#include "shell.h"

/**
 * my_is_chain - test if current char in buffer is a chain delimiter
 * @my_info: the parameter struct
 * @my_buf: the char buffer
 * @my_p: address of current position in buf
 *
 * Return: 1 if chain delimiter, 0 otherwise
 */
int my_is_chain(my_info_t *my_info, char *my_buf, size_t *my_p)
{
    size_t my_j = *my_p;

    if (my_buf[my_j] == '|' && my_buf[my_j + 1] == '|')
    {
        my_buf[my_j] = 0;
        my_j++;
        my_info->my_cmd_buf_type = CMD_OR;
    }
    else if (my_buf[my_j] == '&' && my_buf[my_j + 1] == '&')
    {
        my_buf[my_j] = 0;
        my_j++;
        my_info->my_cmd_buf_type = CMD_AND;
    }
    else if (my_buf[my_j] == ';') /* found end of this command */
    {
        my_buf[my_j] = 0; /* replace semicolon with null */
        my_info->my_cmd_buf_type = CMD_CHAIN;
    }
    else
        return (0);
    *my_p = my_j;
    return (1);
}

/**
 * my_check_chain - checks we should continue chaining based on last status
 * @my_info: the parameter struct
 * @my_buf: the char buffer
 * @my_p: address of current position in buf
 * @my_i: starting position in buf
 * @my_len: length of buf
 *
 * Return: Void
 */
void my_check_chain(my_info_t *my_info, char *my_buf, size_t *my_p, size_t my_i, size_t my_len)
{
    size_t my_j = *my_p;

    if (my_info->my_cmd_buf_type == CMD_AND)
    {
        if (my_info->my_status)
        {
            my_buf[my_i] = 0;
            my_j = my_len;
        }
    }
    if (my_info->my_cmd_buf_type == CMD_OR)
    {
        if (!my_info->my_status)
        {
            my_buf[my_i] = 0;
            my_j = my_len;
        }
    }

    *my_p = my_j;
}

/**
 * my_replace_alias - replaces an aliases in the tokenized string
 * @my_info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int my_replace_alias(my_info_t *my_info)
{
    int my_i;
    my_list_t *my_node;
    char *my_p;

    for (my_i = 0; my_i < 10; my_i++)
    {
        my_node = my_node_starts_with(my_info->my_alias, my_info->my_argv[0], '=');
        if (!my_node)
            return (0);
        free(my_info->my_argv[0]);
        my_p = my_strchr(my_node->my_str, '=');
        if (!my_p)
            return (0);
        my_p = my_strdup(my_p + 1);
        if (!my_p)
            return (0);
        my_info->my_argv[0] = my_p;
    }
    return (1);
}

/**
 * my_replace_vars - replaces vars in the tokenized string
 * @my_info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int my_replace_vars(my_info_t *my_info)
{
    int my_i = 0;
    my_list_t *my_node;

    for (my_i = 0; my_info->my_argv[my_i]; my_i++)
    {
        if (my_info->my_argv[my_i][0] != '$' || !my_info->my_argv[my_i][1])
            continue;

        if (!my_strcmp(my_info->my_argv[my_i], "$?"))
        {
            my_replace_string(&(my_info->my_argv[my_i]),
                               my_strdup(my_convert_number(my_info->my_status, 10, 0)));
            continue;
        }
        if (!my_strcmp(my_info->my_argv[my_i], "$$"))
        {
            my_replace_string(&(my_info->my_argv[my_i]),
                               my_strdup(my_convert_number(my_getpid(), 10, 0)));
            continue;
        }
        my_node = my_node_starts_with(my_info->my_env, &my_info->my_argv[my_i][1], '=');
        if (my_node)
        {
            my_replace_string(&(my_info->my_argv[my_i]),
                               my_strdup(my_strchr(my_node->my_str, '=') + 1));
            continue;
        }
        my_replace_string(&my_info->my_argv[my_i], my_strdup(""));
    }
    return (0);
}

/**
 * my_replace_string - replaces string
 * @my_old: address of old string
 * @my_new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int my_replace_string(char **my_old, char *my_new)
{
    free(*my_old);
    *my_old = my_new;
    return (1);
}

