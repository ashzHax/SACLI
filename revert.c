#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'revert' action command
 */
int _revert(struct command_info* c)
{
	int return_value = EXIT_NORMAL;

	size_t index;
	json_t *value;

	char *cmd = NULL;
	size_t size = 0;

	if (c == NULL) {
		errout("command_info is NULL");
		return_value = EXIT_REVERT_ERROR;
		goto EXIT;
	}

	d("starting 'revert' command handler");

	size = sizeof(char) * (MAX_CMD_LEN + _strlen(c->root_path));
	cmd = (char*)malloc(size);
	if (cmd == NULL) {
		errout("malloc() failed");
		return_value = EXIT_REVERT_ERROR;
		goto EXIT;
	}

	json_array_foreach(c->json_files, index, value) {
		snprintf(cmd, size, "svn revert %s/%s", c->root_path, json_string_value(value));
		d("running command: [%s]", cmd);
		system(cmd);
	}

	out("reverted all files inside group [%s]", c->grp_name);

EXIT:
	if (cmd) {
		free(cmd);
	}

	return return_value;
}
