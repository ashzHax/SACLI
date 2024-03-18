#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'clear' action command
 * Clears a group's file list
 */
int _clear(struct command_info* c)
{
	int return_value = EXIT_NORMAL;

	if (c == NULL) {
		errout("command_info is NULL");
		return_value = EXIT_CLEAR_ERROR;
		goto EXIT;
	}

	d("starting 'clear' command handler");

	json_array_clear(c->json_files);

	out("cleared all files in group [%s]", c->grp_name);
EXIT:
	return return_value;
}
