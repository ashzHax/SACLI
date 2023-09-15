#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <jansson.h>

#define MAX_ARG_LEN					1024

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

#define MAX_LOG_SIZE 1024

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
	int mode;
	int opt_arg_cnt;		// option argument count
	int act_arg_cnt;		// action argument count
	const char **opt_arg;	// option arguments
	const char **act_arg;	// action arguments
	char *root_path;		// subversion root dir
	char *cfg_filepath;		// configuration file path
	json_t *cfg_obj;		// json object
};

// debug functions
void	debug_log(const char* f, int l, const char* s, ...);
#define	d(s, ...) debug_log(__func__, __LINE__, s, ##__VA_ARGS__)
void out(const char* s, ...);
void errout(const char* s, ...);

// utilities functions
size_t	_strlen(const char *s);
char*	_strlow(char *s);
char*	_strcpy(const char *s, char *d, size_t t);
int		_strfcmp(const char *a, const char *b);
int		_strncmp(const char *a, const char *b, size_t t);
#endif // __GLOBAL_H__
