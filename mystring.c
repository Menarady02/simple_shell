#ifndef _MY_SHELL_H_
#define _MY_SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define MY_READ_BUF_SIZE 1024
#define MY_WRITE_BUF_SIZE 1024
#define MY_BUF_FLUSH -1

/* for command chaining */
#define MY_CMD_NORM	0
#define MY_CMD_OR	1
#define MY_CMD_AND	2
#define MY_CMD_CHAIN	3

/* for my_convert_number() */
#define MY_CONVERT_LOWERCASE	1
#define MY_CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define MY_USE_GETLINE 0
#define MY_USE_STRTOK 0

#define MY_HIST_FILE	".my_simple_shell_history"
#define MY_HIST_MAX	4096

extern char **my_environ;

/**
 * struct my_liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct my_liststr
{
	int num;
	char *str;
	struct my_liststr *next;
} my_list_t;

/**
 *struct my_passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@my_arg: a string generated from getline containing arguements
 *@my_argv: an array of strings generated from arg
 *@my_path: a string path for the current command
 *@my_argc: the argument count
 *@my_line_count: the error count
 *@my_err_num: the error code for exit()s
 *@my_linecount_flag: if on count this line of input
 *@my_fname: the program filename
 *@my_env: linked list local copy of environ
 *@my_environ: custom modified copy of environ from LL env
 *@my_history: the history node
 *@my_alias: the alias node
 *@my_env_changed: on if environ was changed
 *@my_status: the return status of the last exec'd command
 *@my_cmd_buf: address of pointer to cmd_buf, on if chaining
 *@my_cmd_buf_type: MY_CMD_type ||, &&, ;
 *@my_readfd: the fd from which to read line input
 *@my_histcount: the history line number count
 */
typedef struct my_passinfo
{
	char *my_arg;
	char **my_argv;
	char *my_path;
	int my_argc;
	unsigned int my_line_count;
	int my_err_num;
	int my_linecount_flag;
	char *my_fname;
	my_list_t *my_env;
	my_list_t *my_history;
	my_list_t *my_alias;
	char **my_environ;
	int my_env_changed;
	int my_status;

	char **my_cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int my_cmd_buf_type; /* MY_CMD_type ||, &&, ; */
	int my_readfd;
	int my_histcount;
} my_info_t;

#define MY_INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct my_builtin - contains a builtin string and related function
 *@my_type: the builtin command flag
 *@my_func: the function
 */
typedef struct my_builtin
{
	char *my_type;
	int (*my_func)(my_info_t *);
} my_builtin_table;

/* my_toem_shloop.c */
int my_hsh(my_info_t *, char **);
int my_find_builtin(my_info_t *);
void my_find_cmd(my_info_t *);
void my_fork_cmd(my_info_t *);

/* my_toem_parser.c */
int my_is_cmd(my_info_t *, char *);
char *my_dup_chars(char *, int, int);
char *my_find_path(my_info_t *, char *, char *);

/* my_loophsh.c */
int my_loophsh(char **);

/* my_toem_errors.c */
void my_eputs(char *);
int my_eputchar(char);
int my_putfd(char c, int fd);
int my_putsfd(char *str, int fd);

/* my_toem_string.c */
int my_strlen(char *);
int my_strcmp(char *, char *);
char *my_starts_with(const char *, const char *);
char *my_strcat(char *, char *);

/* my_toem_string1.c */
char *my_strcpy(char *, char *);
char *my_strdup(const char *);
void my_puts(char *);
int my_putchar(char);

/* my_toem_exits.c */
char *my_strncpy(char *, char *, int);
char *my_strncat(char *, char *, int);
char *my_strchr(char *, char);

/* my_toem_tokenizer.c */
char **my_strtow(char *, char *);
char **my_strtow2(char *, char);

/* my_toem_realloc.c */
char *my_memset(char *, char, unsigned int);
void my_ffree(char **);
void *my_realloc(void *, unsigned int, unsigned int);

/* my_toem_memory.c */
int my_bfree(void **);

/* my_toem_atoi.c */
int my_interactive(my_info_t *);
int my_is_delim(char, char *);
int my_isalpha(int);
int my_atoi(char *);

/* my_toem_errors1.c */
int my_erratoi(char *);
void my_print_error(my_info_t *, char *);
int my_print_d(int, int);
char *my_convert_number(long int, int, int);
void my_remove_comments(char *);

/* my_toem_builtin.c */
int my_myexit(my_info_t *);
int my_mycd(my_info_t *);
int my_myhelp(my_info_t *);

/* my_toem_builtin1.c */
int my_myhistory(my_info_t *);
int my_myalias(my_info_t *);

/*my_toem_getline.c */
ssize_t my_get_input(my_info_t *);
int my_getline(my_info_t *, char **, size_t *);
void my_sigintHandler(int);

/* my_toem_getinfo.c */
void my_clear_info(my_info_t *);
void my_set_info(my_info_t *, char **);
void my_free_info(my_info_t *, int);

/* my_toem_environ.c */
char *my_getenv(my_info_t *, const char *);
int my_myenv(my_info_t *);
int my_mysetenv(my_info_t *);
int my_myunsetenv(my_info_t *);
int my_populate_env_list(my_info_t *);

/* my_toem_getenv.c */
char **my_get_environ(my_info_t *);
int my_unsetenv(my_info_t *, char *);
int my_setenv(my_info_t *, char *, char *);

/* my_toem_history.c */
char *my_get_history_file(my_info_t *my_info);
int my_write_history(my_info_t *my_info);
int my_read_history(my_info_t *my_info);
int my_build_history_list(my_info_t *my_info, char *buf, int linecount);
int my_renumber_history(my_info_t *my_info);

/* my_toem_lists.c */
my_list_t *my_add_node(my_list_t **, const char

