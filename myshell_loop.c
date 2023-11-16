#include "shell.h"

/**
 * my_hsh - main shell loop
 * @my_info: the parameter & return info struct
 * @my_av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int my_hsh(my_info_t *my_info, char **my_av)
{
	ssize_t r = 0;
	int my_builtin_ret = 0;

	while (r != -1 && my_builtin_ret != -2)
	{
		my_clear_info(my_info);
		if (my_interactive(my_info))
			my_puts("$ ");
		my_eputchar(BUF_FLUSH);
		r = my_get_input(my_info);
		if (r != -1)
		{
			my_set_info(my_info, my_av);
			my_builtin_ret = my_find_builtin(my_info);
			if (my_builtin_ret == -1)
				my_find_cmd(my_info);
		}
		else if (my_interactive(my_info))
			my_putchar('\n');
		my_free_info(my_info, 0);
	}
	my_write_history(my_info);
	my_free_info(my_info, 1);
	if (!my_interactive(my_info) && my_info->status)
		exit(my_info->status);
	if (my_builtin_ret == -2)
	{
		if (my_info->err_num == -1)
			exit(my_info->status);
		exit(my_info->err_num);
	}
	return (my_builtin_ret);
}

/**
 * my_find_builtin - finds a builtin command
 * @my_info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int my_find_builtin(my_info_t *my_info)
{
	int i, my_built_in_ret = -1;
	my_builtin_table my_builtintbl[] = {
		{"exit", my_myexit},
		{"env", my_myenv},
		{"help", my_myhelp},
		{"history", my_myhistory},
		{"setenv", my_mysetenv},
		{"unsetenv", my_myunsetenv},
		{"cd", my_mycd},
		{"alias", my_myalias},
		{NULL, NULL}
	};

	for (i = 0; my_builtintbl[i].type; i++)
		if (my_strcmp(my_info->argv[0], my_builtintbl[i].type) == 0)
		{
			my_info->line_count++;
			my_built_in_ret = my_builtintbl[i].func(my_info);
			break;
		}
	return (my_built_in_ret);
}

/**
 * my_find_cmd - finds a command in PATH
 * @my_info: the parameter & return info struct
 *
 * Return: void
 */
void my_find_cmd(my_info_t *my_info)
{
	char *my_path = NULL;
	int i, k;

	my_info->path = my_info->argv[0];
	if (my_info->linecount_flag == 1)
	{
		my_info->line_count++;
		my_info->linecount_flag = 0;
	}
	for (i = 0, k = 0; my_info->arg[i]; i++)
		if (!my_is_delim(my_info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

my_path = my_find_path(my_info, my_getenv(my_info, "PATH="), my_info->argv[0]);
	if (my_path)
	{
		my_info->path = my_path;
		my_fork_cmd(my_info);
	}
	else
	{
		if ((my_interactive(my_info) || my_getenv(my_info, "PATH=")
			|| my_info->argv[0][0] == '/') && my_is_cmd(my_info, my_info->argv[0]))
			my_fork_cmd(my_info);
		else if (*(my_info->arg) != '\n')
		{
			my_info->status = 127;
			my_print_error(my_info, "not found\n");
		}
	}
}

/**
 * my_fork_cmd - forks a an exec thread to run cmd
 * @my_info: the parameter & return info struct
 *
 * Return: void
 */
void my_fork_cmd(my_info_t *my_info)
{
	pid_t my_child_pid;

	my_child_pid = fork();
	if (my_child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (my_child_pid == 0)
	{
		if (execve(my_info->path, my_info->argv, my_get_environ(my_info)) == -1)
		{
			my_free_info(my_info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(my_info->status));
		if (WIFEXITED(my_info->status))
		{
			my_info->status = WEXITSTATUS(my_info->status);
			if (my_info->status == 126)
				my_print_error(my_info, "Permission denied\n");
		}
	}
}
