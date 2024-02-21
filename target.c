/*
 * WARNING!
 * Before copying code to this file, make sure it's as perfect
 * as you can make it to be, since no debug logs should be
 * recorded here.
 * Eh, who cares?
 */

#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'revert' action command.
 */
int _target(struct command_info* c)
{
	int return_value = EXIT_NORMAL;
	char cmd[MAX_CMD_LEN+MAX_PATH_LEN] = {0};

	if (c == NULL) {
		d("critical error, 'c' is NULL");
		return_value = EXIT_TARGET_FAILED;
		goto EXIT;
	}

	d("starting 'target' command handler");

	// open target file
	snprintf(cmd, sizeof(cmd), "vi %s", c->auto_target_path);
	d("cmd: [%s]", cmd);
	system(cmd);

EXIT:
	return return_value;
}
