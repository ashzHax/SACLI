// by     : ashz
// reason : main code interface for handling arguments and running sub-systems
/* tab = 4 spaces */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "global.h"

#define EXIT_NORMAL					0
#define EXIT_ERROR					-1
#define EXIT_ARGUMENT_PARSE_FAIL	-2
#define EXIT_INVALID_ARGUMENT		-3

struct command_info {
	int mode;
	int opt_arg_cnt;		// option argument count
	const char **opt_arg;	// option arguments
	int act_arg_cnt;		// action argument count
	const char **act_arg;	// action arguments
};
struct command_info cmd;

// modes
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

const char* mode_list[MODE_MAX] = {
	"add",
	"remove",
	"show",
	"comment",
	"commit",
	"edit",
	"help",
	"info",
	"clean",
	"revert",
	"rollback"
};

// move to a different file
#define MAX_LOG_SIZE 1024
void debug_log(const char* f, int l, const char* s, ...)
{
	va_list list;
	char log[MAX_LOG_SIZE] = {0};
	int p = 0;

	p = snprintf(log, sizeof(log), "[%s():%d] ", f, l);

	va_start(list, s);
	vsnprintf(log+p, sizeof(log)-p, s, list);
	va_end(list);

	printf("%s\n", log);

	return;
}
#define d(s, ...) debug_log(__func__, __LINE__, s, ##__VA_ARGS__)

// move to lib :/
inline BOOL cpy_string(char *s, char *d, size_t t)
{
	int i;

	if(s == NULL || d == NULL || t <= 0) {
		return 0;
	}

	for(i=0; i<t; i++) {
		if( s[i] == '\0' ) {
			break;
		}
		d[i] = s[i];
	}
	d[i] = '\0';

	return 1;
}

/* 
 * handmade strlen() 
 *
 * note
 * Personally, I think it is a very bad idea to modify the function arguments.
 * But for fast processing, declaring a "could be saved" variable is a waste of time.
 * 
 * return
 * length of "s" (till character NULL)
 * -1 on error
*/
size_t get_str_len(const char *s)
{
	size_t len = 0;

	if(s == NULL) {
		return -1;
	}

	while(*(s++) != '\0') {
		len++;
	}
	return len;
}

// convert ASCII string to lowercase
char* cvt_str_lower(char *s)
{
	int i;
	size_t len = 0;

	if (s == NULL) {
		return NULL;
	}

	len = get_str_len(s);

	if (len <= 0) {
		return NULL;
	}

	for (i=0; i<len; i++) {
		if (s[i] >= 65 && s[i] <= 90) {
			s[i] += 32;
		}
	}

	return s;
}

// handmade strncmp, but a full compare of length.
// thinking about it, its more like Java isEqual()
// returns diff amount, 0 = the same
unsigned char chk_str_diff(const char *a, const char *b)
{
	int i;
	int len;

	if(a == NULL || b == NULL) {
		return (unsigned char)-1;
	}

	len = get_str_len(a);

	if(len != get_str_len(b)) {
		return (unsigned char)-1;
	}

	for(i=0; i<(int)len; i++) {
		if(a[i] != b[i]) {
			return (unsigned char)len-i;
		}
	}

	return (unsigned char)0;
}

/*
 * check if the variable 's' matches one of the action strings
 */
static int contains_action_string(char *s)
{
	int i = 0;

	for (i=0; i<MODE_MAX; i++) {
		if (chk_str_diff(s, mode_list[i]) == 0) {
			return 1;
		}
	}

	return 0;
}

/*
 * find the action argument, then divide option and action arguments
 */
#define MAX_ARG_LEN 1024
static int cut_from_action(int argc, char** argv)
{
	int i = 0;
	char argt[MAX_ARG_LEN] = {0};

	for (i=1; i<argc; i++) {
		strncpy(argt, argv[i], MAX_ARG_LEN); // less efficient, since always 1024 bytes are written, but it's for a few times

		// finding action parameter
		if (contains_action_string(cvt_str_lower(argt))) {
			break;
		}
		cmd.opt_arg_cnt++;
	}

	cmd.opt_arg = (const char**)malloc(sizeof(const char*)*cmd.opt_arg_cnt);
	for(i=0; i<cmd.opt_arg_cnt; i++) {
		cmd.opt_arg[i] = argv[i+1];
	}
	for(i=1; i<(cmd.opt_arg_cnt+1); i++) {
		d("opt_args = %s", cmd.opt_arg[i]);
	}

		
		// append option parameters
	// get action parameter

	// test case, move this to DEINIT
	free(cmd.opt_arg);

	return 1;
}

int option_parse(int argc, char** argv)
{
#if DEBUG_MODE // print argument list
	int i = 0;

	for(i = 0; i < argc; i++) {
		printf("[%d] - argv[%s]\n", i, argv[i]);
	}
#endif // DEBUG_MODE
	
	cut_from_action(argc, argv);

	if (chk_str_diff(argv[1], "add") == 0) { // if same
		printf("found ADD!!!!\n");
		d("\"ADD\" command detected");
	}

	return 0;
}

int main(int argc, char** argv)
{
	int mode = MODE_INVALID;

	mode = option_parse(argc, argv);
	switch(mode) {
		case MODE_ADD:
		{
			break;
		}
	}

	d("Finished. [^.^]");
	return 0;
}
