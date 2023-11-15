#include "shell.h"

/**
 * my_main - entry point
 * @my_ac: arg count
 * @my_av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int my_main(int my_ac, char **my_av)
{
	info_t my_info[] = { INFO_INIT };
	int my_fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (my_fd)
		: "r" (my_fd));

	if (my_ac == 2)
	{
		my_fd = open(my_av[1], O_RDONLY);
		if (my_fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(my_av[0]);
				_eputs(": 0: Can't open ");
				_eputs(my_av[1]);
				_eputchar('\n');
				_eputchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		my_info->readfd = my_fd;
	}
	populate_env_list(my_info);
	read_history(my_info);
	hsh(my_info, my_av);
	return (EXIT_SUCCESS);
}

