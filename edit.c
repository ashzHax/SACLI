#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'edit' action command
 */
int _edit(struct command_info* c)
{
	int return_value = EXIT_NORMAL;

	size_t index;
	json_t *value;

	const char *s = NULL;
	char cmd[MAX_PATH_LEN] = {0};

	if (c == NULL) {
		errout("command_info is NULL");
		return_value = EXIT_EDIT_ERROR;
		goto EXIT;
	}

	d("starting 'edit' command handler");

	json_array_foreach (c->json_files, index, value) {
		s = json_string_value(value);
		if (s) {
			snprintf(cmd, sizeof(cmd), "vi %s", s);
			d("running command: [%s]", cmd);
			system(cmd);
		} else {
			errout("failed to get string value from JSON, skipping");
		}
	}

EXIT:
	return return_value;
}
