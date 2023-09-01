/* 
 * by     : ashz
 * reason : main code interface for handling arguments and running sub-systems
 * tab    : 4 spaces, 1 tab character
 */

#include <stdio.h>
#include <stdlib.h>
#include "global.h"

struct command_info cmd;

// string list of command modes, for sifting user input
// list order must match enum defined in global.h
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

/*
 * description
 * Checks if the variable 's' matches one of the action strings
 */
static int contains_action_string(char *s)
{
	int i = 0;

	for (i=0; i<MODE_MAX; i++) {
		if (_strfcmp(s, mode_list[i]) == 0) {
			cmd.mode = i;
			return 1;
		}
	}

	return 0;
}

/*
 * description
 * Finds the action argument, then divide option and action
 * arguments
 */
static void option_parse(int argc, char** argv)
{
	int i = 0;
	char argt[MAX_ARG_LEN] = {0};
	int idx_action = 0;

	// debugging purposes
	for(i = 0; i < argc; i++) {
		d("argv[%d] = %s", i, argv[i]);
	}

	for (i=1; i<argc; i++) {
		// less efficient, since doing it this way makes 1024 bytes be written
		// all the time, but this function only runs once at init
		_strcpy(argv[i], argt, MAX_ARG_LEN); 

		// finding action parameter
		if (contains_action_string(_strlow(argt))) {
			cmd.act_arg_cnt = argc - i - 1; // cannot go below 0 (in theory)
			idx_action = i;
			break;
		}
		cmd.opt_arg_cnt++;
	}
	d("idx_action = %d, act_arg_cnt=%d", idx_action, cmd.act_arg_cnt);

	cmd.opt_arg = (const char**)malloc(sizeof(const char*)*cmd.opt_arg_cnt);
	for (i=0; i<cmd.opt_arg_cnt; i++) {
		cmd.opt_arg[i] = argv[i+1];
		d("opt_args[%d] = argv[%d] = %s", i, i+1, argv[i+1]);
	}

	cmd.act_arg = (const char**)malloc(sizeof(const char*)*cmd.act_arg_cnt);
	for (i=0; i<cmd.act_arg_cnt; i++) {
		cmd.act_arg[i] = argv[idx_action+1+i];
		d("act_args[%d] = argv[%d] = %s", i, idx_action+1+i, argv[idx_action+1+i]);
	}
}

static void help()
{
	out("usage: svm [options] [action] [...]");
}

static void init()
{
	cmd.mode = MODE_INVALID;
}

static void deinit()
{
	free(cmd.opt_arg);
	free(cmd.act_arg);
}

int main(int argc, char** argv)
{
	int exit_code;

	init();

	option_parse(argc, argv);

	switch(cmd.mode) {
		case MODE_ADD:
		{
			d("running \"ADD\" function");
			break;
		}
		default:
		{
			help();
			exit_code = EXIT_ACTION_NOT_FOUND;
			d("found no action argument, ending program");
		}
	}

	deinit();
	out("Finished. [^.^]");

	return exit_code;
}
