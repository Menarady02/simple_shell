#include "shell.h"

/**
 * my_is_cmd - determines if a file is an executable command
 * @my_info: the info struct
 * @my_path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int my_is_cmd(my_info_t *my_info, char *my_path)
{
	struct stat my_st;

	(void)my_info;
	if (!my_path || stat(my_path, &my_st))
		return (0);

	if (my_st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * my_dup_chars - duplicates characters
 * @my_pathstr: the PATH string
 * @my_start: starting index
 * @my_stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *my_dup_chars(char *my_pathstr, int my_start, int my_stop)
{
	static char my_buf[1024];
	int my_i = 0, my_k = 0;

	for (my_k = 0, my_i = my_start; my_i < my_stop; my_i++)
		if (my_pathstr[my_i] != ':')
			my_buf[my_k++] = my_pathstr[my_i];
	my_buf[my_k] = 0;
	return (my_buf);
}

/**
 * my_find_path - finds this cmd in the PATH string
 * @my_info: the info struct
 * @my_pathstr: the PATH string
 * @my_cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *my_find_path(my_info_t *my_info, char *my_pathstr, char *my_cmd)
{
	int my_i = 0, my_curr_pos = 0;
	char *my_path;

	if (!my_pathstr)
		return (NULL);
	if ((my_strlen(my_cmd) > 2) && my_starts_with(my_cmd, "./"))
	{
		if (my_is_cmd(my_info, my_cmd))
			return (my_cmd);
	}
	while (1)
	{
		if (!my_pathstr[my_i] || my_pathstr[my_i] == ':')
		{
			my_path = my_dup_chars(my_pathstr, my_curr_pos, my_i);
			if (!*my_path)
				my_strcat(my_path, my_cmd);
			else
			{
				my_strcat(my_path, "/");
				my_strcat(my_path, my_cmd);
			}
			if (my_is_cmd(my_info, my_path))
				return (my_path);
			if (!my_pathstr[my_i])
				break;
			my_curr_pos = my_i;
		}
		my_i++;
	}
	return (NULL);
}
