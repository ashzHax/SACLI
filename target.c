#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'target' action command
 */
int _target(struct command_info* c)
{
	int return_value = EXIT_NORMAL;

	size_t size = 0;
	char *cmd = NULL;

	if (c == NULL) {
		errout("command_info is NULL");
		return_value = EXIT_TARGET_ERROR;
		goto EXIT;
	}

	d("starting 'target' command handler");

	size = sizeof(char) * (MAX_CMD_LEN + _strlen(c->auto_path));
	cmd = (char*)malloc(size);
	if (cmd == NULL) {
		errout("malloc() failed");
		return_value = EXIT_TARGET_ERROR;
		goto EXIT;
	}

	// open target file
	// which is now very simplified
	snprintf(cmd, size, "vi %s", c->auto_path);
	d("running command: [%s]", cmd);
	system(cmd);

EXIT:
	if (cmd) {
		free(cmd);
	}

	return return_value;
}
