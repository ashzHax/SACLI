#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <jansson.h>
#include <unistd.h>

// TODO: needs better implementation, preferably to a Makefile define
#define CFG_FILE ".c.json"

#define ASCII_ALPHA_UP_START		65
#define ASCII_ALPHA_UP_END			90
#define ASCII_ALPHA_CASE_DIFF		32

#define EXIT_NORMAL					0
#define EXIT_ERROR					-1
#define EXIT_ACTION_NOT_FOUND		-2
#define EXIT_INVALID_OPTION_ARG		-3
#define EXIT_INVALID_ACTION_ARG		-3
#define EXIT_NOT_IN_SVN_REPO		-4
#define EXIT_UNABLE_TO_CREATE_FILE	-5
#define EXIT_UNABLE_TO_OPEN_FILE	-6
#define EXIT_INIT_FAILED			-7
#define EXIT_FAILED_TO_GET_CONFIG	-8
#define EXIT_HELP_PAGE				-9
#define EXIT_NO_GROUP_NAME			-10
#define EXIT_UNKNOWN_OPTION			-11
#define EXIT_INVALID_OPTION			-12

#define MAX_ARG_LEN 1024
#define MAX_PATH_LEN 1024
#define MAX_GROUP_NAME_LEN 128

// command modes, depending on 'action' argument user input
// enum order must match list defined in global.c
enum {
	MODE_INVALID = -1,
	MODE_ADD = 0,
	MODE_REMOVE,
	MODE_SHOW,
	MODE_COMMENT,
	MODE_COMMIT,
	MODE_EDIT,
	MODE_HELP,
	MODE_INFO,
	MODE_CLEAN,
	MODE_REVERT,
	MODE_ROLLBACK,
	MODE_MAX
};

struct command_info {
	int				mode;
	int				opt_arg_cnt;					// option argument count
	int				act_arg_cnt;					// action argument count
	const char**	opt_arg;						// option arguments
	const char**	act_arg;						// action arguments
	char*			svn_root_path;					// subversion root dir
	char*			config_path;					// configuration file path
	json_t*			j_config;						// json object for holding configuration value
	char			cwd[MAX_PATH_LEN];				// current working directory, without root path
	char			group_name[MAX_GROUP_NAME_LEN];	// Name of command target group
	json_t*			j_group;						// command target group
};

// utilities functions
size_t	_strlen(const char *s);
char*	_strlow(char *s);
char*	_strcpy(const char *s, char *d, size_t t);
int		_strfcmp(const char *a, const char *b);
int		_strncmp(const char *a, const char *b, size_t t);
void	rm_whitespace(char *s);
char*	add_end_dir_slash(char* s, size_t max);
#endif // __GLOBAL_H__
