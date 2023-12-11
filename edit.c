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
 * Handler for the 'edit' action command.
 */
int _edit(struct command_info* c)
{
	int return_value = EXIT_NORMAL;
	size_t index;
	json_t *value;
	char cmd[MAX_PATH_LEN+4] = {0};
	const char *sv = NULL;

	if (c == NULL) {
		d("critical error, 'c' is NULL");
		return_value = EXIT_EDIT_FAILED;
		goto EXIT;
	}

	d("starting 'edit' command handler");

	json_array_foreach (c->j_files, index, value) {
		sv = json_string_value(value);
		if (sv) {
			snprintf(cmd, sizeof(cmd), "vi %s", sv);
			system(cmd);
		} else {
			d("failed to open a file (or array loop failed somewhere)");
		}
	}
EXIT:
	return return_value;
}
