#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdio.h>
#include <jansson.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <log.h>

// TODO: needs better implementation, preferably to a Makefile define
#define CFG_FILE ".c.json"

#define ASCII_ALPHA_UP_START		65
#define ASCII_ALPHA_UP_END			90
#define ASCII_ALPHA_CASE_DIFF		32

#define EXIT_NORMAL						0
#define EXIT_ERROR						-1
#define EXIT_ACTION_NOT_FOUND			-2
#define EXIT_INVALID_OPTION_ARG			-3
#define EXIT_INVALID_ACTION_ARG			-3
#define EXIT_NOT_IN_SVN_REPO			-4
#define EXIT_UNABLE_TO_CREATE_FILE		-5
#define EXIT_UNABLE_TO_OPEN_FILE		-6
#define EXIT_INIT_FAILED				-7
#define EXIT_FAILED_TO_GET_CONFIG		-8
#define EXIT_HELP_PAGE					-9
#define EXIT_NO_GROUP_NAME				-10
#define EXIT_UNKNOWN_OPTION				-11
#define EXIT_INVALID_OPTION				-12
#define EXIT_ADD_FAILED					-13
#define EXIT_COMMENT_FAILED				-14
#define EXIT_COMMIT_FAILED				-15
#define EXIT_CLEAR_FAILED				-16
#define EXIT_OVERWRITE_FAILED			-17
#define EXIT_OVERWRITE_NOT_ENOUGH_ARGS	-18
#define EXIT_OVERWRITE_TOO_MANY_ARGS	-19

#define MAX_ARG_LEN 1024
#define MAX_PATH_LEN 1024
#define MAX_GROUP_NAME_LEN 128
#define MAX_CMD_LEN 1024
#define MAX_SCHED_LEN 32
#define MAX_FILENAME_LEN 256
#define MAX_CMNT_WORD_LEN 128 // max comment word length

// command modes, depending on 'action' argument user input
// enum order must match list defined in global.c
enum {
	MODE_INVALID = -1,
	MODE_ADD = 0,
	MODE_ADD_SHORT,
	MODE_REMOVE,
	MODE_REMOVE_SHORT,
	MODE_SHOW,
	MODE_COMMENT,
	MODE_COMMIT,
	MODE_CLEAR,
	MODE_OVERWRITE,
	MODE_OVERWRITE_SHORT,
	MODE_EDIT,
	MODE_HELP,
	MODE_INFO,
	MODE_REVERT,
	MODE_ROLLBACK,
	MODE_MAX
};

// SVN file status
enum {
	SVN_SCHED_NORMAL = 0,
	SVN_SCHED_ADD,
	SVN_SCHED_DELETE,
	SVN_SCHED_UNDEFINED
};

struct command_info {
	int				mode;
	int				opt_arg_cnt;					// option argument count
	int				act_arg_cnt;					// action argument count
	const char**	opt_arg;						// option arguments
	const char**	act_arg;						// action arguments
	char			svn_root_path[MAX_PATH_LEN];	// subversion root dir
	char			config_path[MAX_PATH_LEN];		// configuration file path
	json_t*			j_config;						// json object for holding configuration value
	char			cwd[MAX_PATH_LEN];				// current working directory, without root path
	char			group_name[MAX_GROUP_NAME_LEN];	// Name of command target group
	json_t*			j_group;						// target group
	json_t*			j_files;						// target group file array
	json_t*			j_comment;						// target group comment
};

// utilities functions
size_t	_strlen(const char *s);
char*	_strlow(char *s);
char*	_strcpy(const char *s, char *d, size_t t);
int		_strfcmp(const char *a, const char *b);
int		_strncmp(const char *a, const char *b, size_t t);
char*	rm_whitespace(char *s);
char*	add_end_dir_slash(char* s, size_t max);
int		get_file_svn_schedule(const char *s);
char*	ss_find_nth(int nth, const char* s, const char* ss);
int		ss_cnt(const char* s, const char* ss);
int		get_clean_path(struct command_info* c, const char *p, char *f, size_t f_sz);
#endif // __GLOBAL_H__
