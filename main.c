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
extern int _comment(struct command_info* c);
extern int _commit(struct command_info* c);
extern int _clear(struct command_info* c);
extern int _overwrite(struct command_info* c);
extern int _edit(struct command_info* c);
extern int _revert(struct command_info* c);
extern int _rollback(struct command_info* c);
extern int _auto(struct command_info* c);
extern int _target(struct command_info* c);
extern int _info(struct command_info* c);

/*
 * note
 * string list of command modes, for sifting user input
 * list order must match enum defined in global.h
 */
const char* mode_list[MODE_MAX] = {
	"add",
	"remove",
	"rm",
	"show",
	"comment",
	"commit",
	"clear",
	"overwrite",
	"over",
	"edit",
	"help",
	"revert",
	"rollback",
	"auto",
	"target",
	"info"
};

/*
 * description
 * Show the user how to use this amazing command
 */
static void help()
{
	out("usage: svm [options] [action] [...]");
	out("< add {file} >                         Add file to the commit group");
	out("< remove {file} >                      Remove file from the commit group");
	out("< show >                               Show all files inside commit group");
	out("< comment >                            Edit the comment of the commit group");
	out("< commit >                             Start the commit process!");
	out("< clear >                              Remove all files inside the commit group specified");
	out("< overwrite {dest_group} {src_group} > Overwrite destination group with source group (Use with caution!)");
	out("< edit [all/selective] >               Edit files inside the commit group (Default: 'all')");
	out("< help >                               Shows this help page");
	out("< revert >                             Revert all modifications done to the files inside group");
	out("< rollback {revision} {files} >        Get the specified file at specified revision");
	out("< auto >                               Automatically get modified files via 'svn status' (Reference paths only specified in '/.c.target' if file exists)");
	out("< target {path} >                      Path to search for when using the command 'auto' (This command creates and opens '/.c.target')");
}

/*
 * description
 * Save 'c->json_grp' as 'c->grp_name' to c->json_cfg
 * Dump 'c->json_cfg' to file
 */
static void save_config(struct command_info *c)
{
	json_t *group_list = NULL;

	if (c == NULL) {
		return;
	}

	group_list = json_object_get(c->json_cfg, "groups");

	if (group_list == NULL) {
		errout("unable to get 'groups' object");
		return;
	}

	// since 'c->json_grp' and 'c->json_files' might be a new object,
	// always update the value
	json_object_set(c->json_grp, "files", c->json_files);
	json_object_set(group_list, c->grp_name, c->json_grp);
	dj(c->json_cfg);

	if (json_dump_file(c->json_cfg, c->config_path, JSON_INDENT(1)) < 0) {
		errout("failed to dump configuration data to file");
	} else {
		d("data dump successful");
	}
}

/*
 * description
 * Creates a new group
 * 
 * return
 * '0' on success
 * '>0' on fail (will be program exit code)
 */
int create_group(struct command_info *c)
{
	c->json_grp = json_object();
	c->json_files = json_array();

	if (c->json_grp != NULL && c->json_files != NULL) {
		alert("created new group [%s]", c->grp_name);
	} else {
		errout("unable to create new group [%s]", c->grp_name);
		return EXIT_ERROR_CREATE_GROUP_FAIL;
	}

	return EXIT_NORMAL;
}


/*
 * description
 * Option handler for '-h'
 *
 * return
 * '0' on success
 * '>0' on fail (will be program exit code)
 */
int option_handle_help()
{
	return EXIT_HELP_PAGE;
}

/*
 * description
 * Option handler for '-g'
 *
 * return
 * '0' on success
 * '>0' on fail (will be program exit code)
 */
int option_handle_group(struct command_info *c)
{
	json_t *group_list = NULL;
	int ret = EXIT_NORMAL;

	// check if option is valid for current mode
	switch (c->mode) {
		case MODE_ADD:
		case MODE_REMOVE:
		case MODE_SHOW:
		case MODE_COMMENT:
		case MODE_COMMIT:
		case MODE_EDIT:
		case MODE_CLEAR:
		case MODE_REVERT: 
		case MODE_AUTO: {
			break;
		}
		default: {
			errout("invalid option for this operation [%s]", mode_list[c->mode]);
			return EXIT_ERROR_INVALID_OPTION;
		}
	}

	// check validity of received data
	group_list = json_object_get(c->json_cfg, "groups");
	c->json_grp = json_object_get(group_list, c->grp_name);

	if (c->json_grp == NULL) {
		// if group is not found, create it
		ret = create_group(c);
	} else {
		// if group is found
		c->json_files = json_object_get(c->json_grp, "files");
	}

	return ret;
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
	int action = 0;
	json_t *group_list = NULL;
	int ret = EXIT_NORMAL;

	for (i=1; i<argc; i++) {
		_strcpy(argv[i], argt, MAX_ARG_LEN); 
		_strlow(argt);
		d("validating argument [%s]", argt);

		// finding action parameter
		for (j=0; j<MODE_MAX; j++) {
			if (_strfcmp(argt, mode_list[j]) == 0) {
				c->mode = j;
				d("found mode [%s]", mode_list[j]);
				break;
			}
		}

		if (c->mode != MODE_INVALID) {
			// cannot go below 0 (in theory)
			c->act_arg_cnt = argc - i - 1;
			action = i;
			break;
		}
		c->opt_arg_cnt++;
	}
	d("action[%d] opt_cnt[%d] act_cnt[%d]", action, c->opt_arg_cnt, c->act_arg_cnt);

	if (c->mode == MODE_INVALID) {
		return EXIT_ERROR_NO_ACTION;
	}

	c->opt_arg = (const char**)malloc(sizeof(const char*)*c->opt_arg_cnt);
	for (i=0; i<c->opt_arg_cnt; i++) {
		c->opt_arg[i] = argv[i+1];
		d("opt_args[%d] = argv[%d] = %s", i, i+1, argv[i+1]);
	}

	c->act_arg = (const char**)malloc(sizeof(const char*)*c->act_arg_cnt);
	for (i=0; i<c->act_arg_cnt; i++) {
		c->act_arg[i] = argv[action+1+i];
		d("act_args[%d] = argv[%d] = %s", i, action+1+i, argv[action+1+i]);
	}

	/*
	 * How to parse the options?
	 * First, organize all options into the proper data structure
	 * Get the option indicator, then parse the backdata to a variable
	 * Second, make sure the options received are valid for the current mode
	 * Third, check the validity of the data received
	 * Fourth, if data is not used else where, use it now
	 */
	for (i=1; i < c->opt_arg_cnt+1; i++) {
		if (_strncmp(argv[i], "-h", (size_t)2) == 0) {
			d("got [-h] option");

			ret = option_handle_help();
		} else if (_strncmp(argv[i], "-g", (size_t)2) == 0) {
			d("got [-g] option");

			if (_strlen(argv[i]) > 2) {
				// group name is in current option argument
				snprintf(c->grp_name, MAX_GROUP_NAME_LEN, "%s", argv[i]+2);
			} else {
				// group name is in a separate option argument

				// check if there are no more arguments
				if (i >= c->opt_arg_cnt) {
					// has '-g' option, but no group name was included
					ret = EXIT_ERROR_NO_GROUP_NAME;
				}
				
				// skip to the next option argument, which is the group name
				i++;

				snprintf(c->grp_name, MAX_GROUP_NAME_LEN, "%s", argv[i]);
			}
			d("found group name [%s] from option", c->grp_name);

			ret = option_handle_group(c);
		} else {
			errout("found unknown option [%s]", argv[i]);
			ret = EXIT_ERROR_UNKNOWN_OPTION;
		}
	
		if (ret < EXIT_NORMAL) {
			return ret;
		}
	}

	// if no group was found by this point, set 'json_cfg' to default group
	if (c->json_grp == NULL) {
		snprintf(c->grp_name, MAX_GROUP_NAME_LEN, "default");

		group_list = json_object_get(c->json_cfg, "groups");

		c->json_grp = json_object_get(group_list, c->grp_name);
		c->json_files = json_object_get(c->json_grp, "files");

		if (c->json_grp == NULL) {
			errout("unable to get group [default]");
			return EXIT_ERROR_GROUP_GET_FAIL;
		}

		if (c->json_files == NULL) {
			errout("unable to get group's file list [default]");
			return EXIT_ERROR_FILE_LIST_GET_FAIL;
		}
	}

	return EXIT_NORMAL;
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

	if (buf == NULL || len <= 0) {
		return EXIT_ERROR_ROOT_PATH_GET_FAIL;
	}

	pp = popen("svn info 2> /dev/null | grep \"Working Copy Root Path\" | awk \'{print $5}\'", "r");
	if (pp) {
		fgets(buf, len, pp);
		pclose(pp);
	}

	if (_strlen(buf) <= 0) {
		return EXIT_ERROR_ROOT_PATH_GET_FAIL;
	}

	rm_whitespace(buf);

	return EXIT_NORMAL;
}

/*
 * description
 * Creates the configuration file, and fills it with the default values
 */
static int create_config_file(struct command_info *c)
{
	FILE *fp = NULL;
	json_t *new = NULL;
	json_t *group = NULL;
	json_t *_default = NULL; // default group

	fp = fopen(c->config_path, "w+");
	if (fp == NULL) {
		errout("failed to open/create configuration file");
		return EXIT_ERROR_FOPEN_FAIL;
	}

	new = json_object();
	group = json_object();
	_default = json_object();

	if (new == NULL || group == NULL) {
		errout("json_object() failed");
		return EXIT_ERROR;
	}

	// object -> groups -> default -> files -> []
	json_object_set(_default, "files", json_array());
	json_object_set(group, "default", _default);
	json_object_set(new, "groups", group);

	json_dumpf(new, fp, JSON_INDENT(1));

	// close
	json_decref(new);
	fclose(fp);

	return EXIT_NORMAL;
}

/*
 * description
 * For getting configuration file
 * Creates the file with default values if it doesn't exist
 * Reads the file and parse it into a 'json_t' object
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
	int ret = EXIT_NORMAL;
	
	// get configuration file path
	if (c == NULL) {
		errout("command_info is NULL");
		return EXIT_ERROR;
	}

	len = _strlen(c->root_path) + _strlen(CFG_FILE) + 2;
	snprintf(c->config_path, len, "%s/%s", c->root_path, CFG_FILE);
	d("configuration file path [%s]", c->config_path);

	// check if configuration file exists
	// if not, create file with default values
	if (access(c->config_path, F_OK) != 0) {
		ret = create_config_file(c);
		if (ret < EXIT_NORMAL) {
			return ret;
		}
	}

	// open configuration file
	fp = fopen(c->config_path, "r");
	if (fp == NULL) {
		errout("failed to open configuration file");
		return EXIT_ERROR_FOPEN_FAIL;
	}

	// load configuration file as JSON
	c->json_cfg = json_loadf(fp, (JSON_DECODE_ANY | JSON_DISABLE_EOF_CHECK), &error);

	// close
	fclose(fp);

	// debug output JSON structure
	dj(c->json_cfg);

	return ret;
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

	if (s == NULL || n <= 0) {
		return NULL;
	}

	if (root != NULL) {
		root_len = _strlen(root);
	}

	if (getcwd(path, sizeof(path)) != NULL) {
		// remove slash to the end of 'path' if it exists
		rm_slash(path);
		d("getcwd() result (after rm_slash()) [%s]", path);

		// need to check root_len
		if (root_len > 0 && _strncmp(root, path, root_len) == 0) {
			snprintf(s, n, "%s", (path+root_len+1));
		} else {
			snprintf(s, n, "%s", path);
		}

		// in case the current location is svn root
		if (_strlen(s) <= 0) {
			snprintf(s, n, ".");
		}

		return s;
	} else {
		return NULL;
	}
}

/*
 * description
 * Gets the 'auto' command target file's full path
 *
 * return
 * The parameter buffer pointer is returned on success
 * NULL is returned on fail
 */
static char* get_auto_target_file(char* s, size_t n, char* root)
{
	if (s == NULL || n <= 0 || root == NULL) {
		return NULL;
	}

	snprintf(s, n, "%s/%s", root, TRG_FILE);

	return s;
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
	int ret = EXIT_NORMAL;

	if (c == NULL) {
		return EXIT_ERROR;
	}

	// initialize 'struct command_info'
	memset(c, 0x0, sizeof(struct command_info));
	c->mode = MODE_INVALID; // -1

	// get root path
	ret = get_svn_rd(c->root_path, MAX_PATH_LEN);
	if (ret < EXIT_NORMAL) {
		goto EXIT;
	}
	d("svn root path [%s]", c->root_path);

	// get configuration file
	ret = get_config(c);
	if (ret < EXIT_NORMAL) {
		goto EXIT;
	}

	// get current working directory
	if (get_cwd_no_root(c->cwd, MAX_PATH_LEN, c->root_path) == NULL) {
		ret = EXIT_ERROR_CWD_GET_FAIL;
		goto EXIT;
	}
	d("current working directory [%s]", c->cwd);

	// get auto target file
	if (get_auto_target_file(c->auto_path, MAX_PATH_LEN, c->root_path) == NULL) {
		ret = EXIT_ERROR_AUTO_FILE_GET_FAIL;
		goto EXIT;
	}
	d("auto target list file path [%s]", c->auto_path);

EXIT:
	d("finished initalization with code [%d]", ret);
	return ret;
}

/*
 * description
 * The de-initiation function
 * Very important if you don't want un-managed memory
 * lingering in your system
 */
static void deinit(struct command_info *c)
{
	free(c->opt_arg);
	free(c->act_arg);

	json_decref(c->json_cfg);

	d("finished de-initalization");
}

/*
 * description
 * It's the most important function
 * Thats it for the explaination
 */
int main(int argc, char** argv)
{
	struct command_info cmd;
	int exit_code = EXIT_NORMAL;
	int i = 0;

	exit_code = init(&cmd);
	if (exit_code < EXIT_NORMAL) {
		goto END_PROG;
	}

	exit_code = option_parse(&cmd, argc, argv);
	if (exit_code < EXIT_NORMAL) {
		goto END_PROG;
	}

	// command_info structure debug
	d("cmd.mode [%d]", cmd.mode);
	d("cmd.opt_arg_cnt [%d]", cmd.opt_arg_cnt);
	d("cmd.act_arg_cnt [%d]", cmd.act_arg_cnt);
	for (i=0; i<cmd.opt_arg_cnt; i++) {
		d("cmd.opt_arg[%d] [%s]", i, cmd.opt_arg[i]);
	}
	for (i=0; i<cmd.act_arg_cnt; i++) {
		d("cmd.act_arg[%d] [%s]", i, cmd.act_arg[i]);
	}
	d("cmd.root_path [%s]", cmd.root_path);
	d("cmd.config_path [%s]", cmd.config_path);
	d("cmd.cwd [%s]", cmd.cwd);
	d("cmd.grp_name [%s]", cmd.grp_name);
	d("cmd.auto_path [%s]", cmd.auto_path);

	// note: never delete the json array on clean,
	// just pop the data out
	switch(cmd.mode) {
		case MODE_ADD:
		{
			exit_code = _add(&cmd);
			save_config(&cmd);
			break;
		}
		case MODE_REMOVE:
		case MODE_REMOVE_SHORT:
		{
			exit_code = _remove(&cmd);
			save_config(&cmd);
			break;
		}
		case MODE_SHOW:
		{
			exit_code = _show(&cmd);
			break;
		}
		case MODE_COMMENT:
		{
			exit_code = _comment(&cmd);
			save_config(&cmd);
			break;
		}
		case MODE_COMMIT:
		{
			exit_code = _commit(&cmd);
			break;
		}
		case MODE_CLEAR:
		{
			exit_code = _clear(&cmd);
			save_config(&cmd);
			break;
		}
		case MODE_OVERWRITE:
		case MODE_OVERWRITE_SHORT:
		{
			exit_code = _overwrite(&cmd);
			// will save using a custom function, since this will break if default
			// group is the destination
			//save_config(&cmd);
			break;
		}
		case MODE_EDIT:
		{
			exit_code = _edit(&cmd);
			break;
		}
		case MODE_HELP:
		{
			help();
			break;
		}
		case MODE_REVERT:
		{
			exit_code = _revert(&cmd);
			break;
		}
		case MODE_ROLLBACK:
		{
			exit_code = _rollback(&cmd);
			break;
		}
		case MODE_AUTO:
		{
			exit_code = _auto(&cmd);
			save_config(&cmd);
			break;
		}
		case MODE_TARGET:
		{
			exit_code = _target(&cmd);
			break;
		}
		case MODE_INFO:
		{
			exit_code = _info(&cmd);
			break;
		}
	}

END_PROG:
	deinit(&cmd);

	switch(exit_code) {
		case EXIT_NORMAL: {
			out("ending program");
			break;
		}
		case EXIT_ERROR: {
			errout("error has occurred, ending program");
			break;
		}
		case EXIT_ERROR_NO_ACTION: {
			errout("no action found");
			help();
			break;
		}
		case EXIT_ERROR_ROOT_PATH_GET_FAIL: {
			errout("failed to get svn root path");
			errout("are you sure you are inside an svn repository?");
			break;
		}
		case EXIT_ERROR_FOPEN_FAIL: {
			break;
		}
		case EXIT_ERROR_CWD_GET_FAIL: {
			errout("failed to get current working directory path");
			break;
		}
		case EXIT_ERROR_AUTO_FILE_GET_FAIL: {
			errout("failed to get auto target list file");
			break;
		}
		case EXIT_ERROR_NO_GROUP_NAME: {
			errout("no group name was specified");
			help();
			break;
		}
		case EXIT_ERROR_UNKNOWN_OPTION: {
			help();
			break;
		}
		case EXIT_ERROR_GROUP_GET_FAIL: {
			break;
		}
		case EXIT_ERROR_FILE_LIST_GET_FAIL: {
			break;
		}
		case EXIT_ERROR_INVALID_OPTION: {
			break;
		}
		case EXIT_HELP_PAGE: {
			help();
			exit_code = EXIT_NORMAL; // since this is technically not an error
			break;
		}
		case EXIT_ERROR_CREATE_GROUP_FAIL: {
			break;
		}
		case EXIT_ADD_ERROR:
		case EXIT_ADD_ERROR_INVALID_PATH:
		case EXIT_ADD_ERROR_DUPLICATE_PATH: {
			break;
		}
		case EXIT_REMOVE_ERROR:
		case EXIT_REMOVE_ERROR_FILE_NOT_FOUND:
		case EXIT_REMOVE_ERROR_JSON_REMOVE_FAIL: {
			break;
		}
		case EXIT_AUTO_ERROR:
		case EXIT_AUTO_ERROR_FOPEN_FAIL: {
			break;
		}
		case EXIT_CLEAR_ERROR: {
			break;
		}
		case EXIT_COMMENT_ERROR:
		case EXIT_COMMENT_ERROR_FOPEN_FAIL: {
			break;
		}
		case EXIT_COMMIT_ERROR:
		case EXIT_COMMIT_ERROR_FOPEN_FAIL: {
			break;
		}
		case EXIT_EDIT_ERROR: {
			break;
		}
		case EXIT_OW_ERROR:
		case EXIT_OW_ERROR_GROUP_NOT_FOUND:
		case EXIT_OW_ERROR_NO_SRC_FILES:
		case EXIT_OW_ERROR_NO_COMMENT_FILE: {
			break;
		}
		case EXIT_OW_ERROR_NOT_ENOUGH_ARGS: {
			help();
			errout("not enough arguments");
			break;
		}
		case EXIT_OW_ERROR_TOO_MANY_ARGS: {
			help();
			errout("too many arguments");
			break;
		}
		case EXIT_REVERT_ERROR: {
			break;
		}
		case EXIT_RB_ERROR:
		case EXIT_RB_ERROR_INVALID_REVISION: {
			break;
		}
		case EXIT_RB_NOT_ENOUGH_ARGS: {
			help();
			errout("not enough arguments");
			break;
		}
		case EXIT_SHOW_ERROR: {
			break;
		}
		case EXIT_TARGET_ERROR: {
			break;
		}
		case EXIT_INFO_ERROR: {
			break;
		}
		default: {
			errout("undefined exit code [%d]", exit_code);
		}
	}

	return exit_code;
}
