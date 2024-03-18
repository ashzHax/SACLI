#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdio.h>
#include <jansson.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <log.h>

#define CFG_FILE ".svm.json" // config file
#define TRG_FILE ".svm.target.txt" // target file
#define CMT_FILE ".svm.comment.%s.txt" // comment file
#define COT_FILE ".svm.commit.%s.txt" // commit file

#define ASCII_ALPHA_UP_START		65
#define ASCII_ALPHA_UP_END			90
#define ASCII_ALPHA_CASE_DIFF		32

#define EXIT_NORMAL								0
#define EXIT_ERROR								-1
#define EXIT_ACTION_NOT_FOUND					-2
#define EXIT_INVALID_OPTION_ARG					-3
#define EXIT_INVALID_ACTION_ARG					-3
#define EXIT_SVN_PATH_GET_FAIL					-4
#define EXIT_UNABLE_TO_CREATE_FILE				-5
#define EXIT_UNABLE_TO_OPEN_FILE				-6
#define EXIT_INIT_FAILED						-7
#define EXIT_CONFIG_GET_FAIL					-8
#define EXIT_HELP_PAGE							-9
#define EXIT_NO_GROUP_NAME						-10
#define EXIT_ERROR_UNKNOWN_OPTION				-11
#define EXIT_INVALID_OPTION						-12
#define EXIT_ADD_ERROR							-13
#define EXIT_COMMENT_ERROR						-14
#define EXIT_COMMIT_ERROR						-15
#define EXIT_CLEAR_ERROR						-16
#define EXIT_OW_ERROR							-17
#define EXIT_OW_ERROR_NOT_ENOUGH_ARGS			-18
#define EXIT_OW_ERROR_TOO_MANY_ARGS				-19
#define EXIT_EDIT_ERROR							-20
#define EXIT_REVERT_ERROR						-21
#define EXIT_ROLLBACK_ERROR						-22
#define EXIT_ROLLBACK_NOT_ENOUGH_ARGS			-23
#define EXIT_AUTO_ERROR							-24
#define EXIT_TARGET_ERROR						-25
#define EXIT_FAILED_TO_CREATE_GROUP				-26
#define EXIT_ERROR_GROUP_GET_FAILED				-27
#define EXIT_CONFIG_FILE_OPEN_FAIL				-28
#define EXIT_GET_CWD_NO_RD_FAIL					-29
#define EXIT_AUTO_FILE_GET_FAIL					-30
#define EXIT_ADD_ERROR_INVALID_PATH				-31
#define EXIT_ADD_ERROR_DUPLICATE_PATH			-32
#define EXIT_REMOVE_ERROR						-33
#define EXIT_REMOVE_ERROR_FILE_NOT_FOUND		-34
#define EXIT_REMOVE_ERROR_JSON_REMOVE_FAIL		-35
#define EXIT_AUTO_ERROR_FILE_OPEN_FAIL			-36
#define EXIT_COMMENT_ERROR_FILE_OPEN_FAIL		-37
#define EXIT_COMMIT_ERROR_FILE_OPEN_FAIL		-38
#define EXIT_OW_ERROR_GROUP_NOT_FOUND			-39
#define EXIT_OW_ERROR_NO_SRC_FILES				-40
#define EXIT_OW_ERROR_NO_COMMENT_FILE			-41
#define EXIT_ROLLBACK_ERROR_INVALID_REVISION	-42
#define EXIT_SHOW_ERROR							-43
#define EXIT_INFO_ERROR							-44

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
	MODE_REVERT,
	MODE_ROLLBACK,
	MODE_AUTO,
	MODE_TARGET,
	MODE_INFO,
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
	char			root_path[MAX_PATH_LEN];		// subversion root directory path
	char			config_path[MAX_PATH_LEN];		// SVM configuration file path
	char			cwd[MAX_PATH_LEN];				// current working directory (without root path)
	json_t*			json_cfg;						// json object for holding configuration value (cfg->root)
	char			grp_name[MAX_GROUP_NAME_LEN];	// Name of command target group
	json_t*			json_grp;						// target group (cfg->root->group->target_group)
	json_t*			json_files;						// target group file array (cfg->root->group->target_group->files)
	char			auto_path[MAX_PATH_LEN];		// 'auto' command target list
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
char*	get_clean_path(struct command_info* c, const char *p, char *f, size_t f_sz);
char*	rm_slash(char* p);
#endif // __GLOBAL_H__
