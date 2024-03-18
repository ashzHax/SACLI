#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * To save only the JSON root
 * All JSON pointers are borrowed reference from it,
 * so simply just updating that should work
 */
static void overwrite_save_config(struct command_info *c)
{
	if (c == NULL) {
		return;
	}

	dj(c->json_cfg);

	if (json_dump_file(c->json_cfg, c->config_path, JSON_INDENT(1)) < 0) {
		errout("failed to dump configuration data to file");
	} else {
		d("data dump successful");
	}
}

/*
 * description
 * Handler for the 'overwrite' action command.
 */
int _overwrite(struct command_info* c)
{
	int return_value = EXIT_NORMAL;

	const char *des_name = NULL;
	const char *src_name = NULL;

	json_t *list = NULL;
	json_t *des = NULL;
	json_t *src = NULL;

	char *src_path = NULL;
	size_t src_size = 0;
	char *des_path = NULL;
	size_t des_size = 0;

	char *cmd = NULL;
	size_t cmd_size = 0;

	if (c == NULL) {
		errout("command_info is NULL");
		return_value = EXIT_OW_ERROR;
		goto EXIT;
	}

	d("starting 'overwrite' command handler");
	
	// argument check
	if (c->act_arg_cnt == 0) {
		errout("not enough arguments to run overwrite");
		return_value = EXIT_OW_ERROR_NOT_ENOUGH_ARGS;
		goto EXIT;
	}
	if (c->act_arg_cnt > 2) {
		errout("too many arguments to run overwrite");
		return_value = EXIT_OW_ERROR_TOO_MANY_ARGS;
		goto EXIT;
	}

	d("overwriting group [%s] to [%s]", c->act_arg[0], c->act_arg[1]);
	des_name = c->act_arg[0];
	src_name = c->act_arg[1];

	list = json_object_get(c->json_cfg, "groups");
	if (list == NULL) {
		errout("failed to get list");
		return_value = EXIT_OW_ERROR;
		goto EXIT;
	}

	// get source group JSON object
	src = json_object_get(list, src_name);
	if (src == NULL) {
		errout("group does not exist [%s]", src_name);
		return_value = EXIT_OW_ERROR_GROUP_NOT_FOUND;
		goto EXIT;
	}

	// get destination group JSON object
	des = json_object_get(list, des_name);

	// if destination group does exists, delete it first
	if (des != NULL) {
		json_decref(des);
	}

	// create a copy of source group
	des = json_deep_copy(src);

	// put source files to destination object, and update list
	json_object_set_new(list, des_name, des);

	// get source group comment file path
	src_size = sizeof(char) * 
		(_strlen(c->root_path) + _strlen(src_name) + MAX_PATH_LEN);
	src_path = (char*)malloc(src_size);
	if (src_path == NULL) {
		errout("malloc() failed");
		return_value = EXIT_OW_ERROR;
		goto EXIT;
	}
	snprintf(src_path, src_size, "%s/"CMT_FILE, c->root_path, src_name);

	// get destination group comment file path
	des_size = sizeof(char) * 
		(_strlen(c->root_path) + _strlen(des_name) + MAX_PATH_LEN);
	des_path = (char*)malloc(des_size);
	if (des_path == NULL) {
		errout("malloc() failed");
		return_value = EXIT_OW_ERROR;
		goto EXIT;
	}
	snprintf(des_path, des_size, "%s/"CMT_FILE, c->root_path, des_name);

	// check if source group comment exists
	if (access(src_path, F_OK) != 0) {
		errout("no source comment file found");	
		return_value = EXIT_OW_ERROR_NO_COMMENT_FILE;
		goto EXIT;
	}

	// going to use 'cp' command for this operation
	// because I'm lazy, and this works
	// not all that safe, yes, but thats on the user :/
	cmd_size = src_size + des_size + (sizeof(char) * 18);
	cmd = (char*)malloc(cmd_size);
	if (cmd == NULL) {
		errout("malloc() failed");
		return_value = EXIT_OW_ERROR;
		goto EXIT;
	}
	snprintf(cmd, cmd_size, "cp %s %s 2> /dev/null", src_path, des_path);

	d("running command: [%s]", cmd);
	system(cmd);

	// save new configuration
	overwrite_save_config(c);

EXIT:
	if (src_path) {
		free(src_path);
	}
	if (des_path) {
		free(des_path);
	}
	if (cmd) {
		free(cmd);
	}

	return return_value;
}
