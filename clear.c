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
 * Handler for the 'clear' action command.
 */
int _clear(struct command_info* c)
{
	int return_value = EXIT_NORMAL;

	if (c == NULL) {
		d("critical error, 'c' is NULL");
		return_value = EXIT_ADD_FAILED;
		goto EXIT;
	}

	d("starting 'clear' command handler");

	json_array_clear(c->j_files);

	out("cleared all files inside group %s", c->group_name);
EXIT:
	return return_value;
}
