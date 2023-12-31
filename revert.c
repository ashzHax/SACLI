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
 * since having a extremely long string is just stupid
 * going to run multiple commands
 * could make a max string to optimize the amount of commands that runs
 */
#define SINGULAR_STRING_CMD 0

/*
 * description
 * Handler for the 'revert' action command.
 */
int _revert(struct command_info* c)
{
	int return_value = EXIT_NORMAL;
	size_t index;
	json_t *value;
	char cmd[MAX_CMD_LEN] = {0};

	if (c == NULL) {
		d("critical error, 'c' is NULL");
		return_value = EXIT_REVERT_FAILED;
		goto EXIT;
	}

	d("starting 'revert' command handler");

#if SINGULAR_STRING_CMD
	{
		int pos = 0;
		pos = snprintf(cmd, sizeof(cmd), "svn revert ");
		json_array_foreach (c->j_files, index, value) {
			pos += snprintf(cmd+pos, sizeof(cmd)-pos, "%s ",
				json_string_value(value));
		}
		d("-> [%s]", cmd);
		system(cmd);
	}
#else
	json_array_foreach (c->j_files, index, value) {
		snprintf(cmd, sizeof(cmd), "svn revert %s", json_string_value(value));
		d("cmd check -> [%s]", cmd);
		system(cmd);
	}
	
#endif


	out("reverted all files inside group %s", c->group_name);
EXIT:
	return return_value;
}
