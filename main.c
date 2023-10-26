/* 
 * by     : ashz
 * reason : main code interface for handling arguments and running sub-systems
 * tab    : 4 spaces, 1 tab character
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <jansson.h>
#include <global.h>
#include <log.h>

extern int _add(struct command_info* c);
extern int _remove(struct command_info* c);
extern int _show(struct command_info* c);

/*
 * note
 * string list of command modes, for sifting user input
 * list order must match enum defined in global.h
 */
const char* mode_list[MODE_MAX] = {
	"add",
	"a",
	"remove",
	"rm",
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
 * Show the user how to use this amazing command
 */
static void help()
{
	out("usage: svm [options] [action] [...]");
}

/*
 * description
 * Save 'c->j_group' as 'c->group_name' to c->j_config.
 * Dump 'c->j_config' to file.
 */
static void save_config(struct command_info *c)
{
	json_t *group_list = NULL;

	if (c == NULL) return;

	group_list = json_object_get(c->j_config, "groups");
	if (group_list == NULL) {
		errout("Unable to get 'groups' object, critical error");
		return;
	}

	json_object_set(group_list, c->group_name, c->j_group);
	dj(c->j_config);

	if (json_dump_file(c->j_config, c->config_path, JSON_INDENT(1)) < 0) {
		errout("Failed to dump configuration data to file, critical error");
	} else {
		d("Data dump successful");
	}
}

/*
 * description
 * Finds the action argument, then divide option and action arguments
 * 
 * return
 * '0' on success
 * '>0' on fail (will be program exit code)
 */
static int option_parse(struct command_info *c, int argc, char** argv)
{
	int i = 0;
	int j = 0;
	char argt[MAX_ARG_LEN] = {0};
	int act = 0;
	json_t *group_list = NULL;

	for (i=1; i<argc; i++) {
		_strcpy(argv[i], argt, MAX_ARG_LEN); 
		_strlow(argt);
		d("Checking argument [%s]", argt);

		// finding action parameter
		for (j=0; j<MODE_MAX; j++) {
			if (_strfcmp(argt, mode_list[j]) == 0) {
				c->mode = j;
				break;
			}
		}

		if (c->mode != MODE_INVALID) {
			c->act_arg_cnt = argc - i - 1; // cannot go below 0 (in theory)
			act = i;
			break;
		}
		c->opt_arg_cnt++;
	}
	d("act[%d] opt_cnt[%d] act_cnt[%d]", act, c->opt_arg_cnt, c->act_arg_cnt);

	if (c->mode == MODE_INVALID) {
		return EXIT_ACTION_NOT_FOUND;
	}

	c->opt_arg = (const char**)malloc(sizeof(const char*)*c->opt_arg_cnt);
	for (i=0; i < c->opt_arg_cnt; i++) {
		c->opt_arg[i] = argv[i+1];
		d("opt_args[%d] = argv[%d] = %s", i, i+1, argv[i+1]);
	}

	c->act_arg = (const char**)malloc(sizeof(const char*)*c->act_arg_cnt);
	for (i=0; i < c->act_arg_cnt; i++) {
		c->act_arg[i] = argv[act+1+i];
		d("act_args[%d] = argv[%d] = %s", i, act+1+i, argv[act+1+i]);
	}

	// Now, parsing the options. How to parse the options?
	// First, organize all options into the proper data structure.
	// Get the option indicator, then parse the backdata (if it exists) to a variable.
	// Second, make sure the options received are valid for the current mode.
	// Third, check the validity of the data received
	// Fourth, if data is not used else where, use it now
	for (i=1; i < c->opt_arg_cnt+1; i++) {
		if (_strncmp(argv[i], "-h", (size_t)2) == 0) {
			d("Got [-h] option!");
			return EXIT_HELP_PAGE;
		} else if (_strncmp(argv[i], "-g", (size_t)2) == 0) {
			d("Got [-g] option!");

			if (_strlen(argv[i]) > 2) { // group name is in current option argument
				snprintf(c->group_name, MAX_GROUP_NAME_LEN, "%s", argv[i]+2);
			} else { // group name is in a separate option argument
				if (i >= c->opt_arg_cnt) { // check if there are no more arguments
					return EXIT_NO_GROUP_NAME;
				}
				i++;
				snprintf(c->group_name, MAX_GROUP_NAME_LEN, "%s", argv[i]);
			}
			d("Option group name [%s]", c->group_name);
			
			// Step 2: Check if option is valid for current mode
			switch (c->mode) {
				case MODE_ADD:
				case MODE_REMOVE:
				case MODE_SHOW:
				case MODE_COMMENT:
				case MODE_COMMIT:
				case MODE_EDIT:
				case MODE_CLEAN:
				case MODE_REVERT: {
					break;
				}
				default: {
					errout("Invalid option for this action. [%s]", argv[i]);
					return EXIT_INVALID_OPTION;
				}
			}

			// Step 3: Check validity of received data
			group_list = json_object_get(c->j_config, "groups");
			c->j_group = json_object_get(group_list, c->group_name);

			if (c->j_group == NULL) { // if group is not found, create it
				c->j_group = json_array();
				if (c->j_group != NULL) {
					alert("Created new group! [%s] Don't forget it [>.<]", c->group_name);
				} else {
					errout("Unable to create new group. [%s][^.-]", c->group_name);
					return EXIT_ERROR;
				}
			}
		} else {
			errout("IDK what this option is. [%s]", argv[i]);
			return EXIT_UNKNOWN_OPTION;
		}
	}

	// if no group was found by this point, set 'j_config' to default group
	if (c->j_group == NULL) {
		snprintf(c->group_name, MAX_GROUP_NAME_LEN, "default");

		group_list = json_object_get(c->j_config, "groups");
		c->j_group = json_object_get(group_list, c->group_name);

		if (c->j_group == NULL) {
			errout("Unable to get default group!");
			return EXIT_ERROR;
		}
	}

	return 0;
}

/*
 * description
 * Finds the root directory of the current SVN repository
 *
 * return
 * '0' is returned on success
 * '-1' on fail
 */
static int get_svn_rd(char *buf, size_t len)
{
	FILE *pp = NULL;

	if (buf == NULL || len <= 0) return -1;

	pp = popen("svn info 2> /dev/null | grep \"Working Copy Root Path\" | awk \'{print $5}\'", "r");
	if (pp) {
		fgets(buf, len, pp);
		pclose(pp);
	}

	if (_strlen(buf) <= 0) return -1;

	rm_whitespace(buf);
	add_end_dir_slash(buf, len);

	d("SVN root directory path [%s]", buf);

	return 0;
}

/*
 * description
 * Creates the configuration file, and fills it with the default values
 */
static int create_file(struct command_info *c)
{
	FILE *fp = NULL;
	json_t *new = NULL;
	json_t *group = NULL;

	fp = fopen(c->config_path, "w+");
	if (fp == NULL) {
		errout("failed to open/create configuration file");
		return -1;
	}

	new = json_object();
	group = json_object();

	if (new == NULL || group == NULL) {
		errout("failed to create json object");
		return -1;
	}

	// OBJECT -> groups -> default -> []
	json_object_set(group, "default", json_array());
	json_object_set(new, "groups", group);

	json_dumpf(new, fp, JSON_INDENT(4));

	json_decref(new);
	fclose(fp);

	return 0;
}

/*
 * description
 * For getting configuration file.
 * Creates the file with default values if it doesn't exists with default
 * JSON values.
 * Reads the file and parse it into a 'json_t' object.
 *
 * return
 * '0' on success
 * '-1' on error
 */
static int get_config(struct command_info *c)
{
	FILE *fp = NULL;
	json_error_t error; // jansson error handling
	int len = 0;
	
	// get configuration file path
	if (c->config_path == NULL) {
		errout("malloc() failed");
		return -1;
	}

	len = _strlen(c->svn_root_path)+_strlen(CFG_FILE)+2;
	snprintf(c->config_path, len, "%s%s", c->svn_root_path, CFG_FILE);
	d("Configuration file path [%s]", c->config_path);

	// check if configuration file exists
	// if not, create file with default values
	if (access(c->config_path, F_OK) != 0) {
		if (create_file(c) < 0) {
			return -1;
		}
	}

	// open configuration file
	fp = fopen(c->config_path, "r");
	if (fp == NULL) {
		errout("failed to open configuration file");
		return -1;
	}

	// load configuration file as JSON
	c->j_config = json_loadf(fp, (JSON_DECODE_ANY | JSON_DISABLE_EOF_CHECK), &error);
	fclose(fp);
	dj(c->j_config);

	return 0;
}

/*
 * description
 * Gets the current working directory, but removes inital 'root' if it exists
 *
 * return
 * The parameter buffer pointer is returned on success
 * NULL is returned on fail
 */
static char* get_cwd_no_root(char* s, size_t n, char* root)
{
	char path[MAX_PATH_LEN] = {0};
	size_t root_len = 0;
	size_t path_len = 0;

	if (s == NULL || n <= 0) return NULL;

	if (root != NULL) {
		root_len = _strlen(root);
	}

	if (getcwd(path, sizeof(path)) != NULL) {
		path_len = _strlen(path);

		// add slash to the end of 'p' because
		// getcwd() does not return string with a slash
		if (path_len+2 < sizeof(path) && path[path_len-1] != '/') {
			path[path_len] = '/';
			path[path_len+1] = '\0';
		}

		// need to check root_len first
		if (root_len > 0 && _strncmp(root, path, root_len) == 0) {
			snprintf(s, n, "%s", (path + root_len));
		} else {
			snprintf(s, n, "%s", path);
		}

		return s;
	} else {
		return NULL;
	}
}

/*
 * description
 * Initiate variables and functionality
 * 
 * return
 * '0' on success
 * '>0' on fail (will be program exit code)
 */
static int init(struct command_info *c)
{
	int ret = EXIT_NORMAL; // return value

	if (c == NULL) return EXIT_INIT_FAILED;

	// initialize 'struct command_info'
	memset(c, 0x0, sizeof(struct command_info));
	c->mode = MODE_INVALID; // -1

	// get root path
	if (get_svn_rd(c->svn_root_path, MAX_PATH_LEN) < 0) {
		ret = EXIT_NOT_IN_SVN_REPO;
		goto EXIT;
	}

	// get configuration file
	ret = get_config(c);
	if (ret < 0) {
		ret = EXIT_FAILED_TO_GET_CONFIG;
		goto EXIT;
	}

	// get current working directory
	if (get_cwd_no_root(c->cwd, MAX_PATH_LEN, c->svn_root_path) == NULL) {
		ret = EXIT_INIT_FAILED;
		goto EXIT;
	}

EXIT:
	d("Finished initalization [%d]", ret);
	return ret;
}

/*
 * description
 * The de-initiation function.
 * Very important if you don't want un-managed memory
 * lingering in your system.
 */
static void deinit(struct command_info *c)
{
	free(c->opt_arg);
	free(c->act_arg);

	dj(c->j_config);
	json_decref(c->j_config);

	d("Finished de-initalization");
}

/*
 * description
 * It's the most important function.
 * Thats it.
 */
int main(int argc, char** argv)
{
	struct command_info cmd;
	int exit_code = EXIT_NORMAL;
	int i = 0;

	exit_code = init(&cmd);
	if (exit_code < 0) goto END_PROG;

	exit_code = option_parse(&cmd, argc, argv);
	if (exit_code < 0) goto END_PROG;

	d("cmd.mode [%d]", cmd.mode);
	d("cmd.opt_arg_cnt [%d]", cmd.opt_arg_cnt);
	d("cmd.act_arg_cnt [%d]", cmd.act_arg_cnt);
	for (i=0; i<cmd.opt_arg_cnt; i++) {
		d("cmd.opt_arg[%d] [%s]", i, cmd.opt_arg[i]);
	}
	for (i=0; i<cmd.act_arg_cnt; i++) {
		d("cmd.act_arg[%d] [%s]", i, cmd.act_arg[i]);
	}
	d("cmd.svn_root_path [%s]", cmd.svn_root_path);
	d("cmd.config_path [%s]", cmd.config_path);
	d("cmd.cwd [%s]", cmd.cwd);
	d("cmd.group_name [%s]", cmd.group_name);

	// note: never delete the json array on clean. just pop the data out
	switch(cmd.mode) {
		case MODE_ADD:
		case MODE_ADD_SHORT:
		{
			_add(&cmd);
			save_config(&cmd);
			break;
		}
		case MODE_REMOVE:
		case MODE_REMOVE_SHORT:
		{
			_remove(&cmd);
			save_config(&cmd);
			break;
		}
		case MODE_SHOW:
		{
			_show(&cmd);
			break;
		}
		default:
		{
			help();
			exit_code = EXIT_ACTION_NOT_FOUND;
			d("found no action argument, ending program");
		}
	}

END_PROG:
	deinit(&cmd);

	switch(exit_code) {
		case EXIT_NOT_IN_SVN_REPO: {
			errout("not inside a SVN repository! [-.-]");
			break;
		}
		case EXIT_ACTION_NOT_FOUND: {
			errout("no action received. [-.-]");
			help();
			break;
		}
		case EXIT_NO_GROUP_NAME: {
			errout("no group name was indicated in your options. [T.-]");
			help();
			break;
		}
		case EXIT_UNKNOWN_OPTION: {
			errout("unknown option received. [o.O]");
			help();
			break;
		}
		case EXIT_INVALID_OPTION: {
			errout("invalid option received. [o.O]");
			help();
			break;
		}
		case EXIT_HELP_PAGE: {
			help();
			break;
		}
		case EXIT_NORMAL: {
			out("ending with no errors, what a surprise. [^.^][%d]", exit_code);
			break;
		}
		default: {
			out("Unknown exit code.");
		}
	}

	return exit_code;
}
